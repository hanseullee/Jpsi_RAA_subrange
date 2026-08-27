#include "TCanvas.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TParameter.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"

#include "RooFitResult.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
struct Point
{
  double ptLow;
  double ptHigh;
  double value;
  double stat;
  double syst;
  double fitPvalue;
  TString source;
};

TString macroDir()
{
  TString path = __FILE__;
  if (!gSystem->IsAbsoluteFileName(path))
    path = TString(gSystem->WorkingDirectory()) + "/" + path;
  return gSystem->DirName(path);
}

TString outputPath(const char *fileName)
{
  return macroDir() + "/" + fileName;
}

template <typename T>
bool readParameter(TFile &file, const char *key, T &value)
{
  auto *parameter = dynamic_cast<TParameter<T> *>(file.Get(key));
  if (!parameter)
    return false;
  value = parameter->GetVal();
  return true;
}

bool readSubrangePoint(double ptLow, double ptHigh, Point &point)
{
  const TString path = Form(
      "%s/../../roots/y1.80_2.40/subrange/"
      "subrange_result_y1.80_2.40_pt%.2f_%.2f.root",
      macroDir().Data(), ptLow, ptHigh);
  std::unique_ptr<TFile> file(TFile::Open(path, "READ"));
  if (!file || file->IsZombie())
    return false;

  double value = 0.0;
  double error = 0.0;
  double pvalue = -1.0;
  if (!readParameter(*file, "bFraction", value) ||
      !readParameter(*file, "bFractionErr", error))
    return false;
  readParameter(*file, "ctauPvalue", pvalue);

  point = {ptLow, ptHigh, value, error, 0.0, pvalue, path};
  return true;
}

bool readOld2DFitPoint(double ptLow, double ptHigh, Point &point)
{
  const TString path = Form(
      "/data/hwan/psi2S_RAA_PbPb2018/Macros/pp_Jpsi/"
      "roots/2DFit_No_Weight/Final/"
      "2DFitResult_pt%.1f-%.1f_y1.8-2.4_muPt0.0_"
      "PRw_Effw0_Accw0_PtW0_TnP0.root",
      ptLow, ptHigh);
  std::unique_ptr<TFile> file(TFile::Open(path, "READ"));
  if (!file || file->IsZombie())
    return false;

  auto *fitResults = dynamic_cast<TH1D *>(file->Get("2DfitResults"));
  if (!fitResults)
    return false;

  point = {ptLow, ptHigh, fitResults->GetBinContent(1),
           fitResults->GetBinError(1), 0.0, -1.0, path};
  return true;
}

TGraphErrors *makeStatGraph(const std::vector<Point> &points,
                            double xOffset,
                            int color,
                            int markerStyle,
                            double markerSize)
{
  auto *graph = new TGraphErrors(points.size());
  for (size_t i = 0; i < points.size(); ++i)
  {
    const double x = 0.5 * (points[i].ptLow + points[i].ptHigh) + xOffset;
    graph->SetPoint(i, x, points[i].value);
    graph->SetPointError(i, 0.0, points[i].stat);
  }
  graph->SetMarkerColor(color);
  graph->SetLineColor(color);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerSize(markerSize);
  graph->SetLineWidth(2);
  return graph;
}

TGraphAsymmErrors *makeSystGraph(const std::vector<Point> &points,
                                 double xOffset,
                                 double halfWidth,
                                 int color)
{
  auto *graph = new TGraphAsymmErrors(points.size());
  for (size_t i = 0; i < points.size(); ++i)
  {
    const double x = 0.5 * (points[i].ptLow + points[i].ptHigh) + xOffset;
    graph->SetPoint(i, x, points[i].value);
    graph->SetPointError(i, halfWidth, halfWidth,
                         points[i].syst, points[i].syst);
  }
  graph->SetFillColorAlpha(color, 0.22);
  graph->SetLineColor(color);
  graph->SetLineWidth(1);
  return graph;
}

std::vector<Point> makeRatioPoints(const std::vector<Point> &numerator,
                                   const std::vector<Point> &denominator)
{
  std::vector<Point> ratio;
  for (const auto &num : numerator)
  {
    const auto den = std::find_if(
        denominator.begin(), denominator.end(), [&num](const Point &candidate) {
          return std::abs(num.ptLow - candidate.ptLow) < 1e-9 &&
                 std::abs(num.ptHigh - candidate.ptHigh) < 1e-9;
        });
    if (den == denominator.end())
    {
      std::cerr << Form("[WARN] no legacy 2D-fit match for ratio: pT %.1f-%.1f",
                        num.ptLow, num.ptHigh)
                << std::endl;
      continue;
    }
    if (std::abs(den->value) < 1e-12)
    {
      std::cerr << Form("[WARN] zero legacy 2D-fit value for ratio: pT %.1f-%.1f",
                        num.ptLow, num.ptHigh)
                << std::endl;
      continue;
    }

    const double value = num.value / den->value;
    const double error = std::hypot(
        num.stat / den->value,
        num.value * den->stat / (den->value * den->value));
    ratio.push_back(
        {num.ptLow, num.ptHigh, value, error, 0.0, -1.0, "subrange/legacy_2D"});
  }
  return ratio;
}

void writeCsv(const std::vector<Point> &current,
              const std::vector<Point> &old2D,
              const std::vector<Point> &bph,
              const std::vector<Point> &hin)
{
  std::ofstream out(outputPath("bFraction_pp_fwd_y1p8_comparison_values.csv").Data());
  out << "dataset,energy_TeV,rapidity,method,pt_low,pt_high,value,stat,syst,"
         "fit_pvalue,source\n";
  out << std::setprecision(9);

  const auto writeRows = [&out](const std::vector<Point> &points,
                                const char *dataset,
                                const char *energy,
                                const char *rapidity,
                                const char *method) {
    for (const auto &point : points)
    {
      out << dataset << ',' << energy << ',' << rapidity << ',' << method << ','
          << point.ptLow << ',' << point.ptHigh << ',' << point.value << ','
          << point.stat << ',' << point.syst << ',';
      if (point.fitPvalue >= 0.0)
        out << point.fitPvalue;
      out << ',' << point.source << '\n';
    }
  };

  writeRows(current, "current_subrange", "5.02", "1.8<|y|<2.4",
            "mass-subrange signal yields + ctau fit");
  writeRows(old2D, "legacy_2D_fit", "5.02", "1.8<|y|<2.4",
            "mass-ctau 2D fit");
  writeRows(bph, "BPH-10-002", "7", "1.6<|y|<2.4",
            "published CMS result");
  writeRows(hin, "HIN-16-025", "5.02", "|y|<2.4",
            "published CMS result");
}
} // namespace

void bFraction_fwd_y1p8_pp_comparison()
{
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);

  const double currentEdges[][2] = {
      {3.0, 4.5}, {4.5, 5.5}, {5.5, 6.5}, {6.5, 7.5}, {7.5, 8.5},
      {8.5, 9.5}, {9.5, 11.0}, {11.0, 15.0}, {15.0, 20.0}, {20.0, 40.0}};
  std::vector<Point> current;
  for (const auto &edge : currentEdges)
  {
    Point point{};
    if (readSubrangePoint(edge[0], edge[1], point))
      current.push_back(point);
    else
      std::cerr << Form("[WARN] current subrange result missing: pT %.1f-%.1f",
                        edge[0], edge[1])
                << std::endl;
  }

  const double old2DEdges[][2] = {
      {3.0, 4.5}, {4.5, 5.5}, {5.5, 6.5}, {6.5, 7.5}, {7.5, 8.5},
      {8.5, 9.5}, {9.5, 11.0}, {11.0, 15.0}, {15.0, 20.0}, {20.0, 40.0}};
  std::vector<Point> old2D;
  for (const auto &edge : old2DEdges)
  {
    Point point{};
    if (readOld2DFitPoint(edge[0], edge[1], point))
      old2D.push_back(point);
    else
      std::cerr << Form("[WARN] legacy 2D-fit result missing: pT %.1f-%.1f",
                        edge[0], edge[1])
                << std::endl;
  }

  // CMS pp 7 TeV, 1.6 < |y| < 2.4.
  // Local source already used in bFraction_fwd_pp.C:
  // HEPData DOI 10.17182/hepdata.57532.v1/t7.
  const double bphLow[] = {0.0, 1.25, 2.0, 2.75, 3.5, 4.5, 6.5, 10.0};
  const double bphHigh[] = {1.25, 2.0, 2.75, 3.5, 4.5, 6.5, 10.0, 30.0};
  const double bphValue[] = {0.057, 0.087, 0.113, 0.139,
                             0.160, 0.177, 0.235, 0.374};
  const double bphStat[] = {0.021, 0.014, 0.013, 0.014,
                            0.014, 0.012, 0.016, 0.031};
  const double bphSyst[] = {0.042, 0.022, 0.020, 0.010,
                            0.013, 0.012, 0.012, 0.008};
  std::vector<Point> bph;
  for (size_t i = 0; i < sizeof(bphValue) / sizeof(bphValue[0]); ++i)
    bph.push_back({bphLow[i], bphHigh[i], bphValue[i], bphStat[i],
                   bphSyst[i], -1.0, "10.17182/hepdata.57532.v1/t7"});

  // CMS pp 5.02 TeV, |y| < 2.4.
  // Local JSON: Macros/final_Results/roots/
  // Bfraction_Jpsi_HIN16025_Table1_vsPt_y0_2p4.json
  // HEPData DOI 10.17182/hepdata.80816.v1/t1.
  const double hinLow[] = {6.5, 7.5, 8.5, 9.5, 11.0, 13.0,
                           15.0, 17.5, 20.0, 25.0, 30.0, 35.0};
  const double hinHigh[] = {7.5, 8.5, 9.5, 11.0, 13.0, 15.0,
                            17.5, 20.0, 25.0, 30.0, 35.0, 50.0};
  const double hinValue[] = {0.205, 0.228, 0.262, 0.295, 0.343, 0.391,
                             0.442, 0.484, 0.515, 0.574, 0.555, 0.597};
  const double hinStat[] = {0.002, 0.002, 0.002, 0.002, 0.003, 0.004,
                            0.005, 0.007, 0.007, 0.012, 0.018, 0.021};
  const double hinSyst[] = {0.018, 0.015, 0.018, 0.015, 0.015, 0.017,
                            0.017, 0.020, 0.019, 0.027, 0.039, 0.034};
  std::vector<Point> hin;
  for (size_t i = 0; i < sizeof(hinValue) / sizeof(hinValue[0]); ++i)
    hin.push_back({hinLow[i], hinHigh[i], hinValue[i], hinStat[i],
                   hinSyst[i], -1.0, "10.17182/hepdata.80816.v1/t1"});

  writeCsv(current, old2D, bph, hin);

  //makeStatGraph(points, xOffset, color, markerStyle, markerSize)
  auto *gCurrent = makeStatGraph(current, -0.12, kMagenta + 2, 29, 1.8);
  auto *gOld2D = makeStatGraph(old2D, 0.12, kAzure + 2, 20, 1.15);
  auto *gBphStat = makeStatGraph(bph, -0.08, kGray + 2, 25, 1.1);
  auto *gBphSyst = makeSystGraph(bph, -0.08, 0.20, kGray + 1);
  auto *gHinStat = makeStatGraph(hin, 0.08, kGray + 1, 26, 0.95);
  auto *gHinSyst = makeSystGraph(hin, 0.08, 0.20, kGray - 7);
  const std::vector<Point> ratio = makeRatioPoints(current, old2D);
  auto *gRatio = makeStatGraph(ratio, 0.0, kMagenta + 2, 20, 1.05);

  TCanvas canvas("cBfracComparison", "cBfracComparison", 900, 900);
  TPad topPad("topPad", "topPad", 0.0, 0.30, 1.0, 1.0);
  TPad ratioPad("ratioPad", "ratioPad", 0.0, 0.0, 1.0, 0.30);
  topPad.SetLeftMargin(0.14);
  topPad.SetRightMargin(0.04);
  topPad.SetTopMargin(0.09);
  topPad.SetBottomMargin(0.02);
  topPad.SetTicks(1, 1);
  ratioPad.SetLeftMargin(0.14);
  ratioPad.SetRightMargin(0.04);
  ratioPad.SetTopMargin(0.02);
  ratioPad.SetBottomMargin(0.36);
  ratioPad.SetTicks(1, 1);
  topPad.Draw();
  ratioPad.Draw();
  topPad.cd();

  TH1D frame("frame", "", 50, 0.0, 50.0);
  frame.SetMinimum(0.0);
  frame.SetMaximum(1.);
  frame.GetYaxis()->SetTitle("Nonprompt J/#psi fraction");
  frame.GetYaxis()->CenterTitle();
  frame.GetXaxis()->SetLabelSize(0.0);
  frame.GetXaxis()->SetTitleSize(0.0);
  frame.GetYaxis()->SetTitleSize(0.060);
  frame.GetYaxis()->SetLabelSize(0.052);
  frame.GetYaxis()->SetTitleOffset(1.02);
  frame.Draw("AXIS");

  gBphSyst->Draw("2 SAME");
  gHinSyst->Draw("2 SAME");
  gBphStat->Draw("P SAME");
  gHinStat->Draw("P SAME");
  gOld2D->Draw("P SAME");
  gCurrent->Draw("P SAME");

  TLegend legend(0.43, 0.64, 0.94, 0.88);
  legend.SetBorderSize(0);
  legend.SetFillStyle(0);
  legend.SetTextFont(42);
  legend.SetTextSize(0.027);
  legend.AddEntry(gCurrent, "Current subrange, 1.8 < |y| < 2.4 (stat.)", "lp");
  legend.AddEntry(gOld2D, "Legacy 2D fit, 1.8 < |y| < 2.4 (stat.)", "lp");
  legend.AddEntry(gHinStat, "HIN-16-025, |y| < 2.4 (stat. #oplus syst.)", "lp");
  legend.AddEntry(gBphStat, "BPH-10-002, 1.6 < |y| < 2.4 (stat. #oplus syst.)", "lp");
  legend.Draw();

  TLatex text;
  text.SetNDC();
  text.SetTextFont(62);
  text.SetTextSize(0.045);
  text.DrawLatex(0.14, 0.93, "CMS");
  text.SetTextFont(52);
  text.SetTextSize(0.035);
  text.DrawLatex(0.245, 0.93, "Internal");
  text.SetTextFont(42);
  text.SetTextAlign(31);
  text.SetTextSize(0.032);
  text.DrawLatex(0.96, 0.93, "pp, #sqrt{s} = 5.02 and 7 TeV");
  text.SetTextAlign(11);
  text.SetTextSize(0.027);


  topPad.RedrawAxis();

  ratioPad.cd();
  TH1D ratioFrame("ratioFrame", "", 50, 0.0, 50.0);
  ratioFrame.SetMinimum(0.78);
  ratioFrame.SetMaximum(1.08);
  ratioFrame.GetXaxis()->SetTitle("p_{T}^{J/#psi} (GeV/c)");
  ratioFrame.GetYaxis()->SetTitle("Sub / 2DFit");
  ratioFrame.GetXaxis()->CenterTitle();
  ratioFrame.GetYaxis()->CenterTitle();
  ratioFrame.GetXaxis()->SetTitleSize(0.13);
  ratioFrame.GetXaxis()->SetLabelSize(0.11);
  ratioFrame.GetXaxis()->SetTitleOffset(1.05);
  ratioFrame.GetYaxis()->SetTitleSize(0.11);
  ratioFrame.GetYaxis()->SetLabelSize(0.10);
  ratioFrame.GetYaxis()->SetTitleOffset(0.56);
  ratioFrame.GetYaxis()->SetNdivisions(505);
  ratioFrame.Draw("AXIS");

  TLine unity(0.0, 1.0, 50.0, 1.0);
  unity.SetLineStyle(2);
  unity.SetLineWidth(2);
  unity.SetLineColor(kGray + 2);
  unity.Draw("SAME");
  gRatio->Draw("P SAME");
  ratioPad.RedrawAxis();

  canvas.cd();
  canvas.SaveAs(outputPath("bFraction_pp_fwd_y1p8_comparison.pdf"));
  canvas.SaveAs(outputPath("bFraction_pp_fwd_y1p8_comparison.png"));

  std::cout << "[INFO] current subrange points: " << current.size() << std::endl;
  std::cout << "[INFO] legacy 2D-fit points: " << old2D.size() << std::endl;
  std::cout << "[INFO] subrange / legacy 2D-fit ratio points: " << ratio.size()
            << std::endl;
    std::cout << Form("[INFO] current first point pT %.1f-%.1f: %.8f +/- %.8f, p=%.8g",
                      current.front().ptLow, current.front().ptHigh,
                      current.front().value, current.front().stat,
                      current.front().fitPvalue)
              << std::endl;
}

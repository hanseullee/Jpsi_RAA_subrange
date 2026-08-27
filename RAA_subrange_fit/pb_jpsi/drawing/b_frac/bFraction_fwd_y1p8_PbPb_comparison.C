#include "TCanvas.h"
#include "TFile.h"
#include "TGraphErrors.h"
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

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
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
  double fitPvalue;
  int fitStatus;
  int covQual;
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
      "subrange_result_c0_180_y1.80_2.40_pt%.2f_%.2f.root",
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

  int status = -1;
  int covQual = -1;
  if (auto *fitResult =
          dynamic_cast<RooFitResult *>(file->Get("ctauFitResult")))
  {
    status = fitResult->status();
    covQual = fitResult->covQual();
  }

  point = {ptLow, ptHigh, value, error, pvalue, status, covQual, path};
  return true;
}

bool readLegacy2DFitPoint(double ptLow, double ptHigh, Point &point)
{
  const TString path = Form(
      "/data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi_260601/"
      "roots/2DFit_No_Weight/Final/"
      "2DFitResult_pt%.1f-%.1f_y1.8-2.4_muPt0.0_"
      "centrality0-180_PRw_Effw0_Accw0_PtW0_TnP0.root",
      ptLow, ptHigh);
  std::unique_ptr<TFile> file(TFile::Open(path, "READ"));
  if (!file || file->IsZombie())
    return false;

  auto *fitResults = dynamic_cast<TH1D *>(file->Get("2DfitResults"));
  if (!fitResults)
    return false;

  point = {ptLow, ptHigh, fitResults->GetBinContent(1),
           fitResults->GetBinError(1), -1.0, -1, -1, path};
  return true;
}

TGraphErrors *makeGraph(const std::vector<Point> &points,
                        double xOffset,
                        int color,
                        int markerStyle,
                        double markerSize,
                        bool showBinWidth = false)
{
  auto *graph = new TGraphErrors(points.size());
  for (size_t i = 0; i < points.size(); ++i)
  {
    const double x = 0.5 * (points[i].ptLow + points[i].ptHigh) + xOffset;
    const double xError =
        showBinWidth ? 0.5 * (points[i].ptHigh - points[i].ptLow) : 0.0;
    graph->SetPoint(i, x, points[i].value);
    graph->SetPointError(i, xError, points[i].stat);
  }
  graph->SetMarkerColor(color);
  graph->SetLineColor(color);
  graph->SetMarkerStyle(markerStyle);
  graph->SetMarkerSize(markerSize);
  graph->SetLineWidth(2);
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
      std::cerr << Form(
                       "[WARN] no legacy 2D-fit match for ratio: pT %.1f-%.1f",
                       num.ptLow, num.ptHigh)
                << std::endl;
      continue;
    }
    if (std::abs(den->value) < 1e-12)
    {
      std::cerr << Form(
                       "[WARN] zero legacy 2D-fit value for ratio: pT %.1f-%.1f",
                       num.ptLow, num.ptHigh)
                << std::endl;
      continue;
    }

    const double value = num.value / den->value;
    const double error =
        std::hypot(num.stat / den->value,
                   num.value * den->stat / (den->value * den->value));
    ratio.push_back({num.ptLow, num.ptHigh, value, error, -1.0, -1, -1,
                     "subrange/legacy_2D"});
  }
  return ratio;
}

void writeCsv(const std::vector<Point> &current,
              const std::vector<Point> &legacy,
              const std::vector<Point> &hin)
{
  std::ofstream out(
      outputPath("bFraction_PbPb_fwd_y1p8_comparison_values.csv").Data());
  out << "dataset,energy_TeV,rapidity,centrality,method,pt_low,pt_high,"
         "value,stat,fit_pvalue,fit_status,cov_qual,source\n";
  out << std::setprecision(9);

  const auto writeRows = [&out](const std::vector<Point> &points,
                                const char *dataset,
                                const char *centrality,
                                const char *method) {
    for (const auto &point : points)
    {
      out << dataset << ",5.02,1.8<|y|<2.4," << centrality << ',' << method
          << ',' << point.ptLow << ',' << point.ptHigh << ',' << point.value
          << ',' << point.stat << ',';
      if (point.fitPvalue >= 0.0)
        out << point.fitPvalue;
      out << ',';
      if (point.fitStatus >= 0)
        out << point.fitStatus;
      out << ',';
      if (point.covQual >= 0)
        out << point.covQual;
      out << ',' << point.source << '\n';
    }
  };

  writeRows(current, "current_subrange", "0-90%",
            "mass-subrange signal yields + ctau fit");
  writeRows(legacy, "legacy_2D_fit", "0-90%", "mass-ctau 2D fit");
  writeRows(hin, "HIN-16-025", "published selection",
            "published CMS reference (local legacy macro values)");
}

void ratioRange(const std::vector<Point> &ratio, double &minimum, double &maximum)
{
  minimum = 0.75;
  maximum = 1.25;
  if (ratio.empty())
    return;

  double low = std::numeric_limits<double>::max();
  double high = std::numeric_limits<double>::lowest();
  for (const auto &point : ratio)
  {
    low = std::min(low, point.value - point.stat);
    high = std::max(high, point.value + point.stat);
  }
  const double padding = std::max(0.08, 0.18 * (high - low));
  minimum = std::max(0.0, low - padding);
  maximum = high + padding;
  minimum = std::min(minimum, 0.94);
  maximum = std::max(maximum, 1.06);
}
} // namespace

void bFraction_fwd_y1p8_PbPb_comparison()
{
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(5);

  const double ptEdges[][2] = {
      {3.0, 4.5}, {4.5, 5.5}, {5.5, 6.5}, {6.5, 7.5}, {7.5, 8.5},
      {8.5, 9.5}, {9.5, 11.0}, {11.0, 15.0}, {15.0, 20.0}, {20.0, 40.0}};

  std::vector<Point> current;
  std::vector<Point> legacy;
  for (const auto &edge : ptEdges)
  {
    Point point{};
    if (readSubrangePoint(edge[0], edge[1], point))
      current.push_back(point);
    else
      std::cerr << Form("[WARN] current PbPb subrange result missing: "
                        "pT %.1f-%.1f",
                        edge[0], edge[1])
                << std::endl;

    if (readLegacy2DFitPoint(edge[0], edge[1], point))
      legacy.push_back(point);
    else
      std::cerr << Form("[WARN] legacy PbPb 2D-fit result missing: "
                        "pT %.1f-%.1f",
                        edge[0], edge[1])
                << std::endl;
  }

  // HIN-16-025 PbPb reference used by the local legacy forward-rapidity
  // plotting macro:
  // /data/hwan/psi2S_RAA_PbPb2018/Macros/Jpsi_260601/
  // draw_Bfraction_pbpb_y1p8_2p4.C
  const double hinLow[] = {3.0, 4.5, 5.5, 6.5, 7.5,
                           8.5, 9.5, 11.0, 15.0};
  const double hinHigh[] = {4.5, 5.5, 6.5, 7.5, 8.5,
                            9.5, 11.0, 15.0, 50.0};
  const double hinValue[] = {0.145, 0.164, 0.172, 0.198, 0.227,
                             0.250, 0.289, 0.344, 0.470};
  const double hinError[] = {0.003, 0.003, 0.003, 0.004, 0.005,
                             0.006, 0.006, 0.006, 0.010};
  std::vector<Point> hin;
  for (size_t i = 0; i < sizeof(hinValue) / sizeof(hinValue[0]); ++i)
    hin.push_back(
        {hinLow[i], hinHigh[i], hinValue[i], hinError[i], -1.0, -1, -1,
         "Jpsi_260601/draw_Bfraction_pbpb_y1p8_2p4.C"});

  const std::vector<Point> ratio = makeRatioPoints(current, legacy);
  writeCsv(current, legacy, hin);

  auto *gCurrent = makeGraph(current, -0.12, kMagenta + 2, 29, 1.8);
  auto *gLegacy = makeGraph(legacy, 0.12, kAzure + 2, 20, 1.15);
  auto *gHin = makeGraph(hin, 0.0, kGray + 1, 25, 1.05, true);
  auto *gRatio = makeGraph(ratio, 0.0, kMagenta + 2, 20, 1.05);

  TCanvas canvas("cPbPbBfracComparison", "cPbPbBfracComparison", 900, 900);
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
  frame.SetMaximum(0.8);
  frame.GetYaxis()->SetTitle("Nonprompt J/#psi fraction");
  frame.GetYaxis()->CenterTitle();
  frame.GetXaxis()->SetLabelSize(0.0);
  frame.GetXaxis()->SetTitleSize(0.0);
  frame.GetYaxis()->SetTitleSize(0.060);
  frame.GetYaxis()->SetLabelSize(0.052);
  frame.GetYaxis()->SetTitleOffset(1.02);
  frame.Draw("AXIS");

  gHin->Draw("P SAME");
  gLegacy->Draw("P SAME");
  gCurrent->Draw("P SAME");

  TLegend legend(0.42, 0.68, 0.94, 0.87);
  legend.SetBorderSize(0);
  legend.SetFillStyle(0);
  legend.SetTextFont(42);
  legend.SetTextSize(0.028);
  legend.AddEntry(gCurrent, "Current subrange (stat.)", "lp");
  legend.AddEntry(gLegacy, "Legacy 2D fit (stat.)", "lp");
  legend.AddEntry(gHin, "HIN-16-025 reference", "lp");
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
  text.DrawLatex(0.96, 0.93, "PbPb, #sqrt{s_{NN}} = 5.02 TeV");
  text.SetTextAlign(11);
  text.SetTextSize(0.028);
  text.DrawLatex(0.17, 0.84, "1.8 < |y| < 2.4, Cent. 0#font[122]{-}90%");
  topPad.RedrawAxis();

  ratioPad.cd();
  double ratioMinimum = 0.0;
  double ratioMaximum = 0.0;
  ratioRange(ratio, ratioMinimum, ratioMaximum);
  TH1D ratioFrame("ratioFrame", "", 50, 0.0, 50.0);
  ratioFrame.SetMinimum(ratioMinimum);
  ratioFrame.SetMaximum(ratioMaximum);
  ratioFrame.GetXaxis()->SetTitle("p_{T}^{J/#psi} (GeV/c)");
  ratioFrame.GetYaxis()->SetTitle("Subrange / 2D fit");
  ratioFrame.GetXaxis()->CenterTitle();
  ratioFrame.GetYaxis()->CenterTitle();
  ratioFrame.GetXaxis()->SetTitleSize(0.13);
  ratioFrame.GetXaxis()->SetLabelSize(0.11);
  ratioFrame.GetXaxis()->SetTitleOffset(1.05);
  ratioFrame.GetYaxis()->SetTitleSize(0.10);
  ratioFrame.GetYaxis()->SetLabelSize(0.10);
  ratioFrame.GetYaxis()->SetTitleOffset(0.62);
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
  canvas.SaveAs(outputPath("bFraction_PbPb_fwd_y1p8_comparison.pdf"));
  canvas.SaveAs(outputPath("bFraction_PbPb_fwd_y1p8_comparison.png"));

  std::cout << "[INFO] current PbPb subrange points: " << current.size()
            << std::endl;
  std::cout << "[INFO] legacy PbPb 2D-fit points: " << legacy.size()
            << std::endl;
  std::cout << "[INFO] subrange / legacy 2D-fit ratio points: "
            << ratio.size() << std::endl;
  for (const auto &point : current)
    std::cout << Form("[INFO] current pT %.1f-%.1f: %.8f +/- %.8f, "
                      "status=%d, covQual=%d, p=%.8g",
                      point.ptLow, point.ptHigh, point.value, point.stat,
                      point.fitStatus, point.covQual, point.fitPvalue)
              << std::endl;
  for (const auto &point : ratio)
    std::cout << Form("[INFO] ratio pT %.1f-%.1f: %.8f +/- %.8f",
                      point.ptLow, point.ptHigh, point.value, point.stat)
              << std::endl;
}

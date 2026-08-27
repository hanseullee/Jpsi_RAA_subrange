#include <TCanvas.h>
#include <TColor.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TStyle.h>

void draw_b_fraction_vs_pt()
{
  gROOT->Macro("~/rootlogon.C");
  constexpr int nDataBins = 6;

  // pT bin boundaries and the corresponding b-fraction values.
  // The first bin (0--6.5 GeV/c) is empty so that the x-axis starts at zero.
  const double histEdges[nDataBins + 2] = {0.0, 6.5, 8.5, 9.5, 11.0,
                                            15.0, 20.0, 40.0};
  const double bFraction[nDataBins] =        {0.2346, 0.2664, 0.3075, 0.3726, 0.4764, 0.5573};
  const double bFractionHanseul[nDataBins] = {0.252, 0.29, 0.34, 0.39, 0.49, 0.5787};
  const double bFraction16025[nDataBins] =   {0.239, 0.275, 0.307, 0.377, 0.503, 0.5958};
  double bfracErr[nDataBins] = {0.0005, 0.0005, 0.0005,
                                0.0005, 0.0005, 0.0005};

  gStyle->SetOptStat(0);

  auto canvas = new TCanvas("cBFraction", "b-fraction vs pT", 800, 700);
  canvas->SetLeftMargin(0.14);
  canvas->SetRightMargin(0.04);
  canvas->SetBottomMargin(0.13);
  canvas->SetTopMargin(0.08);
  canvas->SetTicks(1, 1);

  auto hist = new TH1D("hBFractionVsPt", "", nDataBins + 1, histEdges);
  auto histHanseul = new TH1D("hBFractionHanseulVsPt", "",
                              nDataBins + 1, histEdges);
  auto hist16025 = new TH1D("hBFraction16025VsPt", "",
                            nDataBins + 1, histEdges);
  for (int i = 0; i < nDataBins; ++i) {
    hist->SetBinContent(i + 2, bFraction[i]);
    hist->SetBinError(i + 2, bfracErr[i]);
    histHanseul->SetBinContent(i + 2, bFractionHanseul[i]);
    histHanseul->SetBinError(i + 2, bfracErr[i]);
    hist16025->SetBinContent(i + 2, bFraction16025[i]);
    hist16025->SetBinError(i + 2, bfracErr[i]);
  }

  const int pastelBlue = TColor::GetColor("#6C9BD2");
  const int pastelCoral = TColor::GetColor("#E89086");
  const int pastelGreen = TColor::GetColor("#78BFA3");

  hist->SetLineColor(pastelBlue);
  hist->SetLineWidth(3);
  hist->SetMarkerColor(pastelBlue);
  hist->SetMarkerStyle(20);
  hist->SetMarkerSize(1.8);

  histHanseul->SetLineColor(pastelCoral);
  histHanseul->SetLineWidth(3);
  histHanseul->SetMarkerColor(pastelCoral);
  histHanseul->SetMarkerStyle(21);
  histHanseul->SetMarkerSize(1.8);

  hist16025->SetLineColor(pastelGreen);
  hist16025->SetLineWidth(3);
  hist16025->SetMarkerColor(pastelGreen);
  hist16025->SetMarkerStyle(22);
  hist16025->SetMarkerSize(1.8);

  hist->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hist->GetYaxis()->SetTitle("b-fraction");
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->CenterTitle();
  hist->GetXaxis()->SetRangeUser(0.0, 40.0);
  hist->GetYaxis()->SetRangeUser(0.0, 1.0);
  hist->GetXaxis()->SetTitleSize(0.05);
  hist->GetYaxis()->SetTitleSize(0.05);
  hist->GetXaxis()->SetLabelSize(0.043);
  hist->GetYaxis()->SetLabelSize(0.043);
  hist->GetXaxis()->SetTitleOffset(1.1);
  hist->GetYaxis()->SetTitleOffset(1.25);

  hist->Draw("E");
  histHanseul->Draw("E SAME");
  hist16025->Draw("E SAME");

  auto legend = new TLegend(0.68, 0.23, 0.91, 0.43);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.038);
  legend->AddEntry(hist, "Subrange", "lp");
  legend->AddEntry(histHanseul, "Old 2d fit", "lp");
  legend->AddEntry(hist16025, "16-025", "lp");
  legend->Draw();

  TLatex cmsLabel;
  cmsLabel.SetNDC();
  cmsLabel.SetTextFont(62);
  cmsLabel.SetTextSize(0.038);
  cmsLabel.SetTextAlign(11);
  cmsLabel.DrawLatex(0.14, 0.94, "CMS");

  TLatex preliminaryLabel;
  preliminaryLabel.SetNDC();
  preliminaryLabel.SetTextFont(42);
  preliminaryLabel.SetTextSize(0.038);
  preliminaryLabel.SetTextAlign(11);
  preliminaryLabel.DrawLatex(0.215, 0.94, "Preliminary");

  TLatex lumiLabel;
  lumiLabel.SetNDC();
  lumiLabel.SetTextFont(42);
  lumiLabel.SetTextSize(0.038);
  lumiLabel.SetTextAlign(31);
  lumiLabel.DrawLatex(0.96, 0.94,
                      "pp #sqrt{s} = 5.02 TeV (28 pb^{-1})");

  TLatex label;
  label.SetNDC();
  label.SetTextFont(42);
  label.SetTextSize(0.040);
  label.DrawLatex(0.18, 0.86, "|y| < 0.6");

  canvas->SaveAs("b_fraction_vs_pt.png");
  canvas->SaveAs("b_fraction_vs_pt.pdf");
}

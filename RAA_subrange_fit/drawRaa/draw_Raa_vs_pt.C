#include <TCanvas.h>
#include <TColor.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TStyle.h>

void draw_Raa_vs_pt()
{
  constexpr int nDataBins = 6;

  // pT bin boundaries and the corresponding R_AA values.
  // The first bin (0--6.5 GeV/c) is empty so that the x-axis starts at zero.
  const double histEdges[nDataBins + 2] = {0.0, 6.5, 8.5, 9.5, 11.0,
                                            15.0, 20.0, 40.0};
  const double raaSubrange[nDataBins] = {0.402, 0.274, 0.251,
                                         0.283, 0.331, 0.396};
  const double raaOld2dFit[nDataBins] = {0.394, 0.283, 0.281,
                                         0.288, 0.323, 0.375};
  const double raa16025[nDataBins] = {0.335, 0.347, 0.330,
                                      0.350, 0.383, 0.383};
  const double raaSubrangeErr[nDataBins] = {0.0159, 0.0079, 0.0054,
                                            0.0058, 0.0078, 0.0114};
  const double raaOld2dFitErr[nDataBins] = {0.0147, 0.0079, 0.0060,
                                            0.0044, 0.0069, 0.0102};
  const double raa16025Err[nDataBins] = {0.0159, 0.0079, 0.0054,
                                         0.0058, 0.0078, 0.0114};

  gStyle->SetOptStat(0);

  auto canvas = new TCanvas("cRaa", "R_{AA} vs pT", 800, 700);
  canvas->SetLeftMargin(0.14);
  canvas->SetRightMargin(0.04);
  canvas->SetBottomMargin(0.13);
  canvas->SetTopMargin(0.08);
  canvas->SetTicks(1, 1);

  auto hist = new TH1D("hRaaSubrangeVsPt", "", nDataBins + 1, histEdges);
  auto histOld2dFit = new TH1D("hRaaOld2dFitVsPt", "",
                               nDataBins + 1, histEdges);
  auto hist16025 = new TH1D("hRaa16025VsPt", "",
                            nDataBins + 1, histEdges);
  for (int i = 0; i < nDataBins; ++i) {
    hist->SetBinContent(i + 2, raaSubrange[i]);
    hist->SetBinError(i + 2, raaSubrangeErr[i]);
    histOld2dFit->SetBinContent(i + 2, raaOld2dFit[i]);
    histOld2dFit->SetBinError(i + 2, raaOld2dFitErr[i]);
    hist16025->SetBinContent(i + 2, raa16025[i]);
    hist16025->SetBinError(i + 2, raa16025Err[i]);
  }

  const int pastelBlue = TColor::GetColor("#6C9BD2");
  const int pastelCoral = TColor::GetColor("#E89086");
  const int pastelGreen = TColor::GetColor("#78BFA3");

  hist->SetLineColor(pastelBlue);
  hist->SetLineWidth(3);
  hist->SetMarkerColor(pastelBlue);
  hist->SetMarkerStyle(20);
  hist->SetMarkerSize(1.8);

  histOld2dFit->SetLineColor(pastelCoral);
  histOld2dFit->SetLineWidth(3);
  histOld2dFit->SetMarkerColor(pastelCoral);
  histOld2dFit->SetMarkerStyle(21);
  histOld2dFit->SetMarkerSize(1.8);

  hist16025->SetLineColor(pastelGreen);
  hist16025->SetLineWidth(3);
  hist16025->SetMarkerColor(pastelGreen);
  hist16025->SetMarkerStyle(22);
  hist16025->SetMarkerSize(1.8);

  hist->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hist->GetYaxis()->SetTitle("R_{AA}");
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
  histOld2dFit->Draw("E SAME");
  hist16025->Draw("E SAME");

  auto legend = new TLegend(0.68, 0.23, 0.91, 0.43);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.038);
  legend->AddEntry(hist, "Subrange", "lp");
  legend->AddEntry(histOld2dFit, "Old 2d fit", "lp");
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
                      "pp, Pb #sqrt{s_{NN}} = 5.02 TeV (28 pb^{-1}, 1.6^{-1})");

  TLatex label;
  label.SetNDC();
  label.SetTextFont(42);
  label.SetTextSize(0.040);
  label.DrawLatex(0.18, 0.86, "Cent.0-90\%");
  label.DrawLatex(0.18, 0.80, "|y| < 0.6");

  canvas->SaveAs("Raa_vs_pt.png");
  canvas->SaveAs("Raa_vs_pt.pdf");
}

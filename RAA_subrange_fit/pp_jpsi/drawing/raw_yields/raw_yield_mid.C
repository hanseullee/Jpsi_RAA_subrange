#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TBox.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TSystem.h"

namespace
{
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
} // namespace

void raw_yield_mid()
{
  TStopwatch __execTimer;
  __execTimer.Start();
  struct __ExecTimerGuard {
    TStopwatch *sw;
    explicit __ExecTimerGuard(TStopwatch *timer) : sw(timer) {}
    ~__ExecTimerGuard() {
      sw->Stop();
      sw->Print();
    }
  } __execTimerGuard(&__execTimer);

  // =====  CMS plot style =====
  gROOT->Macro("/data/users/pjgwak/input_files/rootlogon.C");

  // ===== pp Run2 5.02 TeV reference frame (|y| < 1.6) =====
  const int n_old = 6;

  double pt_low_old[n_old] = {6.5, 9.0, 12.0, 15.0, 20.0, 25.0};
  double pt_high_old[n_old] = {9.0, 12.0, 15.0, 20.0, 25.0, 40.0};

  double val_pp_old[n_old] = {962074.12577, 798947.28002, 325527.64394, 183250.41632, 54258.46031, 31201.56385};
  double stat_pp_old[n_old] = {2056.63047, 1262.20495, 879.84424, 711.94967, 286.03568, 167.41590};

  double sys_pp_old[n_old];
  for (int i = 0; i < n_old; ++i)
    sys_pp_old[i] = 0.0; // sys=0

  double x_old[n_old], ex_old[n_old];
  for (int i = 0; i < n_old; ++i)
  {
    const double width = pt_high_old[i] - pt_low_old[i];
    x_old[i] = 0.5 * (pt_low_old[i] + pt_high_old[i]);
    ex_old[i] = 0.5 * width;
    val_pp_old[i] /= width;
    stat_pp_old[i] /= width;
    sys_pp_old[i] /= width;
  }

    // ===== pp Run2 5.02 TeV |y|<1.6 (subrange 2D fit) =====
  // y = 0.00-1.60
  const int NooY16 = 6;
  double pt_low_oo_y16[NooY16] = {6.5, 9.0, 12.0, 15.0, 20.0, 25.0};
  double pt_high_oo_y16[NooY16] = {9.0, 12.0, 15.0, 20.0, 25.0, 40.0};

  double val_oo_y16[NooY16] = {962074.12577, 798947.28002, 325527.64394, 183250.41632, 54258.46031, 31201.56385};
  double stat_oo_y16[NooY16] = {2056.63047, 1262.20495, 879.84424, 711.94967, 286.03568, 167.41590};

  double sys_oo_y16[NooY16];
  for (int i = 0; i < NooY16; ++i)
    sys_oo_y16[i] = 0.0; // sys=0

  double x_oo_y16[NooY16], ex_oo_y16[NooY16];
  for (int i = 0; i < NooY16; ++i)
  {
    const double width = pt_high_oo_y16[i] - pt_low_oo_y16[i];
    x_oo_y16[i] = 0.5 * (pt_low_oo_y16[i] + pt_high_oo_y16[i]);
    ex_oo_y16[i] = 0.5 * width;
    val_oo_y16[i] /= width;
    stat_oo_y16[i] /= width;
    sys_oo_y16[i] /= width;
  }

    // ===== adjust yMin and yMax =====
  double yMin = 1.0e30;
  double yMax = 0.0;
  for (int i = 0; i < n_old; ++i)
  {
    const double yLow = val_pp_old[i] - stat_pp_old[i];
    const double yHigh = val_pp_old[i] + stat_pp_old[i];
    if (yLow > 0.0 && yLow < yMin)
      yMin = yLow;
    if (yHigh > yMax)
      yMax = yHigh;
  }
  for (int i = 0; i < NooY16; ++i)
  {
    const double yLow = val_oo_y16[i] - stat_oo_y16[i];
    const double yHigh = val_oo_y16[i] + stat_oo_y16[i];
    if (yLow > 0.0 && yLow < yMin)
      yMin = yLow;
    if (yHigh > yMax)
      yMax = yHigh;
  }

  // ===== Graphs =====
  // pp_old: sys band
  TGraphAsymmErrors *gSyspp_old = new TGraphAsymmErrors(n_old, x_old, val_pp_old, ex_old, ex_old, sys_pp_old, sys_pp_old);
  gSyspp_old->SetFillStyle(0);
  gSyspp_old->SetLineColor(kRed + 1);
  gSyspp_old->SetLineWidth(2);

  // pp_old: stat
  TGraphErrors *gStatpp_old = new TGraphErrors(n_old, x_old, val_pp_old, ex_old, stat_pp_old);
  gStatpp_old->SetMarkerStyle(24);
  gStatpp_old->SetMarkerSize(1.7);
  gStatpp_old->SetMarkerColor(kRed + 1);
  gStatpp_old->SetLineColor(kRed + 1);
  gStatpp_old->SetLineWidth(3);

  // OO |y|<1.6: sys (currently 0)
  TGraphAsymmErrors *gSysOOY16 = new TGraphAsymmErrors(NooY16, x_oo_y16, val_oo_y16, ex_oo_y16, ex_oo_y16, sys_oo_y16, sys_oo_y16);
  gSysOOY16->SetFillColorAlpha(kAzure + 2, 0.20);
  gSysOOY16->SetFillStyle(1001);
  gSysOOY16->SetLineColor(kAzure + 2);
  gSysOOY16->SetLineWidth(3);

  // OO |y|<1.6: stat
  TGraphErrors *gStatOOY16 = new TGraphErrors(NooY16, x_oo_y16, val_oo_y16, ex_oo_y16, stat_oo_y16);
  gStatOOY16->SetMarkerStyle(20);
  gStatOOY16->SetMarkerSize(1.7);
  gStatOOY16->SetMarkerColor(kAzure + 2);
  gStatOOY16->SetLineColor(kAzure + 2);
  gStatOOY16->SetLineWidth(3);

  // ===== Draw =====
  TCanvas *c1 = new TCanvas("c1", "pp vs pp_old", 800, 800);
  c1->SetTitle("");
  TPad pad1("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1.SetTopMargin(0.08);
  pad1.SetBottomMargin(0.13);
  pad1.SetLogy();
  pad1.Draw();
  pad1.cd();
  gStyle->SetOptStat(0);
  gSyspp_old->SetTitle("");
  gSyspp_old->Draw("A2");
  gSyspp_old->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  gSyspp_old->GetYaxis()->SetTitle("dN/dp_{T}");
  gSyspp_old->GetXaxis()->CenterTitle();
  gSyspp_old->GetYaxis()->CenterTitle();
  gSyspp_old->GetXaxis()->SetTitleSize(0.05);
  gSyspp_old->GetYaxis()->SetTitleSize(0.05);
  gSyspp_old->GetXaxis()->SetLabelSize(0.042);
  gSyspp_old->GetYaxis()->SetLabelSize(0.042);
  gSyspp_old->GetYaxis()->SetTitleOffset(1.25);
  gSyspp_old->GetXaxis()->SetLimits(pt_low_old[0], pt_high_oo_y16[NooY16 - 1]);
  gSyspp_old->SetMinimum(0.8 * yMin);
  gSyspp_old->SetMaximum(4 * yMax);

  gSyspp_old->Draw("A2");
  gSysOOY16->Draw("2 SAME");
  gStatOOY16->Draw("P SAME");

  // ------------------------------------------------------------------
  // labels (match ctau_pr.C style/positions)
  // ------------------------------------------------------------------
  {
    TLatex tx;
    tx.SetNDC();
    tx.SetTextSize(0.032);
    tx.SetTextFont(42);
    tx.SetTextAlign(31);
    tx.DrawLatex(0.96, 0.935, "pp #sqrt{s} = 5.02 TeV (28.0 pb^{-1})");
  }
  {
    TLatex cms;
    cms.SetNDC();
    cms.SetTextAlign(11);
    cms.SetTextSize(0.040);
    cms.SetTextFont(62);
    cms.DrawLatex(0.21, 0.935, "CMS");

    TLatex extra;
    extra.SetNDC();
    extra.SetTextAlign(11);
    extra.SetTextSize(0.040);
    extra.SetTextFont(72);
    extra.DrawLatex(0.305, 0.935, "Internal");
  }
  {
    TLatex tx;
    tx.SetNDC();
    tx.SetTextSize(0.03);
    tx.SetTextFont(42);
    // tx.SetTextColor(kRed + 1);
    tx.DrawLatex(0.19, 0.865, "|y| < 1.6");
  }

  TLegend *leg = new TLegend(0.7, 0.8, 0.90, 0.90);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.024);
  leg->SetEntrySeparation(0.01);
  leg->AddEntry(gStatOOY16, "pp (c#tau signal fit)", "lp");
  leg->Draw();

  c1->SaveAs(outputPath("raw_yield_ppRun2_mid.pdf"));
}
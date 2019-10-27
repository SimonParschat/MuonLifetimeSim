#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TString.h"
#include "TLatex.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"

{
    TCanvas *c1 = new TCanvas;
    TF1  *f1 = new TF1("func","[0]*exp(-[1]*x)",0,20000);
    TF1  *f2 = new TF1("expected","[0]*exp(-[1]*x)",0,20000);

    f1->SetParName(0,"N0:");
    f1->SetParLimits(0,0,1e6);
    f1->SetParName(1,"Gamma:");
    f1->SetParLimits(1,-1,1);

    TFile* file = TFile::Open("/Users/Simon/Desktop/Simulations/muonSDSimNew/build/muonSDSim_output.root");

    MuonLifetime->SetLineColor(2);
    MuonLifetime->SetLineStyle(2);
    MuonLifetime->SetLineColorAlpha(2, 0.2);
    MuonLifetime->SetLineWidth(1);
    f1->SetLineColor(1);
    f2->SetLineColor(4);
    MuonLifetime->SetFillColor(2);
    f1->SetLineWidth(3);
    // f1->SetFillColor(1);
    f2->SetLineWidth(3);
    // f2->SetFillColor(4);
    MuonLifetime->SetXTitle("Pulse time difference (ns)");
    MuonLifetime->SetYTitle("Counts");
    MuonLifetime->SetLabelSize(0.03,"XY");
    gStyle->SetTitleFontSize(0.06);

    gStyle->SetOptFit();
    MuonLifetime->Fit("func");
    MuonLifetime->Draw("E hist same");
    gPad->Update();

    TPaveStats *tps1 = (TPaveStats*) MuonLifetime->FindObject("stats");
    tps1->SetX1NDC(0.64);
    tps1->SetX2NDC(0.91);
    tps1->SetY1NDC(0.91);
    tps1->SetY2NDC(0.7);
    tps1->SetName("delta time Stats");
    tps1->SetTextColor(1);
    tps1->SetTextSize(0.026);
    c1->SetTitle("time delta b.t. mu- & e- Histogram");
    tps1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(13);
    Tl.SetTextSize(0.03);
    Tl.SetTextColor(1);
    Tl.DrawLatex(13000,470,TString::Format("Simulated #tau = %g (ns)", 1 / f1->GetParameter(1)));
    
    TLatex Tl2;
    Tl2.SetTextAlign(13);
    Tl2.SetTextSize(0.03);
    Tl2.SetTextColor(4);
    Tl2.DrawLatex(13000,420,TString::Format("Experimental #tau = %g (ns)", 2196.9));


    f2->SetParameter(0,f1->GetParameter(0));
    f2->SetParameter(1,0.000455187);
    

    // Fitted Halflife
    // Line Along Y for func 1 showing halflife
    TLine *line11 = new TLine(0,f1->GetParameter(0)/2,f1->GetX(f1->GetParameter(0)/2),f1->GetParameter(0)/2);
    line11->SetLineColor(1);
    line11->SetLineStyle(2);
    line11->SetLineWidth(3);

    // Line Along X for func 1 showing halflife
    // TLine *line = new TLine(xmin,ymin,xmax,ymax);
    TLine *line21 = new TLine(f1->GetX(f1->GetParameter(0)/2),0,f1->GetX(f1->GetParameter(0)/2),f1->GetParameter(0)/2);
    line21->SetLineColor(1);
    line21->SetLineStyle(2);
    line21->SetLineWidth(3);

    // Real Halflife
    // Line Along Y for func 1 showing halflife
    TLine *line12 = new TLine(0,f2->GetParameter(0)/2,f2->GetX(f2->GetParameter(0)/2),f2->GetParameter(0)/2);
    line12->SetLineColor(4);
    line12->SetLineStyle(2);
    line12->SetLineWidth(3);

    // Line Along X for func 1 showing halflife
    // TLine *line = new TLine(xmin,ymin,xmax,ymax);
    TLine *line22 = new TLine(f2->GetX(f2->GetParameter(0)/2),0,f2->GetX(f2->GetParameter(0)/2),f2->GetParameter(0)/2);
    line22->SetLineColor(4);
    line22->SetLineStyle(2);
    line22->SetLineWidth(3);

    // Halflife Label Fitted
    TLatex Tline1;
    Tline1.SetTextAlign(13);
    Tline1.SetTextSize(0.026);
    Tline1.SetTextColor(4);
    Tline1.DrawLatex(1000,-30,TString::Format("Experimental Halflife = %g (ns)",f1->GetX(f1->GetParameter(0)/2)));

    // Halflife Label real 
    TLatex Tline2;
    Tline2.SetTextAlign(13);
    Tline2.SetTextSize(0.026);
    Tline2.SetTextColor(1);
    Tline2.DrawLatex(1000,-50,TString::Format("Fitted Halflife = %g (ns)",f2->GetX(f2->GetParameter(0)/2)));

    f2->Draw("same");
    line11->Draw();
    line21->Draw();
    line12->Draw();
    line22->Draw();
    gPad->Update();
}
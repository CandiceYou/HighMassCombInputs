#include "TF1.h"
#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "RooTFnBinding.h"
#include "fitFunction.c"
#include <fstream>
#include <iostream>
using namespace std;


TGraphErrors* makegr(int n=1,double x[]={0},double y[]={0},double ex[]={0},double ey[]={0},int Marker=1, int lineColor=1,char* title="doubleCB param",char* xtitle="mass",char* ytitle="mean",double init_param[]={0}){
 gStyle->SetPadLeftMargin(0.15);
 gStyle->SetPadRightMargin(0.15);
// gStyle->SetOptFit();
 gStyle->SetOptFit(0000);
 gStyle->SetOptStat(1111);
 gStyle->SetTitleFontSize(0.05);
// gStyle->SetTitleOffset(2, "XYZ");

  const int npars=7;
  TF1* fit = new TF1("fit", fitFunction,0,3500, npars);
  fit->SetParameters(init_param);
  for (int ip=0; ip<2; ip++) fit->FixParameter(ip, fit->GetParameter(ip));
//  fit->FixParameter(8, fit->GetParameter(8));

//  if (ytitle=="mean")
//  fit->FixParameter(8, fit->GetParameter(8));


  TGraphErrors *gr = new TGraphErrors (n,x,y,ex,ey);
  gr->SetMarkerColor(lineColor);
  gr->SetMarkerStyle(Marker);
  gr->SetTitle(title);
  gr->GetXaxis()->SetTitle(xtitle);
  gr->GetYaxis()->SetTitle(ytitle);
  fit->SetLineColor(lineColor);  
  gr->Fit("fit");
  TCanvas* c = new TCanvas("c", "c", 1000, 10, 800, 800);
  c->cd();
  gr->SetMarkerStyle(4);
  fit->SetLineColor(lineColor);
  gr->Draw("AP");
//  fit->Draw("same");
  c->Update();
  char pn[50];
  sprintf(pn,"params/indiFit_param_%s_vs_%s_<channel>.pdf",ytitle,xtitle);
//  sprintf(pn,"param_%s_vs_%s_4mu.png",ytitle,xtitle);
  c->SaveAs(pn);

  ofstream f;
  f.open ("params/individual_fit_param.txt",std::ofstream::out | std::ofstream::app);
   for (int i=0;i<fit->GetNpar();i++) {
   Double_t value = fit->GetParameter(i);
   char par[200];
   sprintf(par,"<channel>_%s_param_%d: %E",ytitle,i,value);
   f<<par<<endl;
   }
   f<<endl;
  f.close();

  return gr;

}


void plotVar_fitInd_<channel>(){
//void plotVar_bind_4mu(){

const Int_t n_MH=<nArray>;
Double_t MH[n_MH]={<massArray>};
Double_t mean[n_MH]={<meanArray>};
Double_t sigma[n_MH]={<sigmaArray>};
Double_t a1[n_MH]={<a1Array>};
Double_t a2[n_MH]={<a2Array>};
Double_t n1[n_MH]={<n1Array>};
Double_t n2[n_MH]={<n2Array>};

Double_t mean_err[n_MH]={<meanErrArray>};
Double_t sigma_err[n_MH]={<sigmaErrArray>};
Double_t a1_err[n_MH]={<a1ErrArray>};
Double_t a2_err[n_MH]={<a2ErrArray>};
Double_t n1_err[n_MH]={<n1ErrArray>};
Double_t n2_err[n_MH]={<n2ErrArray>};
Double_t mass_err[n_MH]={0};

//Double_t MH[n_MH]={0};
//for (int i=0;i<n_MH;i++){
//MH[i]=MH_real[i]-600;
//cout<<"MassDiff["<<i<<"]="<<MassDiff[i]<<endl;
//}


  const int npars=7;

  double a1_initParam[npars]={1000,2000,0,0,0,0,0};
  double a2_initParam[npars]={1000,2000,0,0,0,0,0};
  double n1_initParam[npars]={1000,2000,0,0,0,0,0};
  double n2_initParam[npars]={1000,2000,0,0,0,0,0};
  double mean_initParam[npars]={1000,2000,0,0,0,0,0};
  double sigma_initParam[npars]={1000,2000,0,0,0,0,0};

TGraph* gr1 = makegr(n_MH,MH,mean,mass_err,mean_err,20,2,"doubleCB param","mass","mean",mean_initParam);
TGraph* gr2 = makegr(n_MH,MH,sigma,mass_err,sigma_err,21,3,"doubleCB param","mass","sigma",sigma_initParam);
TGraph* gr3 = makegr(n_MH,MH,a1,mass_err,a1_err,22,4,"doubleCB param","mass","a1",a1_initParam);
TGraph* gr4 = makegr(n_MH,MH,a2,mass_err,a2_err,23,6,"doubleCB param","mass","a2",a2_initParam);
TGraph* gr5 = makegr(n_MH,MH,n1,mass_err,n1_err,24,7,"doubleCB param","mass","n1",n1_initParam);
TGraph* gr6 = makegr(n_MH,MH,n2,mass_err,n2_err,25,94,"doubleCB param","mass","n2",n2_initParam);

 TCanvas* c2 = new TCanvas("c2", "c2", 1000, 10, 800, 800);
  TMultiGraph *g = new TMultiGraph();
  g->Add(gr1);
  g->Add(gr2);
  g->Add(gr3);
  g->Add(gr4);
  g->Add(gr5);
  g->Add(gr6);
  //g->GetYaxis()->SetRangeUser(-2,15);
  TLegend* leg = new TLegend(.87,0.5,0.98,.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gr1,"mean","p");
  leg->AddEntry(gr2,"sigma","p");
  leg->AddEntry(gr3,"a1","p");
  leg->AddEntry(gr4,"a2","p");
  leg->AddEntry(gr5,"n1","p");
  leg->AddEntry(gr6,"n2","p");
  gStyle->SetOptFit(0000);  
  g->Draw("AP");
  g->GetXaxis()->SetTitle("Mass");
  g->GetYaxis()->SetTitle("DoubleCB_param");
  leg->Draw();
  c2->Update();
  c2->SaveAs("params/indiFit_param_all_<channel>.pdf");
  c2->SaveAs("params/indiFit_param_all_<channel>.png");
}

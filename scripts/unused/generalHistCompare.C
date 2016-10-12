#include "TF1.h"
#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
using namespace std;


void test(char* var1="Gen_costheta1", char* var2="helcosthetaZ1", char* title="test", int nbin=30, double min=-1, double max=1){
 gStyle->SetPadLeftMargin(0.1);
 gStyle->SetPadRightMargin(0.15);
 gStyle->SetOptFit(0000);
 gStyle->SetOptStat(0000);
 gStyle->SetTitleFontSize(0.05);

//change tree and file name
 TChain *candTree1 = new TChain("selectedTree");
 candTree1->Add("DY2j_old.root");

 TChain *candTree2 = new TChain("selectedTree");
 candTree2->Add("DY2j_new.root");

 TH1F *hist1 = new TH1F("hist1","hist1",nbin,min,max);
 TH1F *hist2 = new TH1F("hist2","hist2",nbin,min,max);

 char hist1draw[500],hist2draw[500];
 sprintf(hist1draw,"%s>>hist1",var1);
 sprintf(hist2draw,"%s>>hist2",var2);

// change cut string
 candTree1->Draw(hist1draw,"(jetType==1) * weight");
 candTree2->Draw(hist2draw,"(jetType==1) * weight");

 TCanvas* c = new TCanvas("c", "c",800,800);
 c->SetFillColor(0);
 c->cd();
 hist1->SetLineColor(kRed);
 hist2->SetLineColor(kBlack);

 //hist1->Scale(1/hist1->Integral());
 //hist2->Scale(1/hist2->Integral());

 double max1=hist1->GetMaximum()*1.2;
 double max2=hist2->GetMaximum()*1.2;
 double max_y = max1>max2?max1:max2;

 hist2->GetYaxis()->SetRangeUser(0,max_y);

 hist2->SetTitle(var2);
 hist2->Draw("hist");
 hist1->Draw("samehist");


 c->Update();
 char temp[150];
// sprintf(temp,"%s.png",var2);
 c->SaveAs("compare.png");
}

void generalHistCompare(){
test("p0plus","p0plus","p0plus",30,1, 1);
}

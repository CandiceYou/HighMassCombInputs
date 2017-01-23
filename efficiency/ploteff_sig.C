#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include <fstream>
#include <iostream>
using namespace std;

//spin : 0 spin0, 2 spin2.
//ch : 0 4mu, 1 4e, 2 2e2mu.

 TGraphErrors* makegr(int spin=0, int ch=0, Color_t color=2, int marker=20, int line=1){
 gStyle->SetPadLeftMargin(0.1);
 gStyle->SetPadRightMargin(0.3);
 gStyle->SetOptStat(0000);
 gStyle->SetTitleFontSize(0.05);

 char* mode[3] ={"ggH->0p","ggH->2bp","VBF->0p"};
 char* chan[3] ={"4mu","4e","2e2mu"};

 TFile* f;
  TString  inputDir = "/afs/cern.ch/work/c/cayou/public/80Xsamples/samples_2016mc_0726/";

  int inputfiles_ggHlow[]={115,120,124,125,/*126,130,*/135,140,145,150,155,160,165,170,/*175,*/180,190,/*200,*/210,/*230,*/250,270};
  int inputfiles_VBFlow[]={/*115,*/120,/*124,125,*/126,/*130,*/135,140,145,/*150,*/155,160,165,170,175,180,190,200,210,230,250,270};
  int inputfiles_ggH[]={300,/*350,*/400,450,500,550,600,/*700,*/750,800,900,1000,/*1500,*/2000,2500,3000};
  int inputfiles_VBF[]={300,350,400,450,500,550,/*600,*/700,/*750,*/800,900,1000,/*2000,*/2500,3000};
  int inputfiles_ggH_2bp[]={};


  int Nfiles_ggH=sizeof(inputfiles_ggH)/sizeof(*inputfiles_ggH);
  int Nfiles_VBF=sizeof(inputfiles_VBF)/sizeof(*inputfiles_VBF);
  int Nfiles_ggHlow=sizeof(inputfiles_ggHlow)/sizeof(*inputfiles_ggHlow);
  int Nfiles_VBFlow=sizeof(inputfiles_VBFlow)/sizeof(*inputfiles_VBFlow);
  int Nfiles_ggH_2bp=sizeof(inputfiles_ggH_2bp)/sizeof(*inputfiles_ggH_2bp);


  char inputfile[100];
  vector<TString> files_ggH;
  vector<TString> files_VBF;

   for (int i=0; i<Nfiles_ggH; i++) {
     sprintf(inputfile,"ggH%d/ZZ4lAnalysis.root",inputfiles_ggH[i]);
     files_ggH.push_back(inputfile);
   }

   for (int i=0; i<Nfiles_ggHlow; i++) {
     sprintf(inputfile,"ggH%d/ZZ4lAnalysis_new.root",inputfiles_ggHlow[i]);
     files_ggH.push_back(inputfile);
   }

   for (int i=0; i<Nfiles_ggH_2bp; i++) {
     sprintf(inputfile,"ggH_2bp/gg2bp_M%d_new.root",inputfiles_ggH_2bp[i]);
     files_ggH.push_back(inputfile);
   }

   for (int i=0; i<Nfiles_VBF; i++) {
     sprintf(inputfile,"VBFH%d/ZZ4lAnalysis.root",inputfiles_VBF[i]);
     files_VBF.push_back(inputfile);
   }

   for (int i=0; i<Nfiles_VBFlow; i++) {
     sprintf(inputfile,"VBFH%d/ZZ4lAnalysis_new.root",inputfiles_VBFlow[i]);
     files_VBF.push_back(inputfile);
   }

  TChain *candTree = new TChain("ZZTree/candTree");
  TChain *candTree2 = new TChain("ZZTree/candTree");


if(spin!=3){
  for (vector<TString>::const_iterator file = files_ggH.begin(); file!=files_ggH.end(); ++file) 
  candTree->Add(inputDir+(*file));
}
else   candTree->Add("dummy.root");

if(spin==3){
  for (vector<TString>::const_iterator file = files_VBF.begin(); file!=files_VBF.end(); ++file)
  candTree2->Add(inputDir+(*file));
}
else candTree2->Add("dummy.root");

// draw raw histogram with 1GeV binning, 0GeV to 3100GeV.
 const Int_t m=4500;
 int zzflav=28561;
 TH1F *hgen = new TH1F("hgen","hgen",m,0,m);
 TH1F *hreco = new TH1F("hreco","hreco",m,0,m);

 switch (ch){
 case 0: zzflav=28561;  break; //4mu
 case 1: zzflav=14641;  break; //4e
 case 2: zzflav=20449;  break; //2e2mu
 default:  cout<<"channel unknown.";
 }

 char genCut[500],recoCut[500];
 if (spin==2){
 sprintf(genCut,"(genFinalState==%d)*(genHEPMCweight*PUWeight*reweightingweights[7])",ch);
 sprintf(recoCut,"(genFinalState==%d && Z1Flav*Z2Flav==%d && ZZsel>=100)*(genHEPMCweight*PUWeight*dataMCWeight*reweightingweights[7])",ch, zzflav);
  }
 else{
 sprintf(genCut,"(genFinalState==%d)*(genHEPMCweight*PUWeight*reweightingweights[0])",ch);
 sprintf(recoCut,"(genFinalState==%d && Z1Flav*Z2Flav==%d && ZZsel>=100)*(genHEPMCweight*PUWeight*dataMCWeight*reweightingweights[0])",ch, zzflav);
  }
 candTree->Draw("GenHMass>>hgen",genCut);
 candTree->Draw("GenHMass>>hreco",recoCut);

 char genCut_vbf[500],recoCut_vbf[500];
 sprintf(genCut_vbf,"(genFinalState==%d)*(genHEPMCweight*PUWeight)",ch);
 sprintf(recoCut_vbf,"(genFinalState==%d && Z1Flav*Z2Flav==%d && ZZsel>=100)*(genHEPMCweight*PUWeight*dataMCWeight)",ch, zzflav);

 if(spin==3){
 candTree2->Draw("GenHMass>>hgen",genCut_vbf);
 candTree2->Draw("GenHMass>>hreco",recoCut_vbf);}

// bin contents of raw histograms
 double M_raw[m]={0};
 double gen_raw[m]={0};
 double reco_raw[m]={0};

// const Int_t n=35;
// double M[n]={115,120,125,126,135,140,145,150,155,160,180,200,230,250,350,400,450,500,600,750,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600};

// const Int_t n=32;
// double M[n]={115,120,125,126,135,140,145,150,155,160,180,200,230,250,350,400,450,500,600,750,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000};

 const Int_t n=39;
 double M[n]={120,135,140,145,/*150,*/155,160,165,170,/*180,*/190,200,210,230,250,270,300,350,400,450,500,550,600,700,750,800,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600};


 double massE[n]={0};
 double gen[n]={0};
 double genE[n]={0};
 double reco[n]={0};
 double recoE[n]={0};
 double eff[n]={0};
 double effE[n]={0};

 double mass,width;
 for (int bin=1;bin<=m;bin++){
 M_raw[bin-1] = hreco->GetXaxis()->GetBinCenter(bin);
 gen_raw[bin-1] = hgen->GetBinContent(bin);
 reco_raw[bin-1] = hreco->GetBinContent(bin);}

cout<<"spin="<<spin<<",ch="<<ch<<endl;

 for (int i=0;i<n;i++){
  mass = M[i];
  if(mass<=180) width=1;
  else if (mass>180 && mass<=300) width =5;
  else if (mass>300&&mass<600) width = 10;
  else if (mass>=600&&mass<1000) width = 25;
  else if (mass>=1000&&mass<2000) width = 150;
  else if (mass>=2000&&mass<2300) width=200;
  else if (mass>=2300&&mass<=3000) width=500;
  else width=800;
//merge
  for (int j=(mass-width);j<(mass+width);j++){
       gen[i]+=gen_raw[j];
       reco[i]+=reco_raw[j];
      }
 genE[i]=sqrt(gen[i]);
 recoE[i]=sqrt(reco[i]);

// calculate efficiency and efficiency error
 double g,l,r;
 g=gen[i];
 r=reco[i];
 l=(g-r);
 if(g!=0){
 eff[i]=r/g;
 effE[i]=sqrt(l*l*r+r*r*l)/(g*g);
//temp fix
if((spin==0||spin==3) && i>=33) effE[i] = effE[i]*0.5; 
 }
 else {
 eff[i]=0;
 effE[i]=0;
 }
//cout<<i<<":"<<reco[i]<<","<<gen[i]<<","<<eff[i]<<endl;
}

  TF1 *polyFunctot= new TF1("polyFunctot","([0]+[1]*TMath::Erf( (x-[2])/[3] ))*([4]+[5]*x+[6]*x*x+[10]*x*x*x)+[7]*TMath::Gaus(x,[8],[9])", 110., 4000);

  if (spin!=2){ 
  polyFunctot->SetParameters(-4.466301E+00,4.575400E+00,-1.243030E+02,1.367442E+02,2.829370E+00,2.538899E-03,-1.077109E-06,1.575787E-02,1.728418E+02,2.485051E+01,1.385591E-10);
  polyFunctot->SetParLimits(7,0,0.2);
  polyFunctot->SetParLimits(8,160,210);
  polyFunctot->SetParLimits(9,10,70);
   }

  if(spin==0)   polyFunctot->SetLineStyle(1);

  if(spin==2) {
  polyFunctot->SetParameters(-4.416859E+00,4.621453E+00,-6.257656E+01,1.123821E+02,1.949967E+00,1.158274E-03,-3.868945E-07,-9.756119E-02,2.236640E+02,-1.180111E+02,2.620831E-11);
  polyFunctot->SetLineStyle(2);}
  polyFunctot->SetLineColor(color);

  TGraphErrors *gr = new TGraphErrors (n,M,eff,massE,effE);

  gr->SetMarkerColor(color);
  gr->SetMarkerStyle(marker);
  if (spin==2)gr->SetLineStyle(line);
  gr->SetMarkerSize(1);
  cout<<endl<<endl<<"parameters:"<<endl;
  TFitResultPtr r = gr->Fit("polyFunctot","S");
  gr->SetLineColor(color);

  ofstream fp;
  fp.open ("efficiency_param.txt",std::ofstream::out | std::ofstream::app);
   for (int i=0;i<11;i++) {
   Double_t value = r->Parameter(i);
   char par[200];
   int m;
   if (spin==0) m=0; else if (spin==2) m=1; else if (spin==3) m=2;
   sprintf(par,"%s_%s_param_%d: %E",mode[m],chan[ch],i,value);
   fp<<par<<endl;
   }
   fp<<endl;
  fp.close();

  delete hreco;
  return gr;
}

void ploteff_sig(){

int start_s=clock();

  TCanvas* c2 = new TCanvas("c2", "c2", 1000, 10, 1400, 800);
  //c2->SetLogx(); 
  c2->SetFillColor(0);
  c2->SetRightMargin(0.18);
  TMultiGraph *mg = new TMultiGraph();

cout<<"ggH_0+_4mu:"<<endl;
  TGraphErrors* ggH_4mu = makegr(0,0,kRed,20,1);
cout<<"ggH_0+_4e:"<<endl;
  TGraphErrors* ggH_4e = makegr(0,1,kGreen,20,1);
cout<<"ggH_0+_2e2mu:"<<endl;
  TGraphErrors* ggH_2e2mu = makegr(0,2,kBlue,20,1);
cout<<"VBF_0+_4mu:"<<endl;
  TGraphErrors* vbf_4mu = makegr(3,0,kRed+2,23,1);
cout<<"VBF_0+_4e:"<<endl;
  TGraphErrors* vbf_4e = makegr(3,1,kGreen+2,23,1);
cout<<"VBF_0+_2e2mu:"<<endl;
  TGraphErrors* vbf_2e2mu = makegr(3,2,kBlue+2,23,1);
cout<<"ggH_2b+_4mu:"<<endl;
  TGraphErrors* spin2_4mu = makegr(2,0,kRed,24,2);
cout<<"ggH_2b+_4e:"<<endl;
  TGraphErrors* spin2_4e = makegr(2,1,kGreen,24,2);
cout<<"ggH_2b+_2e2mu:"<<endl;
  TGraphErrors* spin2_2e2mu = makegr(2,2,kBlue,24,2);

  mg->Add(ggH_4e);
  mg->Add(ggH_4mu);
  mg->Add(ggH_2e2mu);
  mg->Add(vbf_4e);
  mg->Add(vbf_4mu);
  mg->Add(vbf_2e2mu);
  mg->Add(spin2_4e);
  mg->Add(spin2_4mu);
  mg->Add(spin2_2e2mu);
  mg->Draw("AP");
  mg->GetXaxis()->SetTitle("genHMass [GeV]");
  mg->GetYaxis()->SetTitle("efficiency*acceptance");
  mg->GetYaxis()->SetRangeUser(0,1);
  mg->GetXaxis()->SetRangeUser(100,3000);

  TLegend* leg3 = new TLegend(.83,0.3,0.99,.85);
  leg3->SetFillColor(0);
  leg3->SetBorderSize(0);
  leg3->AddEntry(ggH_4mu,"ggH->0+ 4mu","pl");
  leg3->AddEntry(ggH_2e2mu,"ggH->0+ 2e2mu","pl");
  leg3->AddEntry(ggH_4e,"ggH->0+ 4e","pl");  
  leg3->AddEntry(vbf_4mu,"VBF->0+ 4mu","pl");
  leg3->AddEntry(vbf_2e2mu,"VBF->0+ 2e2mu","pl");
  leg3->AddEntry(vbf_4e,"VBF->0+ 4e","pl");
  leg3->AddEntry(spin2_4mu,"ggH->2b+ 4mu","pl");
  leg3->AddEntry(spin2_2e2mu,"ggH->2b+ 2e2mu","pl");
  leg3->AddEntry(spin2_4e,"ggH->2b+ 4e","pl");
  leg3->Draw();

  c2->Update();
//  c2->SaveAs("/afs/cern.ch/user/c/cayou/www/HighMass/160726/test.png");
  c2->SaveAs("/afs/cern.ch/user/c/cayou/www/HighMass/160726/sigEfficiencyFit_reg.png");
  c2->SaveAs("/afs/cern.ch/user/c/cayou/www/HighMass/160726/sigEfficiencyFit_reg.pdf");
int stop_s=clock();
cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)<< endl;
}

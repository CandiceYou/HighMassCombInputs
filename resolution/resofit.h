#include "TDirectory.h"
#include "TPad.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "THStack.h"
#include "TH2.h"
#include "TF1.h"
#include "TLine.h"
#include "TCut.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "HZZ2L2QRooPdfs.h"
#include "HZZ2L2QRooPdfs.cc+"
//#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
//#include "ZZAnalysis/AnalysisStep/interface/Category.h"
//#include "Math/GenVector/LorentzVector.h"
//#include "Math/GenVector/PtEtaPhiM4D.h"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

//#include "Math/GenVector/LorentzVector.h"
//#include "Math/GenVector/PtEtaPhiM4D.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooSimultaneous.h"
//#include "RooCategory.h"
#include "RooWorkspace.h"
#include "RooHist.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TPaveText.h"
#include "RooAddPdf.h"
#include "RooBreitWigner.h"
#include "RooFitResult.h"
#include "RooFFTConvPdf.h"
#include "RooAddition.h"
#include "RooMinuit.h"
#include "Math/MinimizerOptions.h"
#include <iomanip>
#include "RooAbsCollection.h"
#include "RooWorkspace.h"
using namespace RooFit;
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


  float m4l,genM,Z1Mass,Z2Mass;
  std::vector<float> *wt=0;
  Short_t z1flav, z2flav,zzsel;
  float weight;
  char tempmass[100];
  char tempmass2[100];
  double width[100];
  double xMinF=-2000;
  double xMaxF=1500;
  double xMin[100];
  double xMax[100];

  RooRealVar x("reso","m_{reco}-m_{true}",0.,xMinF,xMaxF,"GeV");
  RooRealVar w("myW","myW",1.0,-2000.,1500.);
  RooCategory massrc("massrc","massrc");
  RooDataSet* dataset;
  RooDataSet* dataset_sub[100];
  vector<TString> files;
  char inputfile[1000];

  RooDoubleCB* DCBall[100];
  RooRealVar* mean_ind[100];
  RooRealVar* sigma_ind[100];
  RooRealVar* a1_ind[100];
  RooRealVar* a2_ind[100];
  RooRealVar* n1_ind[100];
  RooRealVar* n2_ind[100];
  RooFitResult* fitres[100];

  TString  inputDir = "/afs/cern.ch/work/c/cayou/public/80Xsamples/4lsamples_0726/";
  int massBin[]={115,120,125,126,130,135,140,145,150,155,160,170,175,180,200,230,250,350,400,450,500,600,750,800,900,1000};
//  int massBin[]={120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800,820,840,860,880,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2800,3000/*,3200,3400,3600*/};
  int inputfiles[]={115,120,125,126,130,135,140,145,150,155,160,170,175,180,200,230,250,350,400,450,500,600,750,900};
//  int inputfiles[]={115,120,124,125,126,130,135,140,145,150,155,160,170,175,180,200,210,250,270,300,350,400,450,500,550,600,700,750,800,900,1500,2500,3000};
//  int massBin[]={115,120};
//  int inputfiles[]={115,120};
  int maxMassBin=sizeof(massBin)/sizeof(*massBin);;
  int Nfiles=sizeof(inputfiles)/sizeof(*inputfiles);


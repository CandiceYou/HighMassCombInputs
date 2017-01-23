#include <vector>
#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include <fstream>
#include <iostream>
#include "TLorentzVector.h"
#include "TLorentzRotation.h"
using namespace std;

//mode : 0 ggZZ, 1 VBF
//ch : 0 4mu, 1 4e, 2 2e2mu.

TGraphErrors* makegr(int mode=0, int eleSel=0, int ch=0, Color_t color=2, int marker=20, int line=1){
	gStyle->SetPadLeftMargin(0.1);
	gStyle->SetPadRightMargin(0.3);
	gStyle->SetOptStat(0000);
	gStyle->SetTitleFontSize(0.05);

	char* modeName[2] ={"ggZZ","VBF"};
	char* eleSelName[3]={"REG","TLE","RSE"};
	char* chan[3] ={"4mu","4e","2e2mu"};


        int zzflav,zzflav_tle;
        switch (ch){
                case 0: zzflav=28561;  break; //4mu
                case 1: zzflav=14641;zzflav_tle=29282;  break; //4e
                case 2: zzflav=20449;zzflav_tle=40898;  break; //2e2mu
                default:  cout<<"channel unknown.";
        }


	//Load Samples

	  TString  inputDir = "/afs/cern.ch/work/c/cayou/public/80Xsamples/samples_2016mc_0726/";
          TString  inputfile;
          if (mode==0) inputfile = Form("ggTo%s_Contin_MCFM701/ZZ4lAnalysis.root",chan[ch]);
          else if (mode==1) inputfile = Form("VBFTo%sJJ_Contin_phantom128/ZZ4lAnalysis.root",chan[ch]);


        TChain *candTree_gen = new TChain;
        candTree_gen->SetName("ZZTree/candTree");

        TChain *candTree = new TChain;
	if (eleSel==0) candTree->SetName("ZZTree/candTree");
	else if (eleSel==1) candTree->SetName("ZZTreetle/candTree");
	else if (eleSel==2) candTree->SetName("ZZTreelooseEle/candTree");

        candTree_gen->Add(inputDir+inputfile);
        candTree->Add(inputDir+inputfile);

	// draw raw histogram with 1GeV binning, 0GeV to 3500GeV.
	const Int_t m=3500;
	TH1F *hreco = new TH1F("hreco","hreco",m,0,m);
        TH1F *hgen = new TH1F("hgen","hgen",m,0,m);

        // Fill gen histograms
        short genFinalState_gen=0;
        float GenHMass_gen=0,genHEPMCweight_gen=0,PUWeight_gen=0;
        candTree_gen->SetBranchAddress("GenHMass",&GenHMass_gen);
        candTree_gen->SetBranchAddress("genFinalState",&genFinalState_gen);
        candTree_gen->SetBranchAddress("genHEPMCweight",&genHEPMCweight_gen);
        candTree_gen->SetBranchAddress("PUWeight",&PUWeight_gen);

	// Fill reco histograms
	short genFinalState=0,ZZsel=0,Z1Flav=0,Z2Flav=0;
	vector<short> *LepLepId=0,*LepisLoose=0;
	vector<float> *LepPt=0,*LepEta=0,*LepPhi=0;
	float GenHMass=0,genHEPMCweight=0,PUWeight=0,dataMCWeight=0;


	candTree->SetBranchAddress("GenHMass",&GenHMass);
	candTree->SetBranchAddress("genFinalState",&genFinalState);
	candTree->SetBranchAddress("ZZsel",&ZZsel);
	candTree->SetBranchAddress("Z1Flav",&Z1Flav);
	candTree->SetBranchAddress("Z2Flav",&Z2Flav);
	candTree->SetBranchAddress("genHEPMCweight",&genHEPMCweight);
	candTree->SetBranchAddress("dataMCWeight",&dataMCWeight);
	candTree->SetBranchAddress("PUWeight",&PUWeight);
	candTree->SetBranchAddress("LepLepId",&LepLepId);
	candTree->SetBranchAddress("LepPt",&LepPt);
	candTree->SetBranchAddress("LepEta",&LepEta);
	candTree->SetBranchAddress("LepPhi",&LepPhi);
 


//int limit =(100000<candTree->GetEntries())?100000:candTree->GetEntries();
//cout << limit <<endl;
//
        for (int i = 0; i < candTree_gen->GetEntries(); i++) {
//       for (int i = 0; i <100 ; i++) {
                candTree_gen->GetEntry(i);
                if(genFinalState_gen!=ch) continue;
                hgen->Fill(GenHMass_gen,(genHEPMCweight_gen * PUWeight_gen));
         }

	for (int i = 0; i < candTree->GetEntries(); i++) {
//        for (int i = 0; i <100 ; i++) {
//        for (int i = 0; i <limit ; i++) {
                candTree->GetEntry(i);
     		if(genFinalState!=ch) continue;
		if((eleSel==0||eleSel==2) && ZZsel==120 && fabs(Z1Flav*Z2Flav)== zzflav){ //REG and RSE
				hreco->Fill(GenHMass,(genHEPMCweight * PUWeight * dataMCWeight));
		}

		else if (eleSel==1 && ZZsel==120){ //TLE
			int TLE_index=-1, lep1_index=-1, lep2_index=-1;
			for (int i=0 ; i<LepLepId->size() ; i++){ 
				if (abs(LepLepId->at(i))==22) TLE_index=i;
			}
			if (TLE_index<=1) lep1_index=2, lep2_index=3;
			else lep1_index=0, lep2_index=1;
			TLorentzVector lep1,lep2,lep_TLE;
			lep1.SetPtEtaPhiM(LepPt->at(lep1_index),LepEta->at(lep1_index),LepPhi->at(lep1_index),0);
			lep2.SetPtEtaPhiM(LepPt->at(lep2_index),LepEta->at(lep2_index),LepPhi->at(lep2_index),0);
			lep_TLE.SetPtEtaPhiM(LepPt->at(TLE_index),LepEta->at(TLE_index),LepPhi->at(TLE_index),0);
			double TLE_dR_Z = lep_TLE.DeltaR(lep1+lep2);
			//  cout<<TLE_index<<","<<lep1_index<<","<<lep2_index<<",deltaR"<<TLE_dR_Z<<",pt"<<LepPt->at(TLE_index)<<endl;
			if (LepPt->at(TLE_index)>30 && TLE_dR_Z>1.6 && fabs(Z1Flav*Z2Flav)==zzflav_tle){
					hreco->Fill(GenHMass,(genHEPMCweight * PUWeight * dataMCWeight));
			} 
		}
	}
	//Calculate Efficiency 

	// bin contents of raw histograms
	double M_raw[m]={0};
	double gen_raw[m]={0};
	double reco_raw[m]={0};

	// const Int_t n=35;
	// double M[n]={115,120,125,126,135,140,145,150,155,160,180,200,230,250,350,400,450,500,600,750,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600};

//	 const Int_t n=32;
//	 double M[n]={115,120,125,126,135,140,145,150,155,160,180,200,230,250,350,400,450,500,600,750,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000};

         const Int_t n=21;
         double M[n]={200,250,300,350,400,450,500,600,750,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000};
//        const Int_t n=15; 
//        double M[n]={300,400,450,500,550,600,700,750,800,900,1000,1500,2000,2500,3000};

//	const Int_t n=1;
//	double M[n]={750};

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

	for (int i=0;i<n;i++){
		mass = M[i];
  if(mass<=500) width=20;
  else if(mass>500 && mass <=800) width=50;
  else if(mass>800 && mass <=1500) width=150;
  else if(mass>1500 && mass <=2000) width=300;
  else width=500;
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
		}
		else {
			eff[i]=0;
			effE[i]=0;
		}
		//cout<<i<<":"<<reco[i]<<","<<gen[i]<<","<<eff[i]<<endl;
	}

	TF1 *polyFunctot= new TF1("polyFunctot","([0]+[1]*TMath::Erf( (x-[2])/[3] ))*([4]+[5]*x+[6]*x*x+[10]*x*x*x)+[7]*TMath::Gaus(x,[8],[9])", 110., 3500);
	polyFunctot->SetParameters(-4.42749e+00,4.61212e+0,-6.21611e+01,1.13168e+02,2.14321e+00,1.04083e-03,4.89570e-07, 0.03, 200, 30,0);
	polyFunctot->SetParLimits(7,0,0.2);
	polyFunctot->SetParLimits(8,160,210);
	polyFunctot->SetParLimits(9,10,70);
	polyFunctot->SetLineColor(color);
	if(mode==0)   polyFunctot->SetLineStyle(1);
	else if(mode==1) {
		polyFunctot->SetParLimits(7,0,0);
		polyFunctot->SetParLimits(8,0,0);
		polyFunctot->SetParLimits(9,0,0); 
		polyFunctot->SetLineStyle(2);}

	TGraphErrors *gr = new TGraphErrors (n,M,eff,massE,effE);

	gr->SetMarkerColor(color);
	gr->SetMarkerStyle(marker);
	gr->SetMarkerSize(1);
	cout<<endl<<endl<<"parameters:"<<endl;
//	TFitResultPtr r = gr->Fit("polyFunctot","S");
	gr->SetLineColor(color);

/*
	ofstream fp;
	fp.open ("efficiency_param.txt",std::ofstream::out | std::ofstream::app);
	for (int i=0;i<11;i++) {
		Double_t value = r->Parameter(i);
		char par[200];
		sprintf(par,"%s_%s_param_%d: %E",modeName[mode],chan[ch],i,value);
		fp<<par<<endl;
	}
	fp<<endl;
	fp.close();
*/
	delete hreco;
	return gr;
}

void bkg_rse(){
	TCanvas* c2 = new TCanvas("c2", "c2", 1000, 10, 1400, 800);
	//c2->SetLogx(); 
	c2->SetFillColor(0);
	c2->SetRightMargin(0.18);
	TMultiGraph *mg = new TMultiGraph();
	//debug

	TGraphErrors* ggZZ_4e = makegr(0,2,1,kGreen,20,1);
	TGraphErrors* ggZZ_2e2mu = makegr(0,2,2,kBlue,20,1);
        TGraphErrors* VBF_4e = makegr(1,2,1,kGreen+2,23,1);
        TGraphErrors* VBF_2e2mu = makegr(1,2,2,kBlue+2,23,1);

TFile f ("bkg_rse_eff.root","recreate");
f.cd();
ggZZ_4e->SetName("ggZZ_rse_4e"); ggZZ_4e->Write();
ggZZ_2e2mu->SetName("ggZZ_rse_2e2mu"); ggZZ_2e2mu->Write();
VBF_4e->SetName("VBF_rse_4e"); VBF_4e->Write();
VBF_2e2mu->SetName("VBF_rse_2e2mu"); VBF_2e2mu->Write();
f.Close();

	mg->Add(ggZZ_4e);
	mg->Add(ggZZ_2e2mu);
        mg->Add(VBF_4e);
        mg->Add(VBF_2e2mu);

	mg->Draw("APL");
           mg->GetXaxis()->SetTitle("genHMass [GeV]");
           mg->GetYaxis()->SetTitle("efficiency*acceptance");
           mg->GetYaxis()->SetRangeUser(0,0.3);
           mg->GetXaxis()->SetRangeUser(200,3000);

           TLegend* leg3 = new TLegend(.83,0.3,0.99,.85);
           leg3->SetFillColor(0);
           leg3->SetBorderSize(0);
           leg3->AddEntry(ggZZ_4e,"ggZZ_bkg 4e,RSE","pl");
           leg3->AddEntry(ggZZ_2e2mu,"ggZZ_bkg 2e2mu,RSE","pl");
           leg3->AddEntry(VBF_4e,"VBF_bkg 4e,RSE","pl");
           leg3->AddEntry(VBF_2e2mu,"VBF_bkg 2e2mu,RSE","pl");
           leg3->Draw();

	c2->Update();
	c2->SaveAs("/afs/cern.ch/user/c/cayou/www/HighMass/160726/bkgEfficiency_rse.png");
	c2->SaveAs("/afs/cern.ch/user/c/cayou/www/HighMass/260726/bkgEfficiency_rse.pdf");
}

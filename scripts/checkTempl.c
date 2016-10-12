#include "TF1.h"
#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "../src/normalize.cc"
using namespace std;

void make1DSlice(int spin=0 ,char* process="sig_resolved"){
	gStyle->SetOptStat(0);

	const int nbinx=370, nbiny=30 ,Xmin=300 , Xmax=4000; //check it's consistent with smoothing templates
	float width = (Xmax-Xmin)/nbinx;

	char* dir = "/afs/cern.ch/work/c/cayou/HighMass_RunII/CMSSW_7_6_3/src/ZZAnalysis/AnalysisStep/test/CombInputs/HighMassStudy_2l2q/run/";
	TFile* file = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_nominal_2Dtemp.root",dir,spin,spin));
	TFile* file_up = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_JECup_2Dtemp.root",dir,spin,spin));
	TFile* file_dn = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_JECdn_2Dtemp.root",dir,spin,spin));

	TH2F* templ2D=(TH2F*)file->Get(process);
	TH2F* templ2D_up=(TH2F*)file_up->Get(process);
	TH2F* templ2D_dn=(TH2F*)file_dn->Get(process);

	/*
	//check 2D
	TCanvas* c=new TCanvas("c1", "c1", 800,800);  
	c->SetFillColor(0);
	c->cd();
	templ2D_dn->Draw("COL");
	templ2D_dn->SetTitle("");
	templ2D_dn->GetXaxis()->SetTitle("m_2l2q");
	templ2D_dn->GetYaxis()->SetTitle("D_Zjj");
	c->Update();
	c->SaveAs(Form("spin%d_%s_dn.png",spin,process));
	*/
	TH1 *hbins[nbinx] ;
	TH1 *hbins_up[nbinx] ;
	TH1 *hbins_dn[nbinx] ;

	//find each mass slice TH1F
	for (int i=1;i<=nbinx;i++) {
		int m = (i-1)*width+Xmin; //find mass
		hbins[i-1] = new TH1F(Form("HM%d",m),Form("HM%d",m),nbiny,0,1);
		hbins_up[i-1] = new TH1F(Form("HM%d_up",m),Form("HM%d_up",m),nbiny,0,1);
		hbins_dn[i-1] = new TH1F(Form("HM%d_dn",m),Form("HM%d_dn",m),nbiny,0,1);

		for (int j=1;j<=nbiny;j++) {
			float v = templ2D->GetBinContent(i,j);
			hbins[i-1]->SetBinContent(j,v);
			float vup = templ2D_up->GetBinContent(i,j);
			hbins_up[i-1]->SetBinContent(j,vup);
			float vdn = templ2D_dn->GetBinContent(i,j);
			hbins_dn[i-1]->SetBinContent(j,vdn);
			//cout<<"v="<<v<<", v_up="<<vup<<", v_dn="<<vdn<<endl;
		}

		if (m%500==0){
			TCanvas* c2=new TCanvas("c2", "c2", 800,800);
			c2->SetFillColor(0);
			c2->cd();
			hbins_up[i-1]->SetLineColor(2);
			hbins_dn[i-1]->SetLineColor(3);
			double max1=hbins_up[i-1]->GetMaximum()*1.2;
			double max2=hbins_dn[i-1]->GetMaximum()*1.2;
			double max_y = max1>max2?max1:max2;
			hbins[i-1]->GetYaxis()->SetRangeUser(0,max_y);
			hbins[i-1]->Draw();
			hbins[i-1]->SetTitle(Form("spin%d %s %dGeV",spin,process,m));
			hbins[i-1]->GetXaxis()->SetTitle("D_Zjj");
			hbins_up[i-1]->Draw("Same");
			hbins_dn[i-1]->Draw("Same");
			/*
			   for (int j=1;j<=nbiny;j++) {
			   cout<< hbins[i-1]->GetBinContent(j) <<","
			   <<hbins_up[i-1]->GetBinContent(j)<<","
			   <<hbins_dn[i-1]->GetBinContent(j)<<endl;}
			   */
			TLegend* leg = new TLegend(0.4,0.65,0.6,0.85);
			leg->AddEntry(hbins[i-1],"nominal","l");
			leg->AddEntry(hbins_up[i-1],"JEC up","l");
			leg->AddEntry(hbins_dn[i-1],"JEC down","l");
			leg->SetBorderSize(0);
			leg->Draw();

			c2->SaveAs(Form("2l2q2DTemplate/spin%d/1DSlices/spin%d_%s_MH%d_1D.png",spin,spin,process,m));
			delete c2;
		}
		delete hbins[i-1];
		delete hbins_up[i-1];
		delete hbins_dn[i-1];

	}

}
void checkTempl(){
	char* process[8]={"DY_resolved","DY_merged","TTbar_resolved","TTbar_merged","Diboson_resolved","Diboson_merged","sig_resolved","sig_merged"};
	for (int i=0;i<8;i++){
		make1DSlice(0,process[i]);
		make1DSlice(2,process[i]);
	}
}

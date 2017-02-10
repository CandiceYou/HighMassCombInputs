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

void normTempl(int spin=2 ,char*JEC ="nominal",  bool check=0){

        TFile* file = TFile::Open(Form("../../TemplateBuilder/2l2q_spin%d_smooth_%s.root",spin,JEC));
	const int nProcess=8;
	char* process[nProcess]={"DY_resolved","DY_merged","TTbar_resolved","TTbar_merged","Diboson_resolved","Diboson_merged","sig_resolved","sig_merged"};
	TH2F* histSm[nProcess];
	TH2F* histSmNorm[nProcess];
	TCanvas* c[nProcess];
	TCanvas* c2[nProcess];

	for (i=0 ; i<nProcess ; i++){
		cout<<process[i]<<endl;
		histSm[i]=(TH2F*)file->Get(process[i]);
		if (i==6) histSmNorm[i]=normalize(histSm[i],1,1500); //sig resolved
		else if (i==7) histSmNorm[i]=normalize(histSm[i],1,2500); //sig merged
		else if (i==0 || i==2 || i==4) histSmNorm[i]=normalize(histSm[i],1,1000); //bkg resolved
		else if (i==1 || i==3 || i==5) histSmNorm[i]=normalize(histSm[i],1,1000); //bkg merged
		histSmNorm[i]->SetName(process[i]);
	}


	//plotting smoothed un-normalized template
	gStyle->SetPalette(1);
	if (check==1){
		for(int i=0;i<nProcess;i++){
			c[i]=new TCanvas(Form("c%d",i), Form("c%d",i), 800,800);
			c[i]->SetFillColor(0);
			c[i]->cd();
			histSm[i]->Draw("COL");
			histSm[i]->SetTitle("");
			histSm[i]->GetXaxis()->SetTitle("m_2l2q");
			histSm[i]->GetYaxis()->SetTitle("D_Zjj");
			c[i]->Update();
			c[i]->SaveAs(Form("2l2q2DTemplate/spin%d/spin%d_%s_%s_sm.png",spin,spin,process[i],JEC));
			c[i]->SaveAs(Form("2l2q2DTemplate/spin%d/spin%d_%s_%s_sm.pdf",spin,spin,process[i],JEC));
		}}

	//plotting smoothed and normalized template
	gStyle->SetPalette(1);

	for(int i=0;i<nProcess;i++){
		c2[i]=new TCanvas(Form("c%d",i), Form("c%d",i), 800,800);
		c2[i]->SetFillColor(0);
		c2[i]->cd();
		histSmNorm[i]->Draw("COL");
		histSmNorm[i]->SetTitle("");
		histSmNorm[i]->GetXaxis()->SetTitle("m_2l2q");
		histSmNorm[i]->GetYaxis()->SetTitle("D_Zjj");
		c2[i]->Update();
		c2[i]->SaveAs(Form("2l2q2DTemplate/spin%d/spin%d_%s_%s.png",spin,spin,process[i],JEC));
		c2[i]->SaveAs(Form("2l2q2DTemplate/spin%d/spin%d_%s_%s.pdf",spin,spin,process[i],JEC));
	}



	//store
	TFile ftemp (Form("2l2q2DTemplate/spin%d/2l2q_spin%d_%s_2Dtemp.root",spin,spin,JEC),"recreate");
	ftemp.cd();
	for (i=0 ; i<nProcess ; i++){histSmNorm[i]->Write();}
	ftemp.Close();
}


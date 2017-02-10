#include "TF1.h"
#include "TText.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
using namespace std;

void make1DSlice(int spin=0 ,char* bkgch="DY", char* cat="resolved"){


	gStyle->SetOptStat(0);

//	char* dir = "/afs/cern.ch/work/c/cayou/HighMass_RunII/CMSSW_7_6_3/src/ZZAnalysis/AnalysisStep/test/CombInputs/HighMassStudy_2l2q/run/";
        char* dir = ".";
	TFile* file = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_nominal_2Dtemp.root",dir,spin,spin));
	TFile* file_up = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_JECup_2Dtemp.root",dir,spin,spin));
	TFile* file_dn = TFile::Open(Form("%s/2l2q2DTemplate/spin%d/2l2q_spin%d_JECdn_2Dtemp.root",dir,spin,spin));

	TH2F* templ2D_DY=(TH2F*)file->Get(Form("%s_%s",bkgch,cat));
	TH2F* templ2D_up_DY=(TH2F*)file_up->Get(Form("%s_%s",bkgch,cat));
	TH2F* templ2D_dn_DY=(TH2F*)file_dn->Get(Form("%s_%s",bkgch,cat));

	TH2F* templ2D_sig=(TH2F*)file->Get(Form("sig_%s",cat));
	TH2F* templ2D_up_sig=(TH2F*)file_up->Get(Form("sig_%s",cat));
	TH2F* templ2D_dn_sig=(TH2F*)file_dn->Get(Form("sig_%s",cat));

	float ZZMass=0,weight=0,KD=0;
	short jetType=0;
	char* MCname=bkgch;
	if (bkgch=="TTbar") MCname="TTBar";
        else if (bkgch=="DY") MCname="DYjets";
	int  typ;

	if (cat=="merged") typ=0;
	else if (cat=="resolved") typ=1;

	TFile* MC_file = TFile::Open(Form("%s/2l2qSelectedTrees/2l2qtree_%s.root",dir,MCname));
	TTree* MC_Tree = (TTree*)MC_file->Get("selectedTree");
	MC_Tree->SetBranchAddress("ZZMass",&ZZMass);
	MC_Tree->SetBranchAddress("weight",&weight);
	MC_Tree->SetBranchAddress("jetType",&jetType);
	if(spin==0)
		MC_Tree->SetBranchAddress("Dbkg_0plus",&KD);
	else if (spin==2)
		MC_Tree->SetBranchAddress("Dbkg_2bplus",&KD);

	TFile* data_file = TFile::Open(Form("%s/2l2qSelectedTrees/2l2qtree_data.root",dir,MCname));
	TTree* data_Tree = (TTree*)data_file->Get("selectedTree");
	data_Tree->SetBranchAddress("ZZMass",&ZZMass);
	data_Tree->SetBranchAddress("weight",&weight);
	data_Tree->SetBranchAddress("jetType",&jetType);
	if(spin==0)
		data_Tree->SetBranchAddress("Dbkg_0plus",&KD);
	else if (spin==2)
		data_Tree->SetBranchAddress("Dbkg_2bplus",&KD);

	TAxis *xaxis = templ2D_DY->GetXaxis();
	TAxis *xaxis_sig = templ2D_sig->GetXaxis();

	int nbinx=templ2D_DY->GetNbinsX();
	int nbiny=30;
	TH1 *hbins_DY[nbinx];
	TH1 *hbins_up_DY[nbinx];
	TH1 *hbins_dn_DY[nbinx];

	TH1 *hbins_sig[nbinx];
	TH1 *hbins_up_sig[nbinx];
	TH1 *hbins_dn_sig[nbinx];

	TH1 *MC_KD[nbinx];
	TH1 *data_KD[nbinx];
	for (int i=1;i<=nbinx;i++) {
		MC_KD[i-1] = new TH1F(Form("MC_HM%d",i),Form("MC_HM%d",i),nbiny,0,1);
		data_KD[i-1] = new TH1F(Form("data_HM%d",i),Form("data_HM%d",i),nbiny,0,1);
	}

	//find each mass slice TH1F
	for (int ev = 0; ev < MC_Tree->GetEntries(); ev++) {
		MC_Tree->GetEntry(ev);
		if (ZZMass>4000) continue;
		int massbin=templ2D_DY->GetXaxis()->FindBin(ZZMass);
		//cout<<ZZMass<<" "<<massbin<<endl;
		if (jetType==typ)MC_KD[massbin-1]->Fill(KD,weight);
	}

	for (int ev = 0; ev < data_Tree->GetEntries(); ev++) {
		data_Tree->GetEntry(ev);
		if (ZZMass>4000) continue;
		int massbin=templ2D_DY->GetXaxis()->FindBin(ZZMass);
		if (jetType==typ) {data_KD[massbin-1]->Fill(KD);}
	}




	for (int i=1;i<=nbinx;i++) {
		int m_low = xaxis->GetBinLowEdge(i); //find mass low end
		int m_high = xaxis->GetBinUpEdge(i); //find mass high end     
		int b_bkg = i; 
		int b_sig = xaxis_sig->FindBin((m_low+m_high)/2);

		//plot projection
		hbins_DY[i-1] = new TH1F(Form("HM%dto%d",m_low,m_high),Form("HM%dto%d",m_low,m_high),nbiny,0,1);
		hbins_up_DY[i-1] = new TH1F(Form("HM%dto%d_up",m_low,m_high),Form("HM%dto%d_up",m_low,m_high),nbiny,0,1);
		hbins_dn_DY[i-1] = new TH1F(Form("HM%dto%d_dn",m_low,m_high),Form("HM%dto%d_dn",m_low,m_high),nbiny,0,1);

		hbins_sig[i-1] = new TH1F(Form("sigHM%dto%d",m_low,m_high),Form("sigHM%dto%d",m_low,m_high),nbiny,0,1);
		hbins_up_sig[i-1] = new TH1F(Form("sigHM%dto%d_up",m_low,m_high),Form("sigHM%dto%d_up",m_low,m_high),nbiny,0,1);
		hbins_dn_sig[i-1] = new TH1F(Form("sigHM%dto%d_dn",m_low,m_high),Form("sigHM%dto%d_dn",m_low,m_high),nbiny,0,1);

		/*
		   int ferr=0;
		   if (cat=="merged") ferr=10;
		   else if (cat=="resolved") ferr=3; 
		   */
		for (int j=1;j<=nbiny;j++) {
			float v_bkg = templ2D_DY->GetBinContent(b_bkg,j);
			float v_sig = templ2D_sig->GetBinContent(b_sig,j);

			//			float vup_bkg = templ2D_up_DY->GetBinContent(b_bkg,j);
			float vup_bkg=v_bkg;
			if(b_bkg!=nbinx) vup_bkg = templ2D_DY->GetBinContent(b_bkg+1,j);
			float vup_sig = templ2D_up_sig->GetBinContent(b_sig,j);

			//			float vdn_bkg = templ2D_dn_DY->GetBinContent(b_bkg,j);
			float vdn_bkg=v_bkg;
			if(b_bkg!=1)vdn_bkg = templ2D_DY->GetBinContent(b_bkg-1,j);
			float vdn_sig = templ2D_dn_sig->GetBinContent(b_sig,j);

			if (b_bkg==nbinx) vup_bkg = v_bkg*(v_bkg/vdn_bkg);
			if (b_bkg==1) vdn_bkg = v_bkg*(v_bkg/vup_bkg);

			hbins_DY[i-1]->SetBinContent(j,v_bkg);
			hbins_sig[i-1]->SetBinContent(j,v_sig);
			hbins_up_DY[i-1]->SetBinContent(j,vup_bkg);
			hbins_up_sig[i-1]->SetBinContent(j,vup_sig);
			hbins_dn_DY[i-1]->SetBinContent(j,vdn_bkg);
			hbins_dn_sig[i-1]->SetBinContent(j,vdn_sig);
			//cout<<"v="<<v<<", v_up="<<vup<<", v_dn="<<vdn<<endl;
		}

		double norm = data_KD[i-1]->Integral();
		if (norm ==0) norm=1;
		MC_KD[i-1]->Scale(norm/MC_KD[i-1]->Integral());
		hbins_DY[i-1]->Scale(norm/hbins_DY[i-1]->Integral());
		hbins_up_DY[i-1]->Scale(norm/hbins_up_DY[i-1]->Integral());
		hbins_dn_DY[i-1]->Scale(norm/hbins_dn_DY[i-1]->Integral());
		hbins_sig[i-1]->Scale(norm/hbins_sig[i-1]->Integral());
		hbins_up_sig[i-1]->Scale(norm/hbins_up_sig[i-1]->Integral());
		hbins_dn_sig[i-1]->Scale(norm/hbins_dn_sig[i-1]->Integral());

		double max1=hbins_up_DY[i-1]->GetMaximum()*1.2;
		double max2=hbins_dn_DY[i-1]->GetMaximum()*1.2;
		double max12 = max1>max2?max1:max2;
		double max3=MC_KD[i-1]->GetMaximum()*1.2;
		double max4=data_KD[i-1]->GetMaximum()*1.2;
		double max34 = max3>max4?max3:max4;
		double max_y = max34>max12?max34:max12;
		double max5 = hbins_sig[i-1]->GetMaximum()*1.2;
		max_y = max_y>max5?max_y:max5;

		if (true){
			TCanvas* c2=new TCanvas("c2", "c2", 800,800);
			c2->SetFillColor(0);
			c2->cd();
			hbins_up_DY[i-1]->SetLineStyle(2);
			hbins_dn_DY[i-1]->SetLineStyle(3);
			hbins_DY[i-1]->GetYaxis()->SetRangeUser(0,max_y);
			hbins_DY[i-1]->Draw();
			hbins_DY[i-1]->SetTitle(Form("spin%d %s %dto%dGeV",spin,cat,m_low,m_high));
			hbins_DY[i-1]->GetXaxis()->SetTitle("D_Zjj");
			hbins_up_DY[i-1]->Draw("Same");
			hbins_dn_DY[i-1]->Draw("Same");

			hbins_sig[i-1]->SetLineColor(2);
			hbins_up_sig[i-1]->SetLineColor(2);
			hbins_dn_sig[i-1]->SetLineColor(2);
			hbins_up_sig[i-1]->SetLineStyle(2);
			hbins_dn_sig[i-1]->SetLineStyle(3);
			hbins_sig[i-1]->Draw("Same");
			hbins_up_sig[i-1]->Draw("Same");
			hbins_dn_sig[i-1]->Draw("Same");
			MC_KD[i-1]->SetLineColor(3);
			MC_KD[i-1]->Draw("Samehist");
			data_KD[i-1]->SetLineColor(1);
			data_KD[i-1]->SetMarkerStyle(8);
			data_KD[i-1]->Draw("ESameP");


			double p_bkg=data_KD[i-1]->KolmogorovTest(hbins_DY[i-1]);
			double p_sig=data_KD[i-1]->KolmogorovTest(hbins_sig[i-1]);

			//cout<<p_bkg<<" "<<p_sig<<endl;
			TLatex *t = new TLatex();
			t->SetTextSize(0.02);
			t->SetTextColor(kBlue+2);
			t->DrawLatex(0.65,max_y*0.85,"Kolmogorov Test:");
			t->DrawLatex(0.65,max_y*0.81,Form("p_{bkg}=%f",p_bkg));
			t->DrawLatex(0.65,max_y*0.78,Form("p_{sig}=%f",p_sig));
			t->Draw();

			//   for (int j=1;j<=nbiny;j++) {
			//   cout<< hbins[i-1]->GetBinContent(j) <<","
			//   <<hbins_up[i-1]->GetBinContent(j)<<","
			//   <<hbins_dn[i-1]->GetBinContent(j)<<endl;}

			TLegend* leg = new TLegend(0.4,0.65,0.6,0.85);
			leg->AddEntry(hbins_DY[i-1],Form("%s templ nominal",bkgch),"l");
			leg->AddEntry(hbins_up_DY[i-1],Form("%s templ up",bkgch),"l");
			leg->AddEntry(hbins_dn_DY[i-1],Form("%s templ down",bkgch),"l");
			leg->AddEntry(hbins_sig[i-1],"sig templ nominal","l");
			leg->AddEntry(hbins_up_sig[i-1],"sig templ up","l");
			leg->AddEntry(hbins_dn_sig[i-1],"sig templ down","l");
			leg->AddEntry(MC_KD[i-1],Form("%s MC",bkgch),"l");
			leg->AddEntry(data_KD[i-1],"data","p");
			leg->SetBorderSize(0);
			leg->Draw();

			c2->SaveAs(Form("2l2q2DTemplate/spin%d/1DSlices/spin%d_%s_%s_MH%dto%d_1D.png",spin,spin,cat,bkgch,m_low,m_high));
			delete c2;
		}

	}


	TCanvas* c1=new TCanvas("c1", "c1", 800,800);
	c1->SetFillColor(0);

	//make gradient color
	/*
	   for (double i=2;i<=nbinx+1;i++) {              
	   double r=1-i/(nbinx*2); 
	   double g=i/nbinx;
	   double b=i/nbinx;
	   gROOT->GetColor(i)->SetRGB(r,g,b);
	   }
	   */


	//plot all KD distribution together
	int col[8]={632,801,416,433,600,616,880,1};

	int massbin_bkg[8]={0,0,0,0,0,0,0,0};
	for (int i=1;i<=nbinx;i++) {
		int m_low = xaxis->GetBinLowEdge(i); //find mass low end
		hbins_DY[i-1]->Scale(1/hbins_DY[i-1]->Integral());
		c1->cd();
		if(i==1)hbins_DY[i-1]->Draw();
		double max_y=hbins_DY[i-1]->GetMaximum()*1.2;
		max_y = max_y>0.25?max_y:0.25; 
		hbins_DY[0]->GetYaxis()->SetRangeUser(0,max_y);
		hbins_DY[0]->SetTitle("");
		hbins_DY[0]->GetXaxis()->SetTitle("D_Zjj");
		int massgroup;  
		for (int k=0; k<8; k++){               
			if(m_low>=300+200*k) massgroup=k;
		}
		massbin_bkg[massgroup]=i;
		hbins_DY[i-1]->SetLineWidth(1.5);                               
		hbins_DY[i-1]->SetLineColor(col[massgroup]);
		//cout<<"mass low edge < "<<(massgroup+1)*500<<endl;
		hbins_DY[i-1]->Draw("same");

	}

	TLegend* leg_bkg = new TLegend(0.4,0.65,0.7,0.85);
	for (i=1;i<=8;i++){
		if(massbin_bkg[i-1]!=0){
			if(massbin_bkg[i]!=0) leg_bkg->AddEntry(hbins_DY[massbin_bkg[i-1]-1],Form("%i to %i GeV",300+(i-1)*200,300+i*200),"l");
			if(massbin_bkg[i]==0) leg_bkg->AddEntry(hbins_DY[massbin_bkg[i-1]-1],Form("above %i GeV",300+(i-1)*200),"l");
		}
	}
	leg_bkg->SetBorderSize(0);
	leg_bkg->Draw();

	c1->SaveAs(Form("2l2q2DTemplate/spin%d/1DSlices/spin%d_%s_%s_1D_all.png",spin,spin,cat,bkgch));
	delete c1;

	if(bkgch=="DY"){ 
		TCanvas* c1sig=new TCanvas("c1sig", "c1sig", 800,800);
		c1sig->SetFillColor(0);
		int nbinx_sig=templ2D_sig->GetNbinsX();
		TH1 *sig1D[nbinx_sig];
		int massbin[8]={0,0,0,0,0,0,0,0};
		for (int i=1;i<=nbinx_sig;i++) {
			int m_low = xaxis_sig->GetBinLowEdge(i); 
			int m_high = xaxis_sig->GetBinUpEdge(i); 
			sig1D[i-1] = new TH1F(Form("sig1DHM%dto%d",m_low,m_high),Form("sig1DHM%dto%d",m_low,m_high),nbiny,0,1);
			for (int j=1;j<=nbiny;j++) {
				float v_sig = templ2D_sig->GetBinContent(i,j);
				sig1D[i-1]->SetBinContent(j,v_sig);
			}
			c1sig->cd();
			if(i==1)sig1D[i-1]->Draw();
			double max_y=sig1D[i-1]->GetMaximum()*1.2;
			max_y = max_y>0.25?max_y:0.25;
			sig1D[0]->GetYaxis()->SetRangeUser(0,max_y);
			sig1D[0]->SetTitle("");
			sig1D[0]->GetXaxis()->SetTitle("D_Zjj");
			int massgroup;
			for (int k=0; k<8; k++){
				if(m_low>500*k) massgroup=k;
			}
			massbin[massgroup]=i;
			sig1D[i-1]->SetLineWidth(1.5);
			sig1D[i-1]->SetLineColor(col[massgroup]);
			//cout<<"mass low edge < "<<(massgroup+1)*500<<endl;
			sig1D[i-1]->Draw("same");
		}


		TLegend* leg_sig = new TLegend(0.4,0.65,0.7,0.85);
		for (i=1;i<=8;i++){
			if(massbin[i-1]!=0){
				if(i==1) leg_sig->AddEntry(sig1D[massbin[i-1]-1],"300 to 500 GeV","l");
				else if(massbin[i]!=0) leg_sig->AddEntry(sig1D[massbin[i-1]-1],Form("%i to %i GeV",(i-1)*500,i*500),"l");
				else if(massbin[i]==0) leg_sig->AddEntry(sig1D[massbin[i-1]-1],Form("above %i GeV",(i-1)*500),"l");
			}
		}
		leg_sig->SetBorderSize(0);
		leg_sig->Draw();

		c1sig->SaveAs(Form("2l2q2DTemplate/spin%d/1DSlices/spin%d_%s_sig_1D_all.png",spin,spin,cat));
		delete c1sig;
	}

	TH2F *sig = new TH2F(Form("sig_%s",cat),Form("sig_%s",cat),370,300,4000,30,0,1);
	TH2F *sig_up = new TH2F(Form("sig_%s_up",cat),Form("sig_%s_up",cat),370,300,4000,30,0,1);
	TH2F *sig_dn = new TH2F(Form("sig_%s_dn",cat),Form("sig_%s_dn",cat),370,300,4000,30,0,1);
	TH2F *bkg = new TH2F(Form("%s_%s",bkgch,cat),Form("%s_%s",bkgch,cat),370,300,4000,30,0,1);
	TH2F *bkg_up = new TH2F(Form("%s_%s_up",bkgch,cat),Form("%s_%s_up",bkgch,cat),370,300,4000,30,0,1);
	TH2F *bkg_dn = new TH2F(Form("%s_%s_dn",bkgch,cat),Form("%s_%s_dn",bkgch,cat),370,300,4000,30,0,1);
	for (int i=1;i<=370;i++) {
		for (int j=1;j<=30;j++) {
			double m = 300+10*i-5;
			int massbin=templ2D_DY->GetXaxis()->FindBin(m);
			double KD = hbins_DY[massbin-1]->GetBinContent(j);
			bkg->SetBinContent(i,j,KD);
			double KD_up = hbins_up_DY[massbin-1]->GetBinContent(j);
			bkg_up->SetBinContent(i,j,KD_up);
			double KD_dn = hbins_dn_DY[massbin-1]->GetBinContent(j);
			bkg_dn->SetBinContent(i,j,KD_dn);

			int massbin_sig=templ2D_sig->GetXaxis()->FindBin(m);
			double KD_sig = templ2D_sig->GetBinContent(massbin_sig,j);
			sig->SetBinContent(i,j,KD_sig);
			double KD_sig_up = templ2D_up_sig->GetBinContent(massbin_sig,j);
			sig_up->SetBinContent(i,j,KD_sig_up);
			double KD_sig_dn = templ2D_dn_sig->GetBinContent(massbin_sig,j);
			sig_dn->SetBinContent(i,j,KD_sig_dn);

		}
	}

	TCanvas* c3=new TCanvas("c3", "c3", 800,800);
	c3->SetFillColor(0);
	c3->cd();
	if(bkgch=="DY"){
		sig->Draw("COL");
		c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_sig.png",spin,cat));
		sig_up->Draw("COL");
		c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_sig_up.png",spin,cat));
		sig_dn->Draw("COL");
		c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_sig_dn.png",spin,cat));}

	bkg->Draw("COL");
	c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_%s.png",spin,cat,bkgch));
	bkg_up->Draw("COL");
	c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_%s_up.png",spin,cat,bkgch));
	bkg_dn->Draw("COL");
	c3->SaveAs(Form("2l2q2DTemplate/spin%i_%s_%s_dn.png",spin,cat,bkgch));
	//store

	TFile ftemp (Form("2l2q2DTemplate/2l2q_spin%i_template.root",spin),"update");
	ftemp.cd();
	if(bkgch=="DY"){
		sig->Write();
		sig_up->Write();
		sig_dn->Write();}
	bkg->Write();
	bkg_up->Write();
	bkg_dn->Write();
	ftemp.Close();

	for (int i=1;i<=nbinx;i++) {
		delete hbins_DY[i-1];
		delete hbins_up_DY[i-1];
		delete hbins_dn_DY[i-1];
		delete hbins_sig[i-1];
		delete hbins_up_sig[i-1];
		delete hbins_dn_sig[i-1];
	}
}
void checkTempl_1Dslice_2Dsys(int spin=0){
	char* bkg[3]={"DY","TTbar","Diboson"};	
	   for (int i=0;i<3;i++){
	   make1DSlice(spin,bkg[i],"resolved");
	   make1DSlice(spin,bkg[i],"merged");
	   }	

}

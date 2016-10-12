#include "TMath.h"
#include "TROOT.h"
#include "../interface/ResoEff.h"


void HighMass::plotEfficiency(){

	TString finName=Form("2l2qSelectedTrees/2l2qtree_%s.root",sampleName[sample]);
	TFile *fin; 
	TTree* inTree;
	TH1F* inhgen;
	bool fileExist=(!gSystem->AccessPathName(finName));

	if(fileExist){
		fin = TFile::Open(finName);
		cout<<"Using existing tree!"<<endl;
		inhgen=(TH1F*)fin->Get("hgen");
		inTree = (TTree*)fin->Get("selectedTree");
		inTree->SetBranchAddress("GenHMass",&mGen);
		inTree->SetBranchAddress("weight",&weight);
		inTree->SetBranchAddress("jetType",&typ);
		inTree->SetBranchAddress("lepFlav",&lep);
		inTree->SetBranchAddress("tag",&tag);
		inhgen=(TH1F*)fin->Get("hgen");
	}
	else{ 
		cout<<"Making selected tree!"<<endl;
		HighMass::makeSelectedTree();
		inTree=selTree;
		inhgen=hgen;
	}

	gStyle->SetPadLeftMargin(0.1);
	gStyle->SetPadRightMargin(0.15);
	gStyle->SetOptFit(0000);
	gStyle->SetOptStat(0000);
	gStyle->SetTitleFontSize(0.05);

	// initialize histograms

	const int jetType = 2;
	const int LepFlav = 2;
	const int Tag = 3;
	int col[2][3]={{2,3,4},{634,418,602}};
	const char* prodName[2] ={"ggH","VBF"};
	const char* jetName[2]={"Merged","Resolved"};
	const char* lepName[2]={"eeqq","mumuqq"};
	const char* legName[2]={"eeqq","#mu#muqq"};
	const char* tagName[3]={"vbf-tagged","b-tagged","untagged"};

	TH1F* hreco[jetType][LepFlav][Tag];
	TH1F* heff[jetType][LepFlav][Tag];
	TGraphErrors *gr[jetType][LepFlav][Tag];


	for (int i=0;i<jetType;i++) {
		for (int j=0;j<LepFlav;j++) {
			for (int k=0;k<Tag;k++) {
				hreco[i][j][k] = new TH1F(Form("reco_jet%d_lep%d_tag%d",i,j,k),Form("reco_jet%d_lep%d_tag%d",i,j,k),Mmax,0,Mmax);
				heff[i][j][k] = new TH1F(Form("eff_jet%d_lep%d_tag%d",i,j,k),Form("eff_jet%d_lep%d_tag%d",i,j,k),Mmax,0,Mmax);
			}
		}
	}


	for (int i = 0; i < inTree->GetEntries(); i++) {
		inTree->GetEntry(i);
		//fill histo
		hreco[typ][lep][tag]->Fill(mGen,weight);
		//cout<<i<<" "<<typ<<" "<<lep<<" "<<tag<<" "<<mGen<<" "<<endl;
	}



	// get efficiency with error 

	for (int i=0;i<jetType;i++) {
		for (int j=0;j<LepFlav;j++) {
			for (int k=0;k<Tag;k++) {

				// bin contents of raw histograms
				double M_raw[Mmax];
				double gen_raw[Mmax];
				double reco_raw[Mmax];

				double massE[n], gen[n], genE[n], reco[n], recoE[n], eff[n], effE[n];
				double mass,width;

				for (int bin=1;bin<=Mmax;bin++){
					M_raw[bin-1] = hreco[i][j][k]->GetXaxis()->GetBinCenter(bin);
					gen_raw[bin-1] = inhgen->GetBinContent(bin);
					reco_raw[bin-1] = hreco[i][j][k]->GetBinContent(bin);
					//cout<<bin<<":"<<reco_raw[bin-1]<<","<<gen_raw[bin-1]<<endl;
				}
				for (int b=0;b<n;b++){
					gen[b]=0 ; reco[b]=0;genE[b]=0;recoE[b]=0;eff[b]=0; effE[b]=0;
					mass = M[b];
					if(mass<1000) width=25;
					else if (mass>=1000 && mass<1500) width =50;
					else if (mass>=1500 && mass<2000) width = 100;
					else if (mass>=2000) width = 200;

					//merge
					for (int j=(mass-width);j<(mass+width);j++){
						gen[b]+=gen_raw[j];
						reco[b]+=reco_raw[j];
					}

					genE[b]=sqrt(gen[b]);
					recoE[b]=sqrt(reco[b]);
					// calculate efficiency and efficiency error
					double g,l,r;
					g=gen[b];
					r=reco[b];
					l=(g-r);
					if(g!=0){
						eff[b]=r/g;
						effE[b]=sqrt(l*l*r+r*r*l)/(g*g);
					}
					else {
						eff[b]=0;
						effE[b]=0;
					}
					//cout<<"efficiency ["<<b<<"]="<<eff[b]<<endl;
				}

				gr[i][j][k] = new TGraphErrors (n,M,eff,massE,effE);
			}
		}
	}



	TF1 *polyFunctot_merged= new TF1("polyFunctot_merged",merged_fit,Mmin, Mmax, 9);
	polyFunctot_merged->SetParameters(0.055533, 696.978882, 201.791214, 0.074554, 0.013758, 1489.137451, 272.321777, 0.01, -0.01);
	polyFunctot_merged->SetParLimits(0,.003,.2);
	polyFunctot_merged->SetParLimits(1,500,1100);
	polyFunctot_merged->SetParLimits(2,150,500);
	polyFunctot_merged->SetParLimits(3,0,1);
	polyFunctot_merged->SetParLimits(4,.003,.2);
	polyFunctot_merged->SetParLimits(5,500,1100);
	polyFunctot_merged->SetParLimits(6,150,500);
	polyFunctot_merged->SetParLimits(7,-.02,.02);
	polyFunctot_merged->SetParLimits(8,-.02,.02);


	TF1 *polyFunctot_resolved= new TF1("polyFunctot_resolved",resolved_fit,Mmin, Mmax, 12);
	polyFunctot_resolved->SetParameters(.1, -1.5, 2, 200, 500, .04, 1000, 100, .1, 800, 200);
	polyFunctot_resolved->SetParLimits(0,0,.25);
	polyFunctot_resolved->SetParLimits(1,-2,-1);
	polyFunctot_resolved->SetParLimits(2,1.3,2.5);
	polyFunctot_resolved->SetParLimits(3,50,300);
	polyFunctot_resolved->SetParLimits(4,400,700);
	polyFunctot_resolved->SetParLimits(6,1000,1500);
	polyFunctot_resolved->SetParLimits(7,100,500);
	polyFunctot_resolved->SetParLimits(8,-.05,.05);
	polyFunctot_resolved->SetParLimits(9,100,600);
	polyFunctot_resolved->SetParLimits(10,200,800);
	polyFunctot_resolved->SetParLimits(11,100,1000);

	//plotting
	TString filename="Efficiency_unknown"; 
	filename = Form("efficiency_2l2q_%s.root",sampleName[sample]);

	TFile *fout = new TFile(filename,"recreate");
	fout->cd();
	for (int j=0;j<2;j++){
		TCanvas* c = new TCanvas("c","c", 1600,800);
		c->SetFillColor(0);
		c->SetRightMargin(0.18);
		c->cd();
		TMultiGraph * mg = new TMultiGraph();
		TLegend* leg = new TLegend(.83,0.3,0.99,.85);
		leg->SetFillColor(0);
		leg->SetBorderSize(0);

		int tag_start=0;
		if (sample==spin2) tag_start=1; //skip VBF for spin2

		for (int i=0;i<jetType;i++) {
			TF1 *polyFunctot = i?polyFunctot_resolved:polyFunctot_merged;
			const char * polyFunctot_name = i?"polyFunctot_resolved":"polyFunctot_merged";
			for (int k=tag_start;k<Tag;k++) {
				if (i==0) gr[i][j][k]->SetMarkerStyle(24);
				else if (i==1) gr[i][j][k]->SetMarkerStyle(20);
				gr[i][j][k]->SetMarkerSize(1);
				gr[i][j][k]->SetMarkerColor(col[i][k]);
				gr[i][j][k]->SetLineColor(col[i][k]);
				gr[i][j][k]->SetLineWidth(2);
				gr[i][j][k]->SetLineStyle(2);
				polyFunctot->SetLineColor(col[i][k]);
				gr[i][j][k]->Fit(polyFunctot_name);
				leg->AddEntry(gr[i][j][k],Form("%s_%s_%s",legName[j],jetName[i],tagName[k]),"pl");

				gr[i][j][k]->SetName(Form("%s_%s_%s_%s",sampleName[sample],lepName[j],jetName[i],tagName[k]));

				//write to file
				mg->Add(gr[i][j][k]);gr[i][j][k]->Write();

				//save parameters to file
				char * param_txt = Form("efficiency_%s_%s_%s_%s.txt",sampleName[sample],lepName[j],jetName[i],tagName[k]);

				FILE *fp = fopen(param_txt,"w");
				if (fp!=NULL) {
					for (int i=0;i<polyFunctot->GetNpar();i++) {
						Float_t value = polyFunctot->GetParameter(i);
						printf("p%d\t%f\n",i,value);
					}
					fprintf(fp,"%s", "polyFunctot->SetParameters(");
					for (int i=0;i<polyFunctot->GetNpar();i++) {
						Float_t value = polyFunctot->GetParameter(i);
						fprintf(fp,"%s%f", (i==0)?"":", ", value);
					}
					fprintf(fp, ")\n");
					for (int i=0;i<polyFunctot->GetNpar();i++) {
						Float_t value = polyFunctot->GetParameter(i);
						fprintf(fp,"\np%d\t%f",i,value);
					}
				}
				//                                  fp->close();
			}
		}
		mg->Draw("APL"); //for multigraph, must draw before change title/range
		mg->GetXaxis()->SetTitle("genHMass [GeV]");
		mg->GetYaxis()->SetTitle("efficiency*acceptance");
		mg->GetYaxis()->SetRangeUser(0,0.25);
		if (sample==spin2) mg->GetYaxis()->SetRangeUser(0,0.3);
		mg->GetXaxis()->SetRangeUser(Mmin,Mmax);
		leg->Draw();
		c->Update();
		c->SaveAs(Form("efficiency_%s_all_%s.png",sampleName[sample],lepName[j]));
		c->SaveAs(Form("efficiency_%s_all_%s.pdf",sampleName[sample],lepName[j]));
	}
	fout->Close();
}

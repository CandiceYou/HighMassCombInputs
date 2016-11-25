#include "TMath.h"
#include "TROOT.h"
#include "../interface/ResoEff.h"

void HighMass::readResoDataset(const char* cType){

        TString finName=Form("../2l2qSelectedTrees/2l2qtree_%s.root",sampleName[sample]);
	TFile *fin;
	TTree* inTree;
	bool fileExist=(!gSystem->AccessPathName(finName));

	if(fileExist){
		fin = TFile::Open(finName);
		cout<<"Using existing tree!"<<endl;
		inTree = (TTree*)fin->Get("selectedTree");
		inTree->SetBranchAddress("ZZMass",&m2l2q);
		inTree->SetBranchAddress("GenHMass",&mGen);
		inTree->SetBranchAddress("weight",&weight);
		inTree->SetBranchAddress("jetType",&typ);
		inTree->SetBranchAddress("lepFlav",&lep);
		inTree->SetBranchAddress("tag",&tag);
	}
	else{
		cout<<"Making selected tree!"<<endl;
		HighMass::makeSelectedTree("");
		inTree=selTree;
	}


	for (int i=0; i<maxMassBin; i++) {
		sprintf(tempmass,"mh%d",massBin[i]);
		massrc.defineType(tempmass,massBin[i]);
		width[i] = 3.21246+0.0312538*(massBin[i])-7.29127e-07*(massBin[i])*(massBin[i]);
		xMin[i] = width[i]*(-25);
		xMax[i] = width[i]*(25);
		yMin[i] = width[i]*(-25)+massBin[i];
		yMax[i] = width[i]*(25)+massBin[i];
	}

	RooArgSet ntupleVarSet = RooArgSet(x,y,w,massrc);
	RooDataSet* dataset = new RooDataSet("resoM","resoM",ntupleVarSet,WeightVar("myW"));
	RooDataSet* dataset_sub[100];

	for (int k = 0; k < inTree->GetEntries(); k++) {
		inTree->GetEntry(k);
		//cout<<typ<<" "<<mGen<<endl;
		for (int i=0; i<maxMassBin; i++) {

			if (strcmp(cType,"resolved")==0 && ((massBin[i]>=1200) && ((m2l2q-mGen)<-500))) continue;

			if (((typ==0 && strcmp(cType,"merged")==0)||(typ==1&&strcmp(cType,"resolved")==0)) && mGen>(massBin[i]*0.9) && mGen<(massBin[i]*1.1)) {
				ntupleVarSet.setCatIndex("massrc",massBin[i]);
				ntupleVarSet.setRealValue("reso",m2l2q-mGen);
				ntupleVarSet.setRealValue("reco",m2l2q-mGen+massBin[i]);
				ntupleVarSet.setRealValue("myW",weight);
				/*
				   cout<<massBin[i]<<endl;
				   massrc.Print();
				   cout<<m2l2q-mGen<<endl;
				   x.Print();
				   cout<<weight<<endl;
				   w.Print();
				   */
				dataset->add(ntupleVarSet,weight);

			}
		}


	}


	cout << "dataset n entries: " << dataset->sumEntries() << endl;

	TFile *fout = new TFile(Form("resoDataset_%s.root",cType),"recreate");
	fout->cd();

	dataset->Write("alldata");

	for (int i=0; i<maxMassBin; i++) {
		sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);
		dataset_sub[i]= (RooDataSet*)dataset->reduce(tempmass2);
		cout << "dataset_sub["<<i<<"] n entries: " << dataset_sub[i]->sumEntries() << endl;
		dataset_sub[i]->Write(Form("resoDataset_mh%d",massBin[i]));
	}
	fout->Close();

}

void HighMass::fitReso(const char* cType){


	for (int i=0; i<maxMassBin; i++) {
		sprintf(tempmass,"mh%d",massBin[i]);
		massrc.defineType(tempmass,massBin[i]);
		width[i] = 3.21246+0.0312538*(massBin[i])-7.29127e-07*(massBin[i])*(massBin[i]);
		xMin[i] = width[i]*(-25);
		xMax[i] = width[i]*(25);
		yMin[i] = width[i]*(-25)+massBin[i];
		yMax[i] = width[i]*(25)+massBin[i];
	}

	TFile *fin = TFile::Open(Form("resoDataset_%s.root",cType));
	RooDataSet* dataset = (RooDataSet*)fin->Get("alldata");
	cout << "dataset n entries: " << dataset->sumEntries() << endl;


	//fitting

	//	RooDoubleCB* DCBall[100];
	RooRealVar* mean_ind[100];
	RooRealVar* sigma_ind[100];
	RooRealVar* a1_ind[100];
	RooRealVar* a2_ind[100];
	RooRealVar* n1_ind[100];
	RooRealVar* n2_ind[100];
	RooFitResult* fitres[100];
	RooDataSet* dataset_sub[100];


	for (int i=0; i<maxMassBin; i++) {
		char formulamass[200];

		mean_ind[i]= new RooRealVar("mean_CB","mean_CB",0.,-200., 200.) ;
		//    mean_ind[i]= new RooRealVar("mean_CB","mean_CB",massBin[i],0., 4000.) ;
		sigma_ind[i]= new RooRealVar("sigma_CB","sigma_CB",1, 0, 500);
		a1_ind[i]= new RooRealVar("a1_CB","a1_CB", 1., 0, 5.);
		n1_ind[i]= new RooRealVar("n1_CB","n1_CB", 1., 0, 10.);
		a2_ind[i]= new RooRealVar("a2_CB","a2_CB", 1., 0, 5.);
		n2_ind[i]= new RooRealVar("n2_CB","n2_CB", 1., 0, 5.);

		sprintf(tempmass,"mh%d",massBin[i]);
		sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);
		DCBall[i] = new RooDoubleCB("DCBall","Double Crystal ball",x,*mean_ind[i],*sigma_ind[i],*a1_ind[i],*n1_ind[i],*a2_ind[i],*n2_ind[i]);
		cout << "Individual fit, mass: "<<massBin[i]<<" , range: [" << xMin[i] << " , " << xMax[i] << "]." << endl<<endl;

		dataset_sub[i]= (RooDataSet*)fin->Get(Form("resoDataset_mh%d",massBin[i]));

		cout << "dataset_sub["<<i<<"] n entries: " << dataset_sub[i]->sumEntries() << endl;

		fitres[i] = (RooFitResult*)DCBall[i]->fitTo(*dataset_sub[i],Range(xMin[i],xMax[i]));
		//    fitres[i] = (RooFitResult*)DCBall[i]->fitTo(*dataset_sub[i],SumW2Error(1),Range(xMin[i],xMax[i]),Strategy(2),NumCPU(8),Save(true));

		RooArgSet * params = DCBall[i]->getParameters(x);
		char paramfilename[100];
		sprintf(paramfilename,"singleMassFit/%s/SingleMassFit_ResoParam_MH%d_%s.txt",cType,massBin[i],cType);
		params->writeToFile(paramfilename) ;

		TCanvas *c1 = new TCanvas("c1","c1",725,725);
		TPad *pad1 = new TPad("pad1","fit",0.05,0.35,0.95,0.97);
		pad1->Draw();
		TPad *pad2 = new TPad("pad2","pull",0.05,0.02,0.95,0.35);
		pad2->Draw();

		int col = kRed+3;;

		char framename[100];
		sprintf(framename,"Resolution_MH%dGeV_%s",massBin[i],cType);

		RooPlot* xframe = x.frame(Range(xMin[i],xMax[i]),Bins(100),Title(framename)) ;
		xframe->GetYaxis()->SetTitleOffset(1.5);
		dataset_sub[i]->plotOn(xframe,DataError(RooAbsData::SumW2), MarkerStyle(kOpenCircle), MarkerSize(1.1));
		DCBall[i]->plotOn(xframe,LineColor(col),Slice(massrc,tempmass),ProjWData(massrc,*dataset));

		RooHist* hpull = xframe->pullHist();
		RooPlot* frame2 = x.frame(Range(xMin[i],xMax[i]),Title("Pull Distribution")) ;
		frame2->addPlotable(hpull,"P");

		c1->cd();
		pad1->cd();
		xframe->Draw();

		pad2->cd() ;
		frame2->Draw() ;
		frame2->SetMinimum(-10);
		frame2->SetMaximum(10);
		char filename1[100];
		char filename2[100];
		sprintf(filename1,"singleMassFit/%s/Resolution_MH%d_%s_singleMassFit.png",cType,massBin[i],cType);
		sprintf(filename2,"singleMassFit/%s/Resolution_MH%d_%s_singleMassFit.pdf",cType,massBin[i],cType);
		c1->SaveAs(filename1);
		c1->SaveAs(filename2);

	}

}


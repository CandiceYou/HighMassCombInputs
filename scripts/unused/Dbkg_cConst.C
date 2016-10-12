void Dbkg_cConst(int spin=0){

	const int maxEvents=-1; //max events per sample
	//output plots
	TString outdir=Form("~/www/HighMass/160927/c_2l2q_spin%d_old/",spin);
	//output c-constant TH1F
	TString coutput = Form("Dbkg_spin%d",spin);
	coutput.Append(".root");


	TChain* sig_tree=new TChain("sig_tree");
	sig_tree->Add(Form("spin%d_tree.root",spin));
	TChain* bkg_tree=new TChain("bkg_tree");
	bkg_tree->Add(Form("spin%d_tree.root",spin));


	float x=0,KD=0,weight=0;

	sig_tree->SetBranchAddress("ZZMassRefit",&x);
	sig_tree->SetBranchAddress("D_zjets",&KD);
	sig_tree->SetBranchAddress("weight",&weight);

	bkg_tree->SetBranchAddress("ZZMassRefit",&x);
	bkg_tree->SetBranchAddress("D_zjets",&KD);
	bkg_tree->SetBranchAddress("weight",&weight);


	// Mass bins : 300-3000 in 100 GeV steps
        const int bin_width=100;
	const int nbins = (3000.-300)/bin_width;
	TH1F* Dbkg[nbins];
	TH1F* Dsig[nbins];
	const int KDsteps=100; // # increment steps of D_bkg

	for (int bin=0; bin<nbins; bin++){
		Dsig[bin]=new TH1F(Form("sig_bin%d",bin),Form("sig_bin%d",bin),KDsteps,0,1);
		Dbkg[bin]=new TH1F(Form("bkg_bin%d",bin),Form("bkg_bin%d",bin),KDsteps,0,1);
	}

	//######### Read Tree ############

	int nEvents_sig, nEvents_bkg;

	if (maxEvents==-1){
            nEvents_sig=sig_tree->GetEntries();
            nEvents_bkg=bkg_tree->GetEntries();}
        else{
             nEvents_sig= maxEvents<sig_tree->GetEntries() ? maxEvents:sig_tree->GetEntries();
             nEvents_bkg= maxEvents<bkg_tree->GetEntries() ? maxEvents:bkg_tree->GetEntries();}
        
	for (int ev=0; ev<nEvents_sig; ev++){
		sig_tree->GetEntry(ev);
		int bin = (x-300.)/bin_width;
		if (bin<0 || bin>=nbins) continue;
                if (weight >=0)
		Dsig[bin]->Fill(KD,weight); 
                //if(bin==26)cout<<"wt_sig="<<weight<<endl;
	}


        for (int ev=0; ev<nEvents_bkg; ev++){
                bkg_tree->GetEntry(ev);
                int bin = (x-300.)/bin_width;
                if (bin<0 || bin>=nbins) continue;
                if (weight>=0)
                Dbkg[bin]->Fill(KD,weight);
                //if(bin==26)cout<<"wt_bkg="<<weight<<endl;
        }

	cout << "Filling histo done!" <<endl<<endl;

	delete sig_tree;
	delete bkg_tree;

	//##########################

	TFile* foutput = new TFile(coutput, "recreate");
	TH1F* histo = new TH1F("k", "", nbins, 300, 3000);
	TH1F* histo_c = new TH1F("c", "", nbins, 300, 3000);

	for (int bin=0; bin<nbins; bin++){
		cout << "Checking bin " << bin << endl;

		double k=0; //closed approach
		double diffFrac=100;
		double nsig=Dsig[bin]->GetEntries();
		double nbkg=Dbkg[bin]->GetEntries();
		double int_sig=Dsig[bin]->Integral();
		double int_bkg=Dbkg[bin]->Integral();
		Dsig[bin]->Scale(1/int_sig);
		Dbkg[bin]->Scale(1/int_bkg);

		if (nsig==0 || nbkg==0) continue;

		for (int iKD=1; iKD<KDsteps; iKD+=1){

			double f_sig=0,f_bkg=0;
			f_sig =Dsig[bin]->Integral(0,iKD)/Dsig[bin]->Integral();
			f_bkg =Dbkg[bin]->Integral(iKD,KDsteps)/Dbkg[bin]->Integral();

			if (fabs(f_sig-f_bkg)<diffFrac){
				diffFrac = fabs(f_sig-f_bkg);
				k = (double(iKD))/(double(KDsteps));  
			}
//			if (bin==26) cout<<"iKD="<<iKD<<", fbkg="<<f_bkg<<", fsig="<<f_sig<<", diffFrac="<<fabs(f_sig-f_bkg)<<endl;
		}

		double c_const = k/ (1-k);
		double k_error=0, c_error=0;
		if (k!=0){ 
			k_error= 1/sqrt(nsig*nbkg); 
			c_error = (1/((1-k)*(1-k))) * k_error;}
		cout <<"min_diffFrac=" <<diffFrac <<endl;
		cout << "k=" << k <<",error="<<k_error<<endl;
		cout << "c constant=" <<c_const <<", error="<<c_error <<endl;

		histo->SetBinContent(bin+1, k);
		histo->SetBinError(bin+1, k_error);
		histo_c->SetBinContent(bin+1, c_const);
		histo_c->SetBinError(bin+1, c_error);

		foutput->WriteTObject(Dbkg[bin]);
		foutput->WriteTObject(Dsig[bin]);

		TCanvas* c = new TCanvas("c","c", 1600,800);
		c->SetFillColor(0);
		c->SetRightMargin(0.18);
		Dbkg[bin]->SetLineColor(1);
		Dsig[bin]->SetLineColor(2);
		double max1=Dbkg[bin]->GetMaximum();
		double max2=Dsig[bin]->GetMaximum();
		double max = max1>max2?max1:max2;
		Dbkg[bin]->GetYaxis()->SetRangeUser(0,max*1.3);
		Dbkg[bin]->Draw("HIST");
		Dsig[bin]->Draw("SAMEHIST");
		int mass = 300+(bin+0.5)*(bin_width);
		TString plotname=outdir + Form("2l2qDbkg_MH%d.png",mass);
		c->SaveAs(plotname);
	}

	foutput->WriteTObject(histo);
	foutput->WriteTObject(histo_c);

	TCanvas* c1 = new TCanvas("c1","c1", 1600,800);
	histo->Draw("E1");
	TString plotk = outdir + "histo_k.png";
	c1->SaveAs(plotk);

	TCanvas* c2 = new TCanvas("c2","c2", 1600,800);
	histo_c->Draw("E1");
	TString plotc = outdir + "histo_c.png";
	c2->SaveAs(plotc);
	foutput->Close();
}

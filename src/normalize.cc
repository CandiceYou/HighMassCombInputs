
TH2F* normalize(TH2F* histSm, bool rebin=0, float stdMass=1000){

	gStyle->SetPadLeftMargin(0.1);
	gStyle->SetPadRightMargin(0.15);
	gStyle->SetOptFit(0000);
	gStyle->SetOptStat(0000);
	gStyle->SetTitleFontSize(0.05);


	const int nbinx=370, nbiny=30 ,Xmin=300 , Xmax=4000;
	float width = (Xmax-Xmin)/nbinx;

	//mass 1D slice
	TH1 *hbins[nbinx] ;
	float sum=0;
	std::vector<int> bins;
	for (int i=1;i<=nbinx;i++) {
		hbins[i-1] = new TH1F(Form("hist%d",i),Form("hist%d",i),nbiny,0,1);
		for (int j=1;j<=nbiny;j++) {
			float v = histSm->GetBinContent(i,j);
			hbins[i-1]->SetBinContent(j,v);
		}
	}   

	//determine bin edge

	float binsize=0;

	int stdBin = ((stdMass-Xmin)/width);
	for (int i=stdBin-5; i<stdBin+5; i++){       
		binsize +=hbins[i]->Integral(); }//use 10 bins avg binsize around standard bin as min cut
	if (rebin==0) binsize=0;
	cout<<"cut on binsize ="<<binsize<<endl;

        bins.push_back(1); //lower edge of first bin
	for (int i=1;i<=nbinx;i++) {
		if (i!=1 && hbins[i-1]->Integral()>= binsize){ 
			//cout<<"bin edge: "<<i<<",bin size: "<<hbins[i-1]->Integral()<<endl;
			bins.push_back(i);
		}
		else if (hbins[i-1]->Integral()< binsize) sum+=hbins[i-1]->Integral();
		if (sum>= binsize){ 
			sum=0;
			//cout<<"bin edge: "<<i<<endl;
			bins.push_back(i);
		}
                delete hbins[i-1];
	}
	if (sum< (binsize/2)) bins.pop_back(); //if last bin too small, merge last two bins
	bins.push_back(nbinx+1); //upper edge of last bin

	//get new binning
        cout<<"binning:"<<endl;
        double xbins[bins.size()];
        for (int k=0;k<bins.size();k++) {
                double bin_low=(bins.at(k)-1)*width+Xmin;
                xbins[k]=bin_low;
                cout<<bin_low; if(k!=bins.size()-1)cout<<",";
        }
        cout<<endl;

        //rebinned 1D histo
	TH1 *hbins_rebin[bins.size()] ;
	float tmp[nbiny]; 

	for (int k=0;k<bins.size()-1;k++) { //#bins = bins.size()-1
		hbins_rebin[k] = new TH1F(Form("hist%d_rebin",k),Form("hist%d_rebin",k),nbiny,0,1);
		for (int j=1;j<=nbiny;j++) {
			tmp[j-1]=0;
			for (int i=bins.at(k);i<=bins.at(k+1);i++) {
				tmp[j-1] += histSm->GetBinContent(i,j)/(bins.at(k+1)-bins.at(k)+1);
			}
			hbins_rebin[k]->SetBinContent(j,tmp[j-1]);
		}
                        hbins_rebin[k]->Scale(1/hbins_rebin[k]->Integral());
	}


        //fill 2D template with variable binning
        TH2F *histn2 = new TH2F("histn2","histn2",bins.size()-1,xbins,nbiny,0,1);
              //cout<<histn2->GetNbinsX() <<" "<<bins.size()-1;
        for (int i=1;i<=bins.size()-1;i++) {
                for (int j=1;j<=nbiny;j++) {
                        int bin = histn2->GetBin(i,j);
                        float v = hbins_rebin[i-1]->GetBinContent(j);
                        histn2->SetBinContent(bin,v);
                }
              //cout<<histn2->GetXaxis()->GetBinLowEdge(i)<<"-"<<histn2->GetXaxis()->GetBinUpEdge(i)<<endl;
              delete hbins_rebin[i-1];
        }

        return histn2;

/*
	//refill 1D slices and normalize
	TH1 *hbins_norm[nbinx] ;

	for (int i=1;i<=nbinx;i++) {
		hbins_norm[i-1] = new TH1F(Form("hist%d_norm",i),Form("hist%d_norm",i),nbiny,0,1);
		int bin=0;
		for(int k=0;k<bins.size()-1;k++) {
			if (i>=bins.at(k) && i<bins.at(k+1)) bin=k;
		}
		//cout<<i<<" "<<bin+1<<" "<<bins.at(bin)<<endl;
		for (int j=1;j<=nbiny;j++) {
			float v = hbins_rebin[bin]->GetBinContent(j);
			hbins_norm[i-1]->SetBinContent(j,v);
		}

		if (hbins_norm[i-1]->Integral()!=0)
			hbins_norm[i-1]->Scale(1/hbins_norm[i-1]->Integral());

	}

        //assign back to TH2F
        TH2F *histn = new TH2F("histn","histn",nbinx,Xmin,Xmax,nbiny,0,1);
        for (int i=1;i<=nbinx;i++) {
                for (int j=1;j<=nbiny;j++) {
                        int bin = histn->GetBin(i,j);
                        float v = hbins_norm[i-1]->GetBinContent(j);
                        histn->SetBinContent(bin,v);
                }
                delete hbins_norm[i-1];
        }

        return histn;
*/
}



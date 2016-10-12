
TH2F* normalize(TH2F* histSm, bool mergeHM=0 , int HMstart=1000){
	gStyle->SetPadLeftMargin(0.1);
	gStyle->SetPadRightMargin(0.15);
	gStyle->SetOptFit(0000);
	gStyle->SetOptStat(0000);
	gStyle->SetTitleFontSize(0.05);


        const int nbinx=370, nbiny=30 ,Xmin=300 , Xmax=4000;
        float width = (Xmax-Xmin)/nbinx;

        //slices of mass
	TH1 *hbins[nbinx] ;

        //merge high mass bins when statics is low
        int HMstartBin = ((HMstart-Xmin)/width)+1;
        int HMbins = (Xmax-HMstart)/width;
	float highmassBin[30];
	for (int j=1;j<=nbiny;j++) {
		highmassBin[j-1]=0;
		for (int i=HMstartBin;i<=nbinx;i++) {
			highmassBin[j-1] += histSm->GetBinContent(i,j)/HMbins;
		}
	} 

        //normalize each mass slice TH1F
	for (int i=1;i<=nbinx;i++) {
		hbins[i-1] = new TH1F(Form("hist%d",i),Form("hist%d",i),nbiny,0,1);
		for (int j=1;j<=nbiny;j++) {
			float v = histSm->GetBinContent(i,j);
			if (i>=HMstartBin && mergeHM==1)hbins[i-1]->SetBinContent(j,highmassBin[j-1]);
                        else hbins[i-1]->SetBinContent(j,v);
		}
		if (hbins[i-1]->Integral()!=0)
			hbins[i-1]->Scale(1/hbins[i-1]->Integral());
	}

        //assign back to TH2F
        TH2F *histn = new TH2F("histn","histn",nbinx,Xmin,Xmax,nbiny,0,1);
	for (int i=1;i<=nbinx;i++) {
		for (int j=1;j<=nbiny;j++) {
			int bin = histn->GetBin(i,j);
			float v = hbins[i-1]->GetBinContent(j);
			histn->SetBinContent(bin,v); 
		}
        delete hbins[i-1];
	}

        return histn;
	delete histn; 

}



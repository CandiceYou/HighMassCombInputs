void indfit(char* channel="4e"){
  for (int i=0; i<maxMassBin; i++) {
    char formulamass[200];

    mean_ind[i]= new RooRealVar("mean_CB","mean_CB",0.,-50., 1.) ;
    sigma_ind[i]= new RooRealVar("sigma_CB","sigma_CB",1, 0, 500);
    a1_ind[i]= new RooRealVar("a1_CB","a1_CB", 1., 0, 5.);
    n1_ind[i]= new RooRealVar("n1_CB","n1_CB", 1., 0, 5.);
    a2_ind[i]= new RooRealVar("a2_CB","a2_CB", 1., 0, 5.);
    n2_ind[i]= new RooRealVar("n2_CB","n2_CB", 1., 0, 20.);


    sprintf(tempmass,"mh%d",massBin[i]);
    sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);
    DCBall[i] = new RooDoubleCB("DCBall","Double Crystal ball",x,*mean_ind[i],*sigma_ind[i],*a1_ind[i],*n1_ind[i],*a2_ind[i],*n2_ind[i]);
    cout << "Individual fit, mass: "<<massBin[i]<<" , range: [" << xMin[i] << " , " << xMax[i] << "]." << endl<<endl;

    fitres[i] = (RooFitResult*)DCBall[i]->fitTo(*dataset_sub[i],Range(xMin[i],xMax[i]));
//    fitres[i] = (RooFitResult*)DCBall[i]->fitTo(*dataset_sub[i],SumW2Error(1),Range(xMin[i],xMax[i]),Strategy(2),NumCPU(8),Save(true));
    RooArgSet * params = DCBall[i]->getParameters(x);
    char paramfilename[100];
    sprintf(paramfilename,"singleMassFit/SingleMassFit_ResoParam_MH%d_%s.txt",massBin[i],channel);
    params->writeToFile(paramfilename) ;

    TCanvas *c1 = new TCanvas("c1","c1",725,725);
    TPad *pad1 = new TPad("pad1","fit",0.05,0.35,0.95,0.97);
    pad1->Draw();
    TPad *pad2 = new TPad("pad2","pull",0.05,0.02,0.95,0.35);
    pad2->Draw();

    int col;
    if(channel =="4mu") col=kOrange+7;
    if(channel =="4e") col=kAzure+2;
    if(channel =="2e2mu") col=kGreen+3;

    char framename[100];
    sprintf(framename,"Resolution_MH%dGeV_%s",massBin[i],channel);

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
    char filename[100];
    sprintf(filename,"singleMassFit/Resolution_MH%d_%s_singleMassFit.png",massBin[i],channel);
    c1->SaveAs(filename);
    sprintf(filename,"singleMassFit/Resolution_MH%d_%s_singleMassFit.pdf",massBin[i],channel);
    c1->SaveAs(filename);
    }
}

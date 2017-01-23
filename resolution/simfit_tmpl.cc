#include "fitFunction.c"
using namespace RooFit;

void simfit(char* channel="4e"){
  bool fixParam = true;

  RooSimultaneous rs("rs","rs",massrc);
//define piecewise fit
  const int npars=7;
  TF1* pcfit = new TF1("pcfit", fitFunction, 0, 4000, npars);

  double a1_initParam[npars]={<a1_param>};
  double a2_initParam[npars]={<a2_param>};
  double n1_initParam[npars]={<n1_param>};
  double n2_initParam[npars]={<n2_param>};
  double mean_initParam[npars]={<mean_param>};
  double sigma_initParam[npars]={<sigma_param>};

//bind
  RooRealVar* a1_pars[npars];
  RooRealVar* a2_pars[npars];
  RooRealVar* n1_pars[npars];
  RooRealVar* n2_pars[npars];
  RooRealVar* mean_pars[npars];
  RooRealVar* sigma_pars[npars];

  RooArgList obsList[100],a1_parList,a2_parList,n1_parList,n2_parList,mean_parList,sigma_parList;

  double frac=0.1;

  for (int ip=0; ip<npars; ip++){
  a1_pars[ip] = new RooRealVar(Form("a1_bindFunction_%i", ip), "", a1_initParam[ip],a1_initParam[ip]-frac*fabs(a1_initParam[ip])-1.0e-07,a1_initParam[ip]+frac*fabs(a1_initParam[ip])+1.0e-07);
  n1_pars[ip] = new RooRealVar(Form("n1_bindFunction_%i", ip), "", n1_initParam[ip],n1_initParam[ip]*-frac*fabs(n1_initParam[ip])-1.0e-07,n1_initParam[ip]+frac*fabs(n1_initParam[ip])+1.0e-07);
  a2_pars[ip] = new RooRealVar(Form("a2_bindFunction_%i", ip), "", a2_initParam[ip],a2_initParam[ip]-frac*fabs(a2_initParam[ip])-1.0e-07,a2_initParam[ip]+frac*fabs(a2_initParam[ip])+1.0e-07);
  n2_pars[ip] = new RooRealVar(Form("n2_bindFunction_%i", ip), "", n2_initParam[ip],n2_initParam[ip]-frac*fabs(n2_initParam[ip])-1.0e-07,n2_initParam[ip]+frac*fabs(n2_initParam[ip])+1.0e-07);
  mean_pars[ip] = new RooRealVar(Form("mean_bindFunction_%i", ip), "", mean_initParam[ip],mean_initParam[ip]-frac*fabs(mean_initParam[ip])-1.0e-07,mean_initParam[ip]+frac*fabs(mean_initParam[ip])+1.0e-07);
  sigma_pars[ip] = new RooRealVar(Form("sigma_bindFunction_%i", ip), "", sigma_initParam[ip],sigma_initParam[ip]-frac*fabs(sigma_initParam[ip])-1.0e-07,sigma_initParam[ip]+frac*fabs(sigma_initParam[ip])+1.0e-07);

  a1_parList.add(*(a1_pars[ip]));
  a2_parList.add(*(a2_pars[ip]));
  n1_parList.add(*(n1_pars[ip]));
  n2_parList.add(*(n2_pars[ip]));
  mean_parList.add(*(mean_pars[ip]));
  sigma_parList.add(*(sigma_pars[ip]));
  }

  for (int ip=0; ip<2; ip++){
   a1_pars[ip]->setConstant(kTRUE);
   a2_pars[ip]->setConstant(kTRUE);
   n1_pars[ip]->setConstant(kTRUE);
   n2_pars[ip]->setConstant(kTRUE);
   sigma_pars[ip]->setConstant(kTRUE);
   mean_pars[ip]->setConstant(kTRUE);
  }

  if (fixParam==true){
   for (int ip=0; ip<npars; ip++){
    a1_pars[ip]->setConstant(kTRUE);
    a2_pars[ip]->setConstant(kTRUE);
    n1_pars[ip]->setConstant(kTRUE);
    n2_pars[ip]->setConstant(kTRUE);
    sigma_pars[ip]->setConstant(kTRUE);
    mean_pars[ip]->setConstant(kTRUE);
   }
  }

 
  RooRealVar* MH[100];
  RooTFnBinding* pa1[100];
  RooTFnBinding* pa2[100];
  RooTFnBinding* pn1[100];
  RooTFnBinding* pn2[100];
  RooTFnBinding* pmean[100];
  RooTFnBinding* psigma[100];

  for (int i=0; i<maxMassBin; i++) {
    char formulamass[200];

// single mass fit
    mean_ind[i]= new RooRealVar("mean_CB","mean_CB",0.,-15., 1.) ;
    sigma_ind[i]= new RooRealVar("sigma_CB","sigma_CB",1, 0, 150);
    a1_ind[i]= new RooRealVar("a1_CB","a1_CB", 1., 0, 5.);
    n1_ind[i]= new RooRealVar("n1_CB","n1_CB", 1., 0, 5.);
    a2_ind[i]= new RooRealVar("a2_CB","a2_CB", 1., 0, 5.);
    n2_ind[i]= new RooRealVar("n2_CB","n2_CB", 1., 0, 20.);

// fit with piece-wise function from Ulascan
    MH[i]= new RooRealVar("MH","MH",0,0,3000);
    MH[i]->setVal(massBin[i]);
    MH[i]->setConstant(kTRUE);
    obsList[i].add(*MH[i]);
    pa1[i] = new RooTFnBinding("bound_fcn_a1", "", pcfit, obsList[i], a1_parList);
    pa2[i] = new RooTFnBinding("bound_fcn_a2", "", pcfit, obsList[i], a2_parList);
    pn1[i] = new RooTFnBinding("bound_fcn_n1", "", pcfit, obsList[i], n1_parList);
    pn2[i] = new RooTFnBinding("bound_fcn_n2", "", pcfit, obsList[i], n2_parList);
    pmean[i] = new RooTFnBinding("bound_fcn_mean", "", pcfit, obsList[i], mean_parList);
    psigma[i] = new RooTFnBinding("bound_fcn_sigma", "", pcfit, obsList[i], sigma_parList);
  
    sprintf(tempmass,"mh%d",massBin[i]);
    sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);
 
    char fitrange[50];
    sprintf(fitrange,"fitRange_mh%d",massBin[i]);
    x.setRange(fitrange,xMin[i],xMax[i]) ;

    DCBall[i] = new RooDoubleCB("DCBall","Double Crystal ball",x,*pmean[i],*psigma[i],*pa1[i],*pn1[i],*pa2[i],*pn2[i]);
    rs.addPdf(*DCBall[i], tempmass);

    }

    RooFitResult *fitressim = (RooFitResult*)rs.fitTo(*dataset,Strategy(2),Range("fitRange"),SplitRange());
    RooArgSet * params2 = rs.getParameters(RooArgList(x,massrc));
    char paramfilename[100];
    if (fixParam==true)
    sprintf(paramfilename,"fixParamFit/fix_param_%s.txt",channel);
    else
    sprintf(paramfilename,"simfit/simfit_param_%s.txt",channel);
    params2->writeToFile(paramfilename) ;

  for (int i=0; i<maxMassBin; i++) {

    char tempmass[100];
    sprintf(tempmass,"mh%d",massBin[i]);
    char tempmass2[100];
    sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);

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

    dataset->plotOn(xframe,DataError(RooAbsData::SumW2), MarkerStyle(kOpenCircle), MarkerSize(1.1), Cut(tempmass2) );
    rs.plotOn(xframe,LineColor(col),Slice(massrc,tempmass),ProjWData(massrc,*dataset));

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
    if (fixParam==true)
    sprintf(filename,"fixParamFit/Resolution_MH%d_%s_fixParam.pdf",massBin[i],channel);
    else
    sprintf(filename,"simfit/Resolution_MH%d_%s_simfit.pdf",massBin[i],channel);
    c1->SaveAs(filename);
    }
 } 

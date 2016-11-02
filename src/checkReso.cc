#include "TMath.h"
#include "TROOT.h"
#include "../interface/ResoEff.h"
using namespace RooFit;

void HighMass::checkReso(const char* cType){

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


  bool fixParam = true;

  RooSimultaneous rs("rs","rs",massrc);
  RooSimultaneous rs_shift("rs_shift","rs_shift",massrc);

//define piecewise fit
  const int npars=7;
  TF1* pcfit = new TF1("pcfit", fitFunction, 0, 5000, npars);
  TF1* pcfit_shift = new TF1("pcfit_shift", fitFunction_shift, 0, 5000, npars);

  double a1_initParam[npars]={1.000000E+03,2.000000E+03,2.322414E-01,6.735000E-04,-2.245000E+00,-6.735000E+00,1.683750E-06};
  double a2_initParam[npars]={1.000000E+03,2.000000E+03,2.230517E-01,6.468500E-04,-2.156167E+00,-6.468500E+00,1.617125E-06};
  double n1_initParam[npars]={1.000000E+03,2.000000E+03,1.024983E+00,2.972450E-03,-9.908167E+00,-2.972450E+01,7.431125E-06};
  double n2_initParam[npars]={1.000000E+03,2.000000E+03,4.043966E-01,1.172750E-03,-3.909167E+00,-1.172750E+01,2.931875E-06};
  double mean_initParam[npars]={1.000000E+03,2.000000E+03,5.242586E-02,1.520350E-04,-5.067833E-01,-1.520350E+00,3.800875E-07};
  double sigma_initParam[npars]={1.000000E+03,2.000000E+03,1.742931E+00,5.054500E-03,-1.684833E+01,-5.054500E+01,1.263625E-05};

//initialize
  RooRealVar* a1_pars[npars];
  RooRealVar* a2_pars[npars];
  RooRealVar* n1_pars[npars];
  RooRealVar* n2_pars[npars];
  RooRealVar* mean_pars[npars];
  RooRealVar* sigma_pars[npars];

  RooArgList obsList[100],a1_parList,a2_parList,n1_parList,n2_parList,mean_parList,sigma_parList;

  double frac=0.1;

  for (int ip=0; ip<npars; ip++){
  a1_pars[ip] = new RooRealVar(Form("a1_bindFunction_%i", ip), "", a1_initParam[ip],a1_initParam[ip]-frac*fabs(a1_initParam[ip]),a1_initParam[ip]+frac*fabs(a1_initParam[ip]));
  n1_pars[ip] = new RooRealVar(Form("n1_bindFunction_%i", ip), "", n1_initParam[ip],n1_initParam[ip]-frac*fabs(n1_initParam[ip]),n1_initParam[ip]+frac*fabs(n1_initParam[ip]));
  a2_pars[ip] = new RooRealVar(Form("a2_bindFunction_%i", ip), "", a2_initParam[ip],a2_initParam[ip]-frac*fabs(a2_initParam[ip]),a2_initParam[ip]+frac*fabs(a2_initParam[ip]));
  n2_pars[ip] = new RooRealVar(Form("n2_bindFunction_%i", ip), "", n2_initParam[ip],n2_initParam[ip]-frac*fabs(n2_initParam[ip]),n2_initParam[ip]+frac*fabs(n2_initParam[ip]));
  mean_pars[ip] = new RooRealVar(Form("mean_bindFunction_%i", ip), "", mean_initParam[ip],mean_initParam[ip]-frac*fabs(mean_initParam[ip]),mean_initParam[ip]+frac*fabs(mean_initParam[ip]));
  sigma_pars[ip] = new RooRealVar(Form("sigma_bindFunction_%i", ip), "", sigma_initParam[ip],sigma_initParam[ip]-frac*fabs(sigma_initParam[ip]),sigma_initParam[ip]+frac*fabs(sigma_initParam[ip]));

  a1_parList.add(*(a1_pars[ip]));
  a2_parList.add(*(a2_pars[ip]));
  n1_parList.add(*(n1_pars[ip]));
  n2_parList.add(*(n2_pars[ip]));
  mean_parList.add(*(mean_pars[ip]));
  sigma_parList.add(*(sigma_pars[ip]));
  }

// fix the junction point
  for (int ip=0; ip<2; ip++){
   a1_pars[ip]->setConstant(kTRUE);
   a2_pars[ip]->setConstant(kTRUE);
   n1_pars[ip]->setConstant(kTRUE);
   n2_pars[ip]->setConstant(kTRUE);
   sigma_pars[ip]->setConstant(kTRUE);
   mean_pars[ip]->setConstant(kTRUE);
  }

//if not simultaneous fitting, fix all parameters
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

//binding 
  RooRealVar* MH[100];
  RooTFnBinding* pa1[100];
  RooTFnBinding* pa2[100];
  RooTFnBinding* pn1[100];
  RooTFnBinding* pn2[100];
  RooTFnBinding* pmean[100];
  RooTFnBinding* pmean_shift[100];
  RooTFnBinding* psigma[100];

  for (int i=0; i<maxMassBin; i++) {
    char formulamass[200];

//set mass bin, as x of fit function
    MH[i]= new RooRealVar("MH","MH",0,0,5000);
    MH[i]->setVal(massBin[i]);
    MH[i]->setConstant(kTRUE);
    obsList[i].add(*MH[i]);

    pa1[i] = new RooTFnBinding("bound_fcn_a1", "", pcfit, obsList[i], a1_parList);
    pa2[i] = new RooTFnBinding("bound_fcn_a2", "", pcfit, obsList[i], a2_parList);
    pn1[i] = new RooTFnBinding("bound_fcn_n1", "", pcfit, obsList[i], n1_parList);
    pn2[i] = new RooTFnBinding("bound_fcn_n2", "", pcfit, obsList[i], n2_parList);
    pmean[i] = new RooTFnBinding("bound_fcn_mean", "", pcfit, obsList[i], mean_parList);
    pmean_shift[i] = new RooTFnBinding("bound_fcn_mean", "", pcfit_shift, obsList[i], mean_parList);
    psigma[i] = new RooTFnBinding("bound_fcn_sigma", "", pcfit, obsList[i], sigma_parList);
  
    sprintf(tempmass,"mh%d",massBin[i]);
    sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);
 
//dCB paramters set as fitted function
    char fitrange[50];
    sprintf(fitrange,"fitRange_mh%d",massBin[i]);
    x.setRange(fitrange,xMin[i],xMax[i]) ;

    DCBall[i] = new RooDoubleCB(Form("DCBall%d",i),"Double Crystal ball",x,*pmean[i],*psigma[i],*pa1[i],*pn1[i],*pa2[i],*pn2[i]);
    rs.addPdf(*DCBall[i], tempmass);

    DCBall_shift[i] = new RooDoubleCB(Form("DCBall%d",i),"Double Crystal ball",x,*pmean_shift[i],*psigma[i],*pa1[i],*pn1[i],*pa2[i],*pn2[i]);
    rs_shift.addPdf(*DCBall_shift[i], tempmass);
    }

    RooFitResult *fitressim = (RooFitResult*)rs.fitTo(*dataset,Strategy(2),Range("fitRange"),SplitRange());
    RooArgSet * params2 = rs.getParameters(RooArgList(x,massrc));
    char paramfilename[100];
    sprintf(paramfilename,"closureTest/fix_param_%s.txt",cType);
    params2->writeToFile(paramfilename) ;

    TCanvas *c1 = new TCanvas("c1","c1",1200,400); //all
    TCanvas *c2 = new TCanvas("c2","c2",725,725); //individual

    c2->cd();
    TPad *pad1 = new TPad("pad1","fit",0.05,0.35,0.95,0.97);
    pad1->Draw();
    TPad *pad2 = new TPad("pad2","pull",0.05,0.02,0.95,0.35);
    pad2->Draw();

  for (int i=0; i<maxMassBin; i++) {

    sprintf(tempmass,"mh%d",massBin[i]);
    sprintf(tempmass2,"massrc == massrc::mh%d",massBin[i]);

    int col=kRed+3;;
    char framename[100];
    sprintf(framename,"Resolution_MH%dGeV_%s",massBin[i],cType);

//draw individual plots
    RooPlot* xframe_ind = x.frame(Range(xMin[i],xMax[i]),Bins(100),Title(framename)) ;
    dataset->plotOn(xframe_ind,DataError(RooAbsData::SumW2), MarkerStyle(kOpenCircle), MarkerSize(1.1), Cut(tempmass2) );
    rs.plotOn(xframe_ind,LineColor(col),Slice(massrc,tempmass),ProjWData(massrc,*dataset));

//draw pull

    RooHist* hpull = xframe_ind->pullHist();
    RooPlot* frame2 = x.frame(Range(xMin[i],xMax[i]),Title("Pull Distribution")) ;
    frame2->addPlotable(hpull,"P");
    frame2->SetMinimum(-20);
    frame2->SetMaximum(20);

//draw ratio
/*
    RooPlot* frame2 = x.frame(Range(xMin[i],xMax[i]),Title("MC/model Distribution")) ;
    char histname[100];
    sprintf(histname,"h_resoM_Cut[massrc == massrc::mh%d]",massBin[i]);
    RooHist* histo = (RooHist*) xframe->findObject(histname) ;
    RooCurve* func = (RooCurve*) xframe->findObject("DCBall_Norm[reso]") ;
    Double_t chi2_fit=0 ;
    RooHist* ratiohist = new RooHist;
    for (Int_t i=0 ; i<histo->GetN() ; i++) {
    Double_t xdata,ydata ;
    histo->GetPoint(i,xdata,ydata) ;
    Double_t yfunc = func->interpolate(xdata) ;
    Double_t y = ydata/yfunc ;
    Double_t data_dyl = histo->GetErrorYlow(i) ;
    Double_t data_dyh = histo->GetErrorYhigh(i) ;
    Double_t dyl = data_dyl/yfunc;
    Double_t dyh = data_dyh/yfunc;
    ratiohist->addBinWithError(xdata,y,dyl,dyh);
    ratiohist->SetMarkerStyle(kFullCircle);
    if (ydata>0) chi2_fit=chi2_fit + (ydata-yfunc)*(ydata-yfunc)/ydata ; }
    cout<<"chi2="<<chi2_fit;
    frame2->addPlotable(ratiohist,"P");
    pad2->SetLogy();
*/

    c2->cd();
    pad1->cd();
    xframe_ind->Draw();
    pad2->cd() ;
    frame2->Draw() ;


//draw all plots together
    RooPlot* xframe = x.frame(Bins(200),Title("Resolution")) ;
    dataset->plotOn(xframe,Normalization(1,RooAbsReal::NumEvent),DataError(RooAbsData::SumW2), MarkerStyle(kOpenCircle), MarkerSize(1.1), Cut(tempmass2) );
    xframe->GetYaxis()->SetTitleOffset(1.5);
    rs_shift.plotOn(xframe,LineColor(col),Slice(massrc,tempmass),ProjWData(massrc,*dataset));

//normalization
    char histname[100];
    sprintf(histname,"h_resoM_Cut[massrc == massrc::mh%d]",massBin[i]);
    RooHist* histo = (RooHist*) xframe->findObject(histname) ;
    RooHist* histonorm = new RooHist;
    RooCurve* func = (RooCurve*) xframe->findObject("DCBall_Norm[reso]") ;

    double width=histo->getNominalBinWidth();
    double norm_1=histo->getFitRangeNEvt(xMin[i],xMax[i]);

    double norm=0;
    for (Int_t j=0 ; j<histo->GetN() ; j++) {
    Double_t xdata,ydata ;
    //cout<<"bin"<<j<<",y"<<ydata<<endl;
    histo->GetPoint(j,xdata,ydata) ;
    norm+=ydata;}
    //cout<<"mass"<<massBin[i]<<",width"<<width<<",norm:"<< norm<<",norm_1:"<<norm_1<<endl;


    for (Int_t j=0 ; j<histo->GetN() ; j++) {
    Double_t xdata,ydata ;
    histo->GetPoint(j,xdata,ydata) ;
    Double_t x_shift=xdata+massBin[i] ;
    Double_t y=ydata/norm ;
    Double_t data_dyl = histo->GetErrorYlow(j) ;
    Double_t data_dyh = histo->GetErrorYhigh(j) ;
    Double_t dyl = data_dyl/norm;
    Double_t dyh = data_dyh/norm;
    histonorm->addBinWithError(x_shift,y,dyl,dyh);
    histonorm->SetMarkerStyle(kOpenCircle);
    }

    RooPlot* xframe_all = x.frame(Range(0,5000,true),Title("Resolution")) ;
    xframe_all->GetYaxis()->SetTitle("Normalized Units");
    xframe_all->GetXaxis()->SetTitle("m_{reco}-m_{true}+m_{pole} (GeV)");
//FIXME:normalization
    rs_shift.plotOn(xframe_all,Normalization(0.5*norm_1/norm,RooAbsReal::NumEvent),LineColor(col),Slice(massrc,tempmass),ProjWData(massrc,*dataset));
    xframe_all->addPlotable(histonorm,"P");


    char filename1[100];
    char filename2[100];

    sprintf(filename1,"closureTest/%s/Resolution_MH%d_%s_fixParam.png",cType,massBin[i],cType);
    sprintf(filename2,"closureTest/%s/Resolution_MH%d_%s_fixParam.pdf",cType,massBin[i],cType);
    c2->SaveAs(filename1);
    c2->SaveAs(filename2);
    c1->cd();
    xframe_all->Draw("SAME");
    }

   c1->SaveAs(Form("closureTest/all_%s.png",cType));
   c1->SaveAs(Form("closureTest/all_%s.pdf",cType));
 }
 

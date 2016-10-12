void fit_c(){
gStyle->SetOptStat(0);
        TFile* file = TFile::Open("/afs/cern.ch/work/c/cayou/HighMass_RunII/CMSSW_7_6_3/src/ZZAnalysis/AnalysisStep/test/CombInputs/HighMassStudy_160921_2l2q_4l/template_2l2q/Dbkg_spin2.root");
        TH1F* c_const = (TH1F*)file->Get("c");
  TH1F *dummy = new TH1F("2l2q spin0 c-constant","2l2q spin0 c-constant",100,0,4000);
  dummy->GetYaxis()->SetRangeUser(0,3);
  TCanvas* c = new TCanvas("c","c", 1600,800);
  c->SetFillColor(0);
  c->SetRightMargin(0.18);
  c_const->GetXaxis()->SetRangeUser(0,4000);
  c_const->SetLineColor(4);
   //TF1 *polyFunctot= new TF1("polyFunctot","[0]+0.0005*x-[1]/(1+exp((x-2500)/[2]))",0., 4000); //fit spin0
   //polyFunctot->SetParameters(5,5,300);
  TF1 *polyFunctot= new TF1("polyFunctot","[0]",0., 4000); //fit spin2
   c_const->Fit("polyFunctot");
//   c_const->Fit("pol1");
  dummy->Draw();
  polyFunctot->Draw("SAME");
  c_const->Draw("E1SAME");
  c->SaveAs("~/www/HighMass/160926/spin2_c_fit.png");
}

//3.05+0.0005*x-2.73/(1+exp((x-2500)/258.26))

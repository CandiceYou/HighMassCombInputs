void cut(TString filename,TString new_filename, double Pmass){
   if (gSystem->AccessPathName(filename)) return;
   TFile *f = TFile::Open(filename);
   TTree *T = (TTree*)f->Get("ZZTree/candTree");
//   TTree *T2 = (TTree*)f->Get("ZZTreelooseEle/candTree");

   float GenHMass;
   T->SetBranchAddress("GenHMass", &GenHMass);
//   T2->SetBranchAddress("GenHMass", &k);

  TFile *newfile = new TFile(new_filename,"recreate");

  TDirectory *dir = newfile->mkdir("ZZTree");
  dir->cd();
  TTree *newtree = T->CloneTree(0);
  for (Long64_t i=0;i<T->GetEntries();i++) {
      T->GetEntry(i);
      if(GenHMass <= Pmass) {
        newtree->Fill();
    }
  }

/*
  TDirectory *dir2 = newfile->mkdir("ZZTreelooseEle");
  dir2->cd();
  TTree *newtree2 = T2->CloneTree(0);

  for (Long64_t i=0;i<T2->GetEntries();i++) {
      T2->GetEntry(i);
      if(GenHMass2 <= Pmass) {
        newtree2->Fill();
    }
  }
*/
//   T->Print();
   newtree->AutoSave();
//   newtree2->AutoSave();
   delete f;
   delete newfile;
}


void cutPM(){
  TString  inputDir = "/afs/cern.ch/work/c/cayou/public/80Xsamples/samples_2016mc_0726/";

  int sampleMass_ggH[]={115,120,124,125,/*126,130,*/135,140,145,150,155,160,165,170,175,180,190,/*200,*/210,/*230,*/250,270};
  int sampleMass_VBF[]={/*115,*/120,/*124,125,*/126,/*130,*/135,140,145,/*150,*/155,160,165,170,175,180,190,200,210,230,250,270};
//  int sampleMass_ggH_2bp[]={750,800,1200,2000,3000,4000};
  int sampleMass_ggH_2bp[]={};
//  int sampleMass_ggH[]={};
//  int sampleMass_VBF[]={};

  int Nfiles_ggH=sizeof(sampleMass_ggH)/sizeof(*sampleMass_ggH);
  int Nfiles_VBF=sizeof(sampleMass_VBF)/sizeof(*sampleMass_VBF);
  int Nfiles_ggH_2bp=sizeof(sampleMass_ggH_2bp)/sizeof(*sampleMass_ggH_2bp);

  char inputfile_ggH[1000];
  char inputfile_VBF[1000];
  char inputfile_ggH_2bp[1000];
  char outfile_ggH[1000];
  char outfile_VBF[1000];
  char outfile_ggH_2bp[1000];


   for (int i=0; i<Nfiles_ggH; i++) {
     cout<<"ggH sample"<<sampleMass_ggH[i]<<endl;
     sprintf(inputfile_ggH,"/ggH%d/ZZ4lAnalysis.root",sampleMass_ggH[i]);
     sprintf(outfile_ggH,"/ggH%d/ZZ4lAnalysis_new.root",sampleMass_ggH[i]);
     cut(inputDir+inputfile_ggH,inputDir+outfile_ggH,sampleMass_ggH[i]);
   }


   for (int i=0; i<Nfiles_VBF; i++) {
     cout<<"VBF sample"<<sampleMass_VBF[i]<<endl;
     sprintf(inputfile_VBF,"/VBFH%d/ZZ4lAnalysis.root",sampleMass_VBF[i]);
     sprintf(outfile_VBF,"/VBFH%d/ZZ4lAnalysis_new.root",sampleMass_VBF[i]);
     cut(inputDir+inputfile_VBF,inputDir+outfile_VBF,sampleMass_VBF[i]);
   }


/*   for (int i=0; i<Nfiles_ggH_2bp; i++) {
     sprintf(inputfile_ggH_2bp,"/gg2bp_M%d.root",sampleMass_ggH_2bp[i]);
     sprintf(outfile_ggH_2bp,"/gg2bp_M%d_new.root",sampleMass_ggH_2bp[i]);
   }
*/
}

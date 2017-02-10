#include "TMath.h"
#include "TROOT.h"
#include "../interface/ResoEff.h"

//spin: 0 for spin0, 1 for spin2
//prod: 0 for ggH, 1 for VBF


HighMass::HighMass(Sample s){
	this->sample = s;
}

void HighMass::readCandTree(const char* fin){

//add files from different dir
//        TString  inputDir2 = "/afs/cern.ch/work/c/cayou/public/80Xsamples/2l2qsamples_170107/"; 
        TString  inputDir2 = "root://eoscms//eos/cms/store/caf/user/sudha/ZZ2l2q/Moriond2017/V1/";

	char inputfile[1000];
	vector<TString> files_spin0ggH,files_spin0VBF,files_spin2;

	for (int i=0; i<Nfiles_spin0ggH; i++) {
		sprintf(inputfile,"ggHiggs%d/ZZ2l2qAnalysis.root",inputfiles_spin0ggH[i]);
		files_spin0ggH.push_back(inputfile);
	}

	for (int i=0; i<Nfiles_spin0VBF; i++) {
		sprintf(inputfile,"VBFHiggs%d/ZZ2l2qAnalysis.root",inputfiles_spin0VBF[i]);
		files_spin0VBF.push_back(inputfile);
	}

	for (int i=0; i<Nfiles_spin2; i++) {
		sprintf(inputfile,"2bp_W0p1_M%d/ZZ2l2qAnalysis.root",inputfiles_spin2[i]);
		files_spin2.push_back(inputfile);
		sprintf(inputfile,"2bp_W0p2_M%d/ZZ2l2qAnalysis.root",inputfiles_spin2[i]);
		files_spin2.push_back(inputfile);
		sprintf(inputfile,"2bp_W0p3_M%d/ZZ2l2qAnalysis.root",inputfiles_spin2[i]);
		files_spin2.push_back(inputfile);
	}

	if ( sample == spin0_ggH){ //spin0 ggH
		for (vector<TString>::const_iterator file = files_spin0ggH.begin(); file!=files_spin0ggH.end(); ++file)
			candTree->Add(inputDir+(*file));}
	else if (sample == spin0_VBF){ //spin0 VBF
		for (vector<TString>::const_iterator file = files_spin0VBF.begin(); file!=files_spin0VBF.end(); ++file)
			candTree->Add(inputDir+(*file));
                candTree->Add(inputDir2+"VBFHiggs1500.root");
                candTree->Add(inputDir2+"VBFHiggs2000.root");
                candTree->Add(inputDir2+"VBFHiggs2500.root");
                candTree->Add(inputDir2+"VBFHiggs3000.root");
                }
        else if (sample == spin0_all){ //spin0 ggH+VBF
                for (vector<TString>::const_iterator file = files_spin0ggH.begin(); file!=files_spin0ggH.end(); ++file)
                        candTree->Add(inputDir+(*file));
                for (vector<TString>::const_iterator file = files_spin0VBF.begin(); file!=files_spin0VBF.end(); ++file)
                { candTree->Add(inputDir+(*file));
                candTree->Add(inputDir2+"VBFHiggs1500.root");
                candTree->Add(inputDir2+"VBFHiggs2000.root");
                candTree->Add(inputDir2+"VBFHiggs2500.root");
                candTree->Add(inputDir2+"VBFHiggs3000.root");}
                }
	else if (sample == spin2){ //spin2
		for (vector<TString>::const_iterator file = files_spin2.begin(); file!=files_spin2.end(); ++file)
			candTree->Add(inputDir+(*file));}
        else if (sample == DYjets) {
/*
                 candTree->Add(inputDir+"DY1JetsToLL/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"DY2JetsToLL/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"DY3JetsToLL/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"DY4JetsToLL/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"DYBFiltJetsToLL/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"DYBJetsToLL/ZZ2l2qAnalysis.root");
*/
                 candTree->Add(inputDir2+"DY1Jet.root");
                 candTree->Add(inputDir2+"DY2Jet.root");
                 candTree->Add(inputDir2+"DY3Jet.root");
                 candTree->Add(inputDir2+"DY4Jet.root");
                 candTree->Add(inputDir2+"DYJetsToLL_BGenFilter.root");
                 candTree->Add(inputDir2+"DYBJetsToLL.root");
                 }
        else if (sample == TTBar) {
/*
                 candTree->Add(inputDir+"TTBar/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"WW2l2nDib/ZZ2l2qAnalysis.root");
*/
                 candTree->Add(inputDir2+"TTBar.root");
                 }
        else if (sample == Diboson) {
/*
                 candTree->Add(inputDir+"WZ3lnDib/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"WZ2l2qDib/ZZ2l2qAnalysis.root");
                 candTree->Add(inputDir+"ZZ2l2qDib/ZZ2l2qAnalysis.root");
*/
                 candTree->Add(inputDir2+"WZDiboson.root");
                 candTree->Add(inputDir2+"ZZDiboson.root");
                 }
        else if (sample == data) {
                 candTree->Add(inputDir2+"DoubleEG2016B.root");
                 candTree->Add(inputDir2+"DoubleEG2016C.root");
                 candTree->Add(inputDir2+"DoubleEG2016D.root");
                 candTree->Add(inputDir2+"DoubleEG2016E.root");
                 candTree->Add(inputDir2+"DoubleEG2016F.root");
                 candTree->Add(inputDir2+"DoubleEG2016G.root");
                 candTree->Add(inputDir2+"DoubleEG2016Hv1.root");
                 candTree->Add(inputDir2+"DoubleEG2016Hv2.root");
                 candTree->Add(inputDir2+"DoubleEG2016Hv3.root");
                 candTree->Add(inputDir2+"DoubleMu2016B.root");
                 candTree->Add(inputDir2+"DoubleMu2016C.root");
                 candTree->Add(inputDir2+"DoubleMu2016D.root");
                 candTree->Add(inputDir2+"DoubleMu2016E.root");
                 candTree->Add(inputDir2+"DoubleMu2016F.root");
                 candTree->Add(inputDir2+"DoubleMu2016G.root");
                 candTree->Add(inputDir2+"DoubleMu2016Hv1.root");
                 candTree->Add(inputDir2+"DoubleMu2016Hv2.root");
                 candTree->Add(inputDir2+"DoubleMu2016Hv3.root");
                 candTree->Add(inputDir2+"SingleEG2016B.root");
                 candTree->Add(inputDir2+"SingleEG2016C.root");
                 candTree->Add(inputDir2+"SingleEG2016D.root");
                 candTree->Add(inputDir2+"SingleEG2016E.root");
                 candTree->Add(inputDir2+"SingleEG2016F.root");
                 candTree->Add(inputDir2+"SingleEG2016G.root");
                 candTree->Add(inputDir2+"SingleEG2016Hv1.root");
                 candTree->Add(inputDir2+"SingleEG2016Hv2.root");
                 candTree->Add(inputDir2+"SingleEG2016Hv3.root");
                 candTree->Add(inputDir2+"SingleMu2016B.root");
                 candTree->Add(inputDir2+"SingleMu2016C.root");
                 candTree->Add(inputDir2+"SingleMu2016D.root");
                 candTree->Add(inputDir2+"SingleMu2016E.root");
                 candTree->Add(inputDir2+"SingleMu2016F.root");
                 candTree->Add(inputDir2+"SingleMu2016G.root");
                 candTree->Add(inputDir2+"SingleMu2016Hv1.root");
                 candTree->Add(inputDir2+"SingleMu2016Hv2.root");
                 candTree->Add(inputDir2+"SingleMu2016Hv3.root");
                 }

        else if (sample == Custom) {
                 candTree->Add(fin);
                 }
	else { cout <<"model doesn't exist!"<<endl; return;}

	candTree->SetBranchAddress("ZZCandType",&ZZCandType);
	candTree->SetBranchAddress("ZZsel",&ZZsel);
	candTree->SetBranchAddress("ZZMassRefit",&ZZMassRefit);
	candTree->SetBranchAddress("ZZMass",&ZZMass);
	candTree->SetBranchAddress("Z1Mass",&Z1Mass);
	candTree->SetBranchAddress("Z2Mass",&Z2Mass);
	candTree->SetBranchAddress("pqqZJJ_VAMCFM",&pqqZJJ_VAMCFM);
	candTree->SetBranchAddress("p0plus_VAJHU",&p0plus_VAJHU);
	candTree->SetBranchAddress("p2bplus_VAJHU",&p2bplus_VAJHU);
        candTree->SetBranchAddress("pqqZJJ_VAMCFM_up",&pqqZJJ_VAMCFM_up);
        candTree->SetBranchAddress("p0plus_VAJHU_up",&p0plus_VAJHU_up);
        candTree->SetBranchAddress("p2bplus_VAJHU_up",&p2bplus_VAJHU_up);
        candTree->SetBranchAddress("pqqZJJ_VAMCFM_dn",&pqqZJJ_VAMCFM_dn);
        candTree->SetBranchAddress("p0plus_VAJHU_dn",&p0plus_VAJHU_dn);
        candTree->SetBranchAddress("p2bplus_VAJHU_dn",&p2bplus_VAJHU_dn);
	candTree->SetBranchAddress("phjj_VAJHU_highestPTJets",&phjj_VAJHU_highestPTJets);
	candTree->SetBranchAddress("pvbf_VAJHU_highestPTJets",&pvbf_VAJHU_highestPTJets);
	candTree->SetBranchAddress("GenHMass",&GenHMass);
	candTree->SetBranchAddress("xsec",&xsec);
	candTree->SetBranchAddress("overallEventWeight",&overallEventWeight);
	candTree->SetBranchAddress("genHEPMCweight",&genHEPMCweight);
	candTree->SetBranchAddress("PUWeight",&PUWeight);
	candTree->SetBranchAddress("Z1tau21", &Z1tau21);
	candTree->SetBranchAddress("Z1Pt", &Z1Pt);
	candTree->SetBranchAddress("Z2Pt", &Z2Pt);
	candTree->SetBranchAddress("Z2Flav", &Z2Flav);
	candTree->SetBranchAddress("JetQGLikelihood", &JetQGLikelihood);
	candTree->SetBranchAddress("JetBTagger", &JetBTagger);
	candTree->SetBranchAddress("JetIsInZZCand", &JetIsInZZCand);
	candTree->SetBranchAddress("JetPt", &JetPt);
}

void HighMass::makeSelectedTree(const char* fin){


        TString finName=Form("2l2qSelectedTrees/2l2qtree_%s.root",sampleName[sample]);
        bool fileExist=(!gSystem->AccessPathName(finName));
        if(fileExist) {
        cout<<"File "<<sampleName[sample]<<" already exists! Aborting.."<<endl;
        return;}

        HighMass::readCandTree(fin);

	float m2l2q_tmp,mz1_tmp,mz2_tmp,weight_tmp,Dbkg_0plus_tmp,Dbkg_2bplus_tmp,Dvbf_tmp,p0plus,p2bplus,pbkg,p0plus_up,p2bplus_up,pbkg_up,Dbkg_0plus_up_tmp,Dbkg_2bplus_up_tmp,p0plus_dn,p2bplus_dn,pbkg_dn,Dbkg_0plus_dn_tmp,Dbkg_2bplus_dn_tmp;
	short typ_tmp,lep_tmp,tag_tmp;

	TTree *SelectedTree = new TTree("test","test");

	SelectedTree->Branch("ZZMass",&m2l2q_tmp,"ZZMass/F");
        SelectedTree->Branch("Z1Mass",&mz1_tmp,"Z1Mass/F");
        SelectedTree->Branch("Z2Mass",&mz2_tmp,"Z2Mass/F");
	SelectedTree->Branch("GenHMass",&GenHMass,"GenHMass/F");
	SelectedTree->Branch("jetType",&typ_tmp,"jetType/S");
	SelectedTree->Branch("lepFlav",&lep_tmp,"lepFlav/S");
	SelectedTree->Branch("tag",&tag_tmp,"tag/S");
        SelectedTree->Branch("weight",&weight_tmp,"weight/F");

        SelectedTree->Branch("Dbkg_0plus",&Dbkg_0plus_tmp,"Dbkg_0plus/F");
        SelectedTree->Branch("Dbkg_0plus_up",&Dbkg_0plus_up_tmp,"Dbkg_0plus_up/F");
        SelectedTree->Branch("Dbkg_0plus_dn",&Dbkg_0plus_dn_tmp,"Dbkg_0plus_dn/F");

        SelectedTree->Branch("Dbkg_2bplus",&Dbkg_2bplus_tmp,"Dbkg_2bplus/F");
        SelectedTree->Branch("Dbkg_2bplus_up",&Dbkg_2bplus_up_tmp,"Dbkg_2bplus_up/F");
        SelectedTree->Branch("Dbkg_2bplus_dn",&Dbkg_2bplus_dn_tmp,"Dbkg_2bplus_dn/F");

        SelectedTree->Branch("Dvbf",&Dvbf_tmp,"Dvbf/F");

        SelectedTree->Branch("p0plus",&p0plus,"p0plus/F");
        SelectedTree->Branch("p0plus_up",&p0plus_up,"p0plus_up/F");
        SelectedTree->Branch("p0plus_dn",&p0plus_dn,"p0plus_dn/F");

        SelectedTree->Branch("p2bplus",&p2bplus,"p2bplus/F");
        SelectedTree->Branch("p2bplus_up",&p2bplus_up,"p2bplus_up/F");
        SelectedTree->Branch("p2bplus_dn",&p2bplus_dn,"p2bplus_dn/F");

        SelectedTree->Branch("pZjj",&pbkg,"pZjj/F");
        SelectedTree->Branch("pZjj_up",&pbkg_up,"pZjj_up/F");
        SelectedTree->Branch("pZjj_dn",&pbkg_dn,"pZjj_dn/F");


	for (int i = 0; i < candTree->GetEntries(); i++) {
		candTree->GetEntry(i);

		// Fill gen histo
		hgen->Fill(GenHMass,(genHEPMCweight * PUWeight));

		typ_tmp=-1, lep_tmp=-1, tag_tmp = -1; 
		int candID=-1 , candID_M=-1, candID_R=-1;

		//selection
		for (int j = 0; j < ZZCandType->size(); j++) {
			if (((ZZCandType->at(j)==1 && Z1tau21->at(j)<=0.6 && ZZMass->at(j)>=300)||(ZZCandType->at(j)==2 && ZZMassRefit->at(j)>=300)) && fabs(ZZsel->at(j))>=100 && Z1Mass->at(j)>=70 && Z1Mass->at(j)<=105 && Z2Mass->at(j)>=60 && Z1Pt->at(j)>100 && Z2Pt->at(j)>100){
				if (ZZCandType->at(j)==1) candID_M=j; //merged, SR
				else if (ZZCandType->at(j)==2) candID_R=j;  //resolved, SR
			}
		}

		//prioritize 
		if( (candID_M==-1) && (candID_R==-1)) {candID=-1 ; typ_tmp=-1;}
		else if ( (candID_M==-1) && (candID_R!=-1)) {candID=candID_R ; typ_tmp=1;}
		else if ( (candID_M!=-1) && (candID_R==-1)) {candID=candID_M ; typ_tmp=0;}
		else if ( (candID_M!=-1) && (candID_R!=-1)){
			if (Z1Pt->at(candID_M)>300 && Z2Pt->at(candID_M)>200) {typ_tmp=0; candID=candID_M;}
			else {typ_tmp=1; candID=candID_R;}
		}

		//b-tagging
		float pt1stJet = 0.0001,pt2ndJet = 0.0001,btag1stJet = 0.,btag2ndJet = 0.,qglik1stJet = 0.,qglik2ndJet = 0.;
		float pt1stSubjet = 0.0001,pt2ndSubjet = 0.0001,btag1stSubjet = 0.,btag2ndSubjet = 0.;
		int nInJets = 0, nExtraJets = 0;

		for (unsigned int nJet=0; nJet<JetPt->size(); nJet++) {
			if (JetQGLikelihood->at(nJet) > -800.) {            // real jets
				if (JetIsInZZCand->at(nJet) ) {
					if (pt1stJet < JetPt->at(nJet)) {
						pt2ndJet = pt1stJet;
						pt1stJet = JetPt->at(nJet);
						qglik2ndJet = qglik1stJet;
						qglik1stJet = JetQGLikelihood->at(nJet);
					} else if (pt2ndJet < JetPt->at(nJet)) {
						pt2ndJet = JetPt->at(nJet);
						qglik2ndJet = JetQGLikelihood->at(nJet);
					}
					if (btag1stJet < JetBTagger->at(nJet)) {
						btag2ndJet = btag1stJet;
						btag1stJet = JetBTagger->at(nJet);
					} else if (btag2ndJet < JetBTagger->at(nJet)) {
						btag2ndJet = JetBTagger->at(nJet);
					}
					nInJets++;
				} else nExtraJets++;
			}
		}


		for (unsigned int nJet=0; nJet<JetPt->size(); nJet++) {
			if (JetQGLikelihood->at(nJet) < -800.) {            // subjets
				if (JetIsInZZCand->at(nJet) ) {
					if (pt1stSubjet < JetPt->at(nJet)) {
						pt2ndSubjet = pt1stSubjet;
						pt1stSubjet = JetPt->at(nJet);
					} else if (pt2ndSubjet < JetPt->at(nJet)) {
						pt2ndSubjet = JetPt->at(nJet);
					}
					if (btag1stSubjet < JetBTagger->at(nJet)) {
						btag2ndSubjet = btag1stSubjet;
						btag1stSubjet = JetBTagger->at(nJet);
					} else if (btag2ndSubjet < JetBTagger->at(nJet)) {
						btag2ndSubjet = JetBTagger->at(nJet);
					}
				}
			}
		}


		// VBF tagging

		if (candID!=-1){

			if (typ_tmp==0) m2l2q_tmp=ZZMass->at(candID);
			else if (typ_tmp==1) m2l2q_tmp=ZZMassRefit->at(candID);

                        mz1_tmp=Z1Mass->at(candID);
                        mz2_tmp=Z2Mass->at(candID);
			//bkg Discriminant
			Dbkg_0plus_tmp=-1;Dbkg_0plus_dn_tmp=-1;Dbkg_0plus_up_tmp=-1;
                        Dbkg_2bplus_tmp=-1;Dbkg_2bplus_up_tmp=-1;Dbkg_2bplus_dn_tmp=-1;

			p0plus=p0plus_VAJHU->at(candID);
			p2bplus=p2bplus_VAJHU->at(candID);
			pbkg=pqqZJJ_VAMCFM->at(candID);

                        p0plus_up=p0plus_VAJHU_up->at(candID);
                        p2bplus_up=p2bplus_VAJHU_up->at(candID);
                        pbkg_up=pqqZJJ_VAMCFM_up->at(candID);

                        p0plus_dn=p0plus_VAJHU_dn->at(candID);
                        p2bplus_dn=p2bplus_VAJHU_dn->at(candID);
                        pbkg_dn=pqqZJJ_VAMCFM_dn->at(candID);

			float c_const_spin0=1,c_const_spin2=1;

			c_const_spin0=0.035*(3.05+0.0005*m2l2q_tmp-2.73/(1+exp((m2l2q_tmp-2500)/258.26)));
			Dbkg_0plus_tmp = p0plus/(p0plus+c_const_spin0*pbkg);
                        Dbkg_0plus_up_tmp = p0plus_up/(p0plus_up+c_const_spin0*pbkg_up);
                        Dbkg_0plus_dn_tmp = p0plus_dn/(p0plus_dn+c_const_spin0*pbkg_dn);
			c_const_spin2=0.14;
			Dbkg_2bplus_tmp = p2bplus/(p2bplus+c_const_spin2*pbkg);
                        Dbkg_2bplus_up_tmp = p2bplus_up/(p2bplus_up+c_const_spin2*pbkg_up);
                        Dbkg_2bplus_dn_tmp = p2bplus_dn/(p2bplus_dn+c_const_spin2*pbkg_dn);

			//VBF Discriminant
			Dvbf_tmp=-1;
			float phjj=phjj_VAJHU_highestPTJets->at(candID);
			float pvbf=pvbf_VAJHU_highestPTJets->at(candID);
			float c_vbf = getDVBF2jetsConstant(m2l2q_tmp);

			if (pvbf>=0 && phjj>=0 && (pvbf+phjj)>0) Dvbf_tmp = 1./(1.+c_vbf*phjj/pvbf);

			//lep_tmpton flavor
			if (abs(Z2Flav->at(candID))==121) lep_tmp=0;
			else if (abs(Z2Flav->at(candID))==169) lep_tmp=1;
			else cout<<"Error! undefined flavor!"<<endl;

			//vbf tagged
			if ( (sample==spin0_ggH || sample==spin0_VBF) && nExtraJets>=2 && Dvbf_tmp>(1.043-460./(m2l2q_tmp+634.))) tag_tmp=0;

			//b-tagged
			else if ((typ_tmp==1 && btag1stJet>0.46 && btag2ndJet>0.46)||(typ_tmp==0 && btag1stSubjet>0.46 && btag2ndSubjet>0.46)) tag_tmp=1;

			//untagged
			else tag_tmp=2;

			float t12weight = 1.;
			if (typ_tmp==0) {
				for (int itau = 0; itau < 24; itau++) {
					if (Z1tau21->at(candID) > tau21bin[itau] && Z1tau21->at(candID) < tau21bin[itau+1])
						t12weight = 1.+tau21corr[itau];
				}
			}
			weight_tmp=overallEventWeight*t12weight;
			SelectedTree->Fill();
		}
	}

	TFile *outFile = new TFile(Form("2l2qtree_%s.root",sampleName[sample]),"recreate");
	if (outFile->IsOpen() ) cout << "File opened successfully" << endl;
	hgen->Write("hgen");
	SelectedTree->Write("selectedTree");
	outFile->Print();
	outFile->Close();     

        // set public selTree
	selTree->Add(Form("2l2qtree_%s.root",sampleName[sample]));
	selTree->SetBranchAddress("ZZMass",&m2l2q);
        selTree->SetBranchAddress("Z1Mass",&mz1);
        selTree->SetBranchAddress("Z2Mass",&mz2);
	selTree->SetBranchAddress("GenHMass",&mGen);
	selTree->SetBranchAddress("weight",&weight);
	selTree->SetBranchAddress("Dbkg_0plus",&Dbkg_0plus);
        selTree->SetBranchAddress("Dbkg_2bplus",&Dbkg_2bplus);
        selTree->SetBranchAddress("Dbkg_0plus_up",&Dbkg_0plus_up);
        selTree->SetBranchAddress("Dbkg_2bplus_up",&Dbkg_2bplus_up);
        selTree->SetBranchAddress("Dbkg_0plus_dn",&Dbkg_0plus_dn);
        selTree->SetBranchAddress("Dbkg_2bplus_dn",&Dbkg_2bplus_dn);
	selTree->SetBranchAddress("Dvbf",&Dvbf);
	selTree->SetBranchAddress("jetType",&typ);
	selTree->SetBranchAddress("lepFlav",&lep);
	selTree->SetBranchAddress("tag",&tag);

}


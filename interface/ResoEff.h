#ifndef DETEFFECT_H  
#define DETEFFECT_H 
//#include "lib.h"
#include "../include/fitFunction.cc"

enum Sample {spin0_ggH=0, spin0_VBF=1, spin0_all=2, spin2=3, DYjets=4, TTBar=5, Diboson=6, data=7, Custom=8};
const char* sampleName[] = {"spin0_ggH","spin0_VBF","spin0_all","spin2","DYjets","TTBar","Diboson","data","testSample"};


class HighMass{

	private:

//		TString  inputDir = "/afs/cern.ch/work/c/cayou/public/80Xsamples/2l2qsamples_1010/";
//		int inputfiles_spin0ggH[18]={200,250,300,350,400,450,500,550,600,700,750,800,900,1000,1500,2000,2500,3000};
//              int inputfiles_spin0VBF[18]={200,250,300,350,400,450,500,550,600,700,750,800,900,1000,1500,2000,2500,3000};
//		int inputfiles_spin2[6]={750,800,1200,2000,3000,4000};

//                TString  inputDir = "/afs/cern.ch/user/t/tomei/public/HZZ2L2Q/Moriond2017/V1/";
                TString  inputDir = "root://eoscms//eos/cms/store/caf/user/sudha/ZZ2l2q/Moriond2017/V2/";
                int inputfiles_spin0ggH[18]={200,250,300,350,400,450,500,550,600,700,750,800,900,1000,1500,2000,2500,3000};
                int inputfiles_spin0VBF[17]={250,300,350,400,450,500,550,600,700,750,800,900,1000,1500,2000,2500,3000};
		int inputfiles_spin2[1]={750};

		int Nfiles_spin0ggH=sizeof(inputfiles_spin0ggH)/sizeof(*inputfiles_spin0ggH);
		int Nfiles_spin0VBF=sizeof(inputfiles_spin0VBF)/sizeof(*inputfiles_spin0VBF);
		int Nfiles_spin2=sizeof(inputfiles_spin2)/sizeof(*inputfiles_spin2);

		//2l2q input tree (after calling 'readCandTree', all variables here can be read directly by other methods)
		TChain *candTree = new TChain("ZZTree/candTree");
		vector<short> *ZZsel=0,*ZZCandType=0,*Z2Flav=0;
		vector<float> *ZZMass=0,*ZZMassRefit=0,*Z1Mass=0,*Z2Mass=0,*Z1tau21=0,*Z1Pt=0,*Z2Pt=0;
		vector<float> *pvbf_VAJHU_highestPTJets=0,*phjj_VAJHU_highestPTJets=0;
                vector<float> *pqqZJJ_VAMCFM=0,*p0plus_VAJHU=0,*p2bplus_VAJHU=0;
                vector<float> *pqqZJJ_VAMCFM_up=0,*p0plus_VAJHU_up=0,*p2bplus_VAJHU_up=0;
                vector<float> *pqqZJJ_VAMCFM_dn=0,*p0plus_VAJHU_dn=0,*p2bplus_VAJHU_dn=0;
		vector<bool> *JetIsInZZCand=0;
		vector<float> *JetQGLikelihood=0,*JetBTagger=0,*JetPt=0;;
		float GenHMass=0,xsec=0,genHEPMCweight=0,PUWeight=0,overallEventWeight=0;



                //gen mass histogram from input tree
                TH1F *hgen = new TH1F("hgen","hgen",4500,0,4500); //must set 1GeV per bin, max value is at least max used in efficiency.

		//selected tree (after calling 'makeSelectedTree', all variables here can be read directly by other methods)
		TChain *selTree = new TChain("selectedTree","selectedTree");
		float m2l2q=0,mz1=0,mz2=0,mGen=0,weight=0,Dbkg_0plus=0,Dbkg_2bplus=0,Dbkg_0plus_up=0,Dbkg_0plus_dn=0,Dbkg_2bplus_up=0,Dbkg_2bplus_dn=0,Dvbf=0;
		short typ=0,lep=0,tag=0;

		//t12 weight parameters
		float tau21bin[25] = {-0.00769231, 0.0346154, 0.0769231, 0.119231, 0.161538, 0.203846, 0.246154, 0.288462, 0.330769, 0.373077, 0.415385, 0.457692, 0.5, 0.542308, 0.584615, 0.626923, 0.669231, 0.711538, 0.753846, 0.796154, 0.838461, 0.880769, 0.923077, 0.965385, 1.00769} ;
		float tau21corr[24] = {0, 0, 0, 0.290173, -0.377686, 0.0977722, 0.412889, 0.322422, -0.169658, -0.272581, -0.384607, 0.109909, -0.123669, -0.158826, 0.0764657, 0.155703, -0.0750966, -0.0484963, 0.239322, 0.107814, -1.40918, 0, 0, 0};

                //efficiency
                const int Mmin=300,Mmax=4500; //Mmax is the max for hgen,hreco and plotting. For taking average, set Mmax>=last_mass_point+200. 
                const int n = 35; //#mass points
                double M[35]={200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600,3800,4000}; //mass points

		//resolution
//		int massBin[1]={400}; //mass points for fitting
                int massBin[25]={400,450,500,550,600,700,750,800,900,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600,3800,4000};
		int maxMassBin=sizeof(massBin)/sizeof(*massBin);;
		char tempmass[100],tempmass2[100];
		double width[100],xMin[100],xMax[100],yMin[100],yMax[100]; //range of individual fit
		RooRealVar x= RooRealVar("reso","m_{reco}-m_{true}",0.,-2000,2000,"GeV"); //center at zero
		RooRealVar y= RooRealVar("reco","m_{reco}-m_{true}+m_{pole}",0.,0,5000,"GeV"); //center at pole mass
		RooRealVar w = RooRealVar("myW","myW",1.0,-2000.,1500.);
		RooCategory massrc = RooCategory("massrc","massrc");
		RooDoubleCB* DCBall[100]; 
                RooDoubleCB* DCBall_shift[100];

	public: 

		Sample sample=spin0_ggH;
		HighMass(Sample sample);
		void readCandTree(const char* fin);
		void makeSelectedTree(const char* fin);
		void plotEfficiency();
                void makeSelectedTree_4l();
                void plotEfficiency_4l();
                void plot2Dtemplate();
		void readResoDataset(const char* ctype);
		void fitReso(const char* ctype);
		void checkReso(const char* ctype);
};
#endif

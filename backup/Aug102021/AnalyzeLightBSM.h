#ifndef AnalyzeLightBSM_H
#define AnalyzeLightBSM_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TH2.h"
#include <TProfile.h>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include"TGraphErrors.h"
#include"TGraphAsymmErrors.h"

class AnalyzeLightBSM : public NtupleVariables{

 public:
  AnalyzeLightBSM(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data", const char *sample="sample");
  ~AnalyzeLightBSM();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *,const char *);
  void     BookHistogram(const char *, const char *);
  int getBinNoV4(int);
  int getBinNoV7(int);
  int getBinNoV6(int);
  TLorentzVector getBestPhoton();
  //Long64_t transMass(float , float, float, float);
  void print(Long64_t);
  //  void findObjMatchedtoG(TLorentzVector);
  double NumEvents;

  //  double   Weight;
  double wt,lumiInfb=35.9;//35.86;//36.814;//35.862824;//36.814;
  int N_0emt=0,N_all=0,N_1e=0,N_2e=0,N_1m=0,N_2m=0,N_1t=0,N_2t=0;
  int n_electrons,n_muon,n_tau;
  Int_t zeroleptons =0, Nzeroleptons =0, Nelectrons =0,Nmuons = 0,Ntaus =0;
  Int_t lept_zeroleptons =0, lept_Nzeroleptons =0, lept_Nelectrons =0,lept_Nmuons = 0,lept_Ntaus =0;
  Int_t Iso_zeroleptons =0, Iso_Nzeroleptons =0, Iso_Nelectrons =0,Iso_Nmuons = 0,Iso_Ntaus =0;
  bool isSignal=false;
  vector<double> METLowEdge={200,270,350,450,750,2000};
  vector<double> METLowEdge1={100,200,270,350,450,750,2000};
  vector<double> METLowEdge2={100,200,270,350,450,2000};
  TH1F *h_isotrack;
  TH1F *h_zerolepton;
  TFile *oFile;
  TH1F *h_events;
  TH1D *h_nEvts;
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_NJets;
  TH1D *h_NbJets;
  TH1F *h_MeT;
  TH1F *h_PhoPt;
  TH1F *h_HT;
  TH1F *h_check_PhoPt;
  //
  TH1D *h_SBins_v7_CD;
  
};
#endif

#ifdef AnalyzeLightBSM_cxx

void AnalyzeLightBSM::BookHistogram(const char *outFileName, const char *N2_mass) {
  int chi2_mass= atoi(N2_mass);
  //  char hname[200], htit[200];
  double xlow = 0.0,  xhigh = 3200, xhigh1 = 3500,xhigh2=300;//4.0*(2350-chi2_mass);
  //  int nbins = 2000;
  char name[100],title[100];

  Double_t xbins_PhotPt[97]={};//{20,25,30,35,40,,7,10,20,30,40,50,80,90,100,150};
  for(int i_bin=0;i_bin<97;i_bin++)
    {
      if(i_bin<=40) xbins_PhotPt[i_bin]=0+(5*(i_bin));
      if(i_bin>40 && i_bin<=70) xbins_PhotPt[i_bin]=200+(10*(i_bin-40));
      if(i_bin>70) xbins_PhotPt[i_bin]=500+(20*(i_bin-70));
      cout<<xbins_PhotPt[i_bin]<<"\t"<<i_bin<<endl;
    }
  //   h_PhoPt=new TH1F("h_PhoPt","",76,xbins_PhotPt);
  
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  h_PhoPt=new TH1F("h_PhoPt","Photon -Pt >20GeV ",96,xbins_PhotPt);
  h_MeT=new TH1F ("h_MeT","MET >100",50,0,1500);
  h_NJets=new TH1D("h_NJets","N hadronic jets (>=2)",20,0,20);
  h_NbJets=new TH1D("h_NbJets","B-tagged jets",15,0,15);
  h_HT= new TH1F("h_HT","Sum of pt for all hadronic jets",100,0,10000);
  h_check_PhoPt= new TH1F("h_check_PhoPt","check Pt distribution",100,0,2000);
  cout<<h_PhoPt->GetNbinsX()<<"\t"<<endl;

  h_SBins_v7_CD = new TH1D("AllSBins_v7_CD","search bins v7:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]_CD",31,0.5,31.5);

  
  h_events = new TH1F("h_events","binwise filling of events",10,0,10);
  
  const char *ee[10] = {"h-h","e-h","t-h","m-h","e-e","t-t","m-m","e-m","e-t","t-m"};
  for(Int_t j =1; j<11;j++)
    {h_events->GetXaxis()->SetBinLabel(j,ee[j-1]);
    }
  h_isotrack = new TH1F("h_isotrack","binwise filling of events",10,0,10);

  //  const char *ee[10] = {"h-h","e-h","t-h","m-h","e-e","t-t","m-m","e-m","e-t","t-m"};
  for(Int_t j =1; j<11;j++)
    {h_isotrack->GetXaxis()->SetBinLabel(j,ee[j-1]);
    }
  h_zerolepton = new TH1F("h_zerolepton","binwise filling of events",10,0,10);

  //  const char *ee[10] = {"h-h","e-h","t-h","m-h","e-e","t-t","m-m","e-m","e-t","t-m"};
  for(Int_t j =1; j<11;j++)
    {h_zerolepton->GetXaxis()->SetBinLabel(j,ee[j-1]);
    }

}


AnalyzeLightBSM::AnalyzeLightBSM(const TString &inputFileList, const char *outFileName, const char* dataset, const char* N2_mass) {
  string nameData=dataset;//vvv

  TChain *tree = new TChain("PreSelection");
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  if(nameData!="signalH") nameData="BG";
  if(nameData=="signalH") nameData="signal";
  cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  BookHistogram(outFileName, N2_mass);
  
}

Bool_t AnalyzeLightBSM::FillChain(TChain *chain, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in this tree : " << chain->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t AnalyzeLightBSM::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }
  return centry;
}

AnalyzeLightBSM::~AnalyzeLightBSM() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif


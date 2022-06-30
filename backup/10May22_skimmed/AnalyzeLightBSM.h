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
  int getBinNoV6_WithOnlyBLSelec(int,int);
  TLorentzVector getBestPhoton();
  double getGendRLepPho();
  double getGenLep(TLorentzVector);
  //Long64_t transMass(float , float, float, float);
  void print(Long64_t);
  //  void findObjMatchedtoG(TLorentzVector);
  double NumEvents;

  //  double   Weight;
  double wt,lumiInfb=35.9;//35.86;//36.814;//35.862824;//36.814;
  int bestPhotonIndxAmongPhotons=-100;
  int N_0emt=0,N_all=0,N_1e=0,N_2e=0,N_1m=0,N_2m=0,N_1t=0,N_2t=0;
  int n_electrons,n_muon,n_tau;
  Int_t zeroleptons =0, Nzeroleptons =0, Nelectrons =0,Nmuons = 0,Ntaus =0;
  Int_t lept_zeroleptons =0, lept_Nzeroleptons =0, lept_Nelectrons =0,lept_Nmuons = 0,lept_Ntaus =0;
  Int_t Iso_zeroleptons =0, Iso_Nzeroleptons =0, Iso_Nelectrons =0,Iso_Nmuons = 0,Iso_Ntaus =0;
  bool isSignal=false;
  vector<double> METLowEdge={200,270,350,450,750,2000};
  vector<double> METLowEdge1={100,200,270,350,450,750,2000};
  vector<double> METLowEdge2={100,200,270,350,450,2000};
  vector<double> METLowEdge_v3={200,300,370,450,600,750,900};
  vector<double> METLowEdge_v3_1={200,300,370,450,600,750};
  TH1F *h_selectBaselineYields_;
  TH1F *h_isotrack;
  TH1F *h_zerolepton;
  TFile *oFile;
  TH1F *h_events;
  TH1D *h_nEvts;
  TH1I *h_RunNum;
  TH1D *h_intLumi;
  TH1D *h_Njets[25];
  TH1D *h_Nbjets[25];
  TH1F *h_MET_[25];
  TH1F *h_PhotonPt[25];
  TH1F *h_Mt_PhoMET[25];
  TH1F *h_dPhi_PhoMET[25];
  TH1F *h_St[25];
  TH1F *h_HT[25];
  TH2F *h_njets_vs_ST[25];
  TH2F *h_njets_vs_HT[25];
  TH2F *h_ST_vs_ptPho[25];
  TH1F *h_HT_njets_2_4;
  TH1F *h_HT_njets_5_6;
  TH1F *h_HT_njets_7;
  TH1F *h_HT_njets_2_4_Met250;
  TH1F *h_HT_njets_5_6_Met250;
  TH1F *h_HT_njets_7_Met250;

  //Met>100GeV
  TH1D *h_NJets;
  TH1D *h_NbJets;
  TH1F *h_MeT;
  TH1F *h_PhoPt;
  //  TH1F *h_HT;
  /* TH1F *h_dPhi_PhoMET; */
  /* TH1F *h_Mt_PhoMET; */
  TH1F *h_check_PhoPt;
  TH1F *h_minDR_PhoLep;
  TH1F *h_madminPhotDeltaR;
  //  TH1F *h_selectBaselineYields_;
  //MET>250GeV
  TH1D *h_NJets_Met250GeV;
  TH1D *h_NbJets_Met250GeV;
  TH1F *h_MeT_Met250GeV;
  TH1F *h_HT_Met250GeV;
  TH1F *h_check_PhoPt_Met250GeV;
  TH1F *h_Mt_PhoMET_Met250GeV;
  TH1F *h_dPhi_PhoMET_Met250GeV;

  //MET>600GeV
  TH1D *h_NJets_Met600GeV;
  TH1D *h_NbJets_Met600GeV;
  TH1F *h_MeT_Met600GeV;
  TH1F *h_HT_Met600GeV;
  TH1F *h_check_PhoPt_Met600GeV;
  TH1F *h_Mt_PhoMET_Met600GeV;
  TH1F *h_dPhi_PhoMET_Met600GeV;
  // 2d plots to check jet multiplicity
  
  /* TH2F *h_njets_vs_ST; */
  /* TH2F *h_njets_vs_HT; */
  /* TH2F *h_ST_vs_ptPho; */

  TH1D *h_SBins_v7_CD;
  TH1F *h_dPhi_Met_hadJets[6];
  TH1F *h_dPhi_Met_hadJets_after[6];
  TH2F *h_dPhivsMET[6];
  TH2F *h_dPhivsMET_after[6];
  TH2F *h_dPhi1vsdPhi2;
  TH1F *h_madminPhotonDeltaR_;
  TH1F *h_madminPhotonDeltaR_after;
  TH1F *h_minPho_lep;
  TH1F *h_minPho_lep_after;
  TH1F *h_madHT;
  TH1F *h_madHT_after;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_Met100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_PrevAna;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250_Pt100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250_Pt100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250_Pt100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met100;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250;
  TH1F *h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250_Pt100;

  TH1F *h_mvaResponse_baseline[24];
  TH1F *h_mvaResponse_4variable[24];
  TH1F *h_mvaResponse_7variable[24];
  TH1F *h_mvaResp_7variable_100tree[24];
  TH1F *h_dPhi_MetJet[24];
  TH1F *h_mvaResponse;

  TH1F *h_Sbins_v6_withOnlyBL_njetsvsHTbin;
};
#endif

#ifdef AnalyzeLightBSM_cxx

void AnalyzeLightBSM::BookHistogram(const char *outFileName, const char *N2_mass) {
  int chi2_mass= atoi(N2_mass);
  //  char hname[200], htit[200];
  double xlow = 0.0,  xhigh = 3200, xhigh1 = 3500,xhigh2=300;//4.0*(2350-chi2_mass);
  //  int nbins = 2000;
  char name[100],title[100];
  char hname[1000],hname1[1000], hname1_2d[1000],hname_2d[10000],hname_njets[10000],hname_nBjets[10000], hname_Met[10000],hname_PhoPt[10000],hname_Mt_phopt[10000],hname_dPhi[10000],hname_st[1000],hname_ht[1000],hname_njet_vs_ST[1000],hname_njet_vs_HT[1000],hname_ST_vs_ptPho[1000];
  //  const char *baseline[25]= {"Nocut","skim","bkg_comp","Met-filter","veto-lep","veto-iso","dPhi_Met","pt_jet","ST_300","Met_250","pT_100","nocut","HT_1TeV_Met100","HT_1TeV_Met250","HT_1TeV_Met250_pt_100","HT_15TeV_Met100","HT_15TeV_Met250","HT_15TeV_Met250_pt_100","HT_175TeV_Met100","HT_175TeV_Met250","HT_175TeV_Met250_pt_100","HT_2TeV_Met100","HT_2TeV_Met250","HT_2TeV_Met250_pt_100","nocut_sam"};//"st_300_Met100","pt_st_Met_250","st_300_Met250","nocut"};
  Double_t xbins_PhotPt[97]={};//{20,25,30,35,40,,7,10,20,30,40,50,80,90,100,150};

  //  const char *baseline[25]={"Nocut","nphotons_1","Phot_pT_30","nHadJets_2","phojet_match","bkg_cmp","cleanin_filters","lep_veto","iso_trk","dphi_MET","MET_100","ST_300","MET_250","pho_pt_100","nocut","HT_1TeV_Met100","HT_1TeV_Met250","HT_1TeV_Met250_pt_100","HT_15TeV_Met100","HT_15TeV_Met250","HT_15TeV_Met250_pt_100","HT_175TeV_Met100","nocut_sam","basic_sam"};//"st_300_Met100","pt_st_Met_250","st_300_Met250","nocut"
  const char *baseline[24]= {"Nocut","skim","bkg_comp","Met-filter","veto-lep","veto-iso","dPhi_Met","pt_jet","ST_300","BDT_cut0","BDT_cut1","BDT_cut2","BDT_cut3","BDT_cut4","BDT_cut5","BDT_cut6","BDT_cut7","Met_250","pT_100","nocut","HT_1TeV_Met100","HT_1TeV_Met250","HT_1TeV_Met250_pt_100","HT_15TeV_Met100"};
  for(int i_bin=0;i_bin<97;i_bin++)
    {
      if(i_bin<=40) xbins_PhotPt[i_bin]=0+(5*(i_bin));
      if(i_bin>40 && i_bin<=70) xbins_PhotPt[i_bin]=200+(10*(i_bin-40));
      if(i_bin>70) xbins_PhotPt[i_bin]=500+(20*(i_bin-70));
      //cout<<xbins_PhotPt[i_bin]<<"\t"<<i_bin<<endl;
    }
  //  h_selectBaselineYields_ = new TH1F("cutflows","cutflows",40,-0.5,40.5);
  
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);
  h_selectBaselineYields_ = new TH1F("cutflows","cutflows",40,-0.5,40.5);
  h_mvaResponse = new TH1F("h_mvaResponse","BDT response per event",500,-2,2);

  h_madminPhotonDeltaR_= new TH1F("h_madminPhotonDeltaR_","madMinPhotonDeltaR",200,0,5);
  h_madminPhotonDeltaR_after= new TH1F("h_madminPhotonDeltaR_after","madMinPhotonDeltaR",200,0,5);
  h_minPho_lep = new TH1F("h_minPho_lep","mindR(best photon & gen lep)",200,0,5);
  h_minPho_lep_after = new TH1F("h_minPho_lep_after","mindR(best photon & gen lep)",200,0,5);
  h_madHT = new TH1F("h_madHT","madHT distributions",400,0,20000);
  h_madHT_after = new TH1F("h_madHT_after","madHT distributions",400,0,20000);

  for(int i=0;i<24;i++)
    {
      cout<<baseline[i]<<endl;
      sprintf(hname_njet_vs_ST,"h_njets_vs_ST_%s",baseline[i]);
      sprintf(hname_njet_vs_HT,"h_njets_vs_HT_%s",baseline[i]);
      sprintf(hname_ST_vs_ptPho,"h_ST_vs_ptPho_%s",baseline[i]);

      sprintf(hname_njets,"h_NhadJets_%s",baseline[i]);
      sprintf(hname_nBjets,"h_NBJets_%s",baseline[i]);
      sprintf(hname_Met,"h_MET_%s",baseline[i]);
      sprintf(hname_PhoPt,"h_PhoPt_%s",baseline[i]);
      sprintf(hname_Mt_phopt,"h_Mt_phoMET_%s",baseline[i]);
      sprintf(hname_dPhi,"h_dPhi_phoMet_%s",baseline[i]);
      sprintf(hname_st,"h_St_%s",baseline[i]);
      sprintf(hname_ht,"h_HT_%s",baseline[i]);
      h_Njets[i]= new TH1D(hname_njets, hname_njets,20,0,20);
      h_Nbjets[i]= new TH1D(hname_nBjets, hname_nBjets,15,0,15);
      h_MET_[i] = new TH1F(hname_Met,hname_Met,400,0,4000);
      h_PhotonPt[i]= new TH1F(hname_PhoPt,hname_PhoPt,200,0,2000);
      h_Mt_PhoMET[i]= new TH1F(hname_Mt_phopt,hname_Mt_phopt,500,0,2500);
      h_dPhi_PhoMET[i]= new TH1F(hname_dPhi,hname_dPhi,200,0,5);
      h_St[i]=new TH1F(hname_st,hname_st,1200,0,12000);
      h_HT[i]= new TH1F(hname_ht,hname_ht,1200,0,12000);
      h_njets_vs_ST[i]= new TH2F(hname_njet_vs_ST,hname_njet_vs_HT,20,0,20,120,0,12000);
      h_njets_vs_HT[i] = new TH2F(hname_njet_vs_HT,hname_njet_vs_HT,20,0,20,120,0,12000);
      h_ST_vs_ptPho[i]= new TH2F(hname_ST_vs_ptPho,hname_ST_vs_ptPho,100,0,2000,120,0,12000);
      sprintf(hname,"h_BDT_response_%s",baseline[i]);
      h_mvaResponse_baseline[i]= new TH1F(hname,hname,500,-2,2);
      sprintf(hname,"h_BDT_response_4variable_%s",baseline[i]);
      h_mvaResponse_4variable[i]= new TH1F(hname,hname,500,-2,2);
      sprintf(hname,"h_BDT_response_7variable_%s",baseline[i]); 
      h_mvaResponse_7variable[i]= new TH1F(hname,hname,500,-2,2); 
      sprintf(hname,"h_BDT_response_7variable_100tree_%s",baseline[i]);
      h_mvaResp_7variable_100tree[i]=new TH1F(hname,hname,500,-2,2);
      sprintf(hname,"hdPhi_MetJet_%s",baseline[i]);
      h_dPhi_MetJet[i]= new TH1F(hname,hname,200,0,5);
    }
  for(int i=0;i<6;i++)
    {
      sprintf(hname,"h_dPhi_btw_Met_%02d_HadJets",i); 
      sprintf(hname_2d,"h_dPhi_vs_Met_%02d_HadJets",i);
      sprintf(hname1,"h_dPhi_btw_Met_%02d_HadJets_After",i);
      sprintf(hname1_2d,"h_dPhi1_vs_MET_%02d_HadJets_After",i);
      h_dPhi_Met_hadJets[i]= new TH1F(hname, hname,100,0,6);
      h_dPhivsMET[i]=new TH2F(hname_2d,hname_2d,100,0,10,70,0,2100);
      h_dPhi_Met_hadJets_after[i]= new TH1F(hname1, hname1,100,0,6);
      h_dPhivsMET_after[i]=new TH2F(hname1_2d,hname1_2d,100,0,10,70,0,2100);

      //      h_dPhi1vsdPhi2[i]=new TH2F(hname1_2d,hname1_2d,100,0,6);
    }
  h_HT_njets_2_4 = new TH1F("h_HT_njets_2_4","HT distribution for nhadjet >=2 & <=4",120,0,12000);
  h_HT_njets_5_6 = new  TH1F("h_HT_njets_5_6","HT distribution for nhadjet >=5 & <=6",120,0,12000);
  h_HT_njets_7 = new  TH1F("h_HT_njets_7","HT distribution for nhadjet >=7 ",120,0,12000);
  h_HT_njets_2_4_Met250 = new TH1F("h_HT_njets_2_4_Met250","HT distribution for nhadjet >=2 & <=4",120,0,12000);
  h_HT_njets_5_6_Met250 = new  TH1F("h_HT_njets_5_6_Met250","HT distribution for nhadjet >=5 & <=6",120,0,12000);
  h_HT_njets_7_Met250 = new  TH1F("h_HT_njets_7_Met250","HT distribution for nhadjet >=7 ",120,0,12000);


  h_dPhi1vsdPhi2= new TH2F("h_dPhi1vsdPhi2","dPhi1 vs dPhi2",100,0,6,100,0,6);
  //  h_selectBaselineYields_ = new TH1F("cutflows","cutflows",10,-0.5,9.5);
  h_minDR_PhoLep = new TH1F("h_minDR_PhoLep","",300,0,2);
  h_madminPhotDeltaR = new TH1F ("h_madminPhotDeltaR","",300,0,2);
  h_PhoPt=new TH1F("h_PhoPt","Photon -Pt >20GeV ",96,xbins_PhotPt);
  h_MeT=new TH1F ("h_MeT","MET >100",50,0,1500);
  h_NJets=new TH1D("h_NJets","N hadronic jets (>=2)",20,0,20);
  h_NbJets=new TH1D("h_NbJets","B-tagged jets",15,0,15);
  //  h_HT= new TH1F("h_HT","Sum of pt for all hadronic jets",100,0,10000);
  h_check_PhoPt= new TH1F("h_check_PhoPt","check Pt distribution",100,0,2000);
  /* h_dPhi_PhoMET = new TH1F("h_dPhi_PhoMET","dPhi (Best Phot & MET)",200,0,5); */
  /* h_Mt_PhoMET = new TH1F("h_Mt_PhoMET","Mt (Photon & MET)",500,0,2500); */
  
  h_Sbins_v6_withOnlyBL_Selec= new TH1F("h_Sbins_v6_withOnlyBL_Selec","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_PrevAna= new TH1F("h_Sbins_v6_withOnlyBL_Selec_PrevAna","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_Met100= new TH1F("h_Sbins_v6_withOnlyBL_Selec_Met100","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met100","search bins SP:[(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_njetsvsHTbin = new TH1F("h_Sbins_v6_withOnlyBL_njetsvsHTbin","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250_Pt100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250_Pt100","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250_Pt100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250_Pt100","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met100","search bins SP:[(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52
							 );
  h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250_Pt100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250_Pt100","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met100","search bins SP:[(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52
							 );
  h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250_Pt100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250_Pt100","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=\
7)]",52,0,52);
  h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met100 = new TH1F("h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met100","search bins SP:[(NJ=2to4),(NJ:5or6),(NJ>=7)]",52,0,52);


  //					h_SBins_v7_CD_SP_elec1_closure = new TH1D("AllSBins_v7_CD_SP_elec1_closure","search bins SP:[0b,1b] x [(NJ=2to4),(NJ:5or6),(NJ>=7)] + EW : Wtag & Htag",10,1,11);

  
  h_MeT_Met250GeV=new TH1F ("h_MeT_Met250GeV","MET >100",50,0,1500);
  h_NJets_Met250GeV=new TH1D("h_NJets_Met250GeV","N hadronic jets (>_Met250GeV=2)",20,0,20);
  h_NbJets_Met250GeV=new TH1D("h_NbJets_Met250GeV","B-tagged jets",15,0,15);
  h_HT_Met250GeV= new TH1F("h_HT_Met250GeV","Sum of pt for all hadronic jets",100,0,10000);
  h_check_PhoPt_Met250GeV= new TH1F("h_check_PhoPt_Met250GeV","check Pt distribution",100,0,2000);
  h_dPhi_PhoMET_Met250GeV = new TH1F("h_dPhi_PhoMET_Met250GeV","dPhi (Best Phot & MET)",200,0,5);
  h_Mt_PhoMET_Met250GeV = new TH1F("h_Mt_PhoMET_Met250GeV","Mt (Photon & MET)",500,0,2500);

  h_MeT_Met600GeV=new TH1F ("h_MeT_Met600GeV","MET >100",50,500,1500);
  h_NJets_Met600GeV=new TH1D("h_NJets_Met600GeV","N hadronic jets (>_Met600GeV=2)",20,0,20);
  h_NbJets_Met600GeV=new TH1D("h_NbJets_Met600GeV","B-tagged jets",15,0,15);
  h_HT_Met600GeV= new TH1F("h_HT_Met600GeV","Sum of pt for all hadronic jets",100,0,10000);
  h_check_PhoPt_Met600GeV= new TH1F("h_check_PhoPt_Met600GeV","check Pt distribution",100,0,2000);
  h_dPhi_PhoMET_Met600GeV = new TH1F("h_dPhi_PhoMET_Met600GeV","dPhi (Best Phot & MET)",200,0,5);
  h_Mt_PhoMET_Met600GeV = new TH1F("h_Mt_PhoMET_Met600GeV","Mt (Photon & MET)",500,0,2500);



  //  cout<<h_PhoPt->GetNbinsX()<<"\t"<<endl;
 
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
  //  TDirectory * dir = new TDirectory("TreeMaker2");
    TChain *tree = new TChain("PreSelection");
  //  TChain *tree = new TChain("PreSelection");
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


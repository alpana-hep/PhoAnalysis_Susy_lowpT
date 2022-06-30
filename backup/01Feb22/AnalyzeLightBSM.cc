#define AnalyzeLightBSM_cxx
#include "AnalyzeLightBSM.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include"TGraphErrors.h"
#include"TGraphAsymmErrors.h"

using namespace std;

int main(int argc, char* argv[])
{

  if (argc < 4) {
    cerr << "Please give 4 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" "<<"process"<< endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];
  const char *sample=argv[4];
  AnalyzeLightBSM ana(inputFileList, outFileName, data,sample);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList,sample);

  return 0;
}

void AnalyzeLightBSM::EventLoop(const char *data,const char *inputFileList, const char *sample) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;
  //Weight 
  //  NumEvents = nentries;
  //cout<<Weight<<endl;
  //Weight = CrossSection/NumEvents;    
  //  wt = Weight*1000.0*35.9;
  char* outFileName = new char[1000];
  sprintf(outFileName,"./skimmed_ntuple_%s_%s.root", data,sample);//,Min_, Max_); // the name of the file where you write thetree.                                                                                                                                            
  TFile* outfile = TFile::Open(outFileName,"recreate");
  skim_tree = new TTree("Pre_Selection","variables for BDT training");
  NtupleVariables::init_piTree();

  TString s_data=data;
  TString s_sample= sample;
  Long64_t nbytes = 0, nb = 0; 
  int decade = 0;
  int count=0;
  double lumiInfb=137.19;
  int count_QCD=0;
  Long64_t nSurvived = 0,bkg_comp=0,MET_rej=0,nocut=0;
  int genphomatch_after=0,genphomatch_before=0;
  bool genphocheck=false;
  bool v17=true, v12=false;
  bool EWselection=true;

  if(s_data.Contains("2016")) lumiInfb=35.922;
  if(s_data.Contains("2017")) lumiInfb=41.529;
  if(s_data.Contains("2018")) lumiInfb=59.74;
  if(s_data.Contains("signal"))lumiInfb= 137.19;
  // int count_QCD=0;
  // Long64_t nSurvived = 0,bkg_comp=0,MET_rej=0,nocut=0;
  cout<<s_sample<<nentries<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
      double progress = 10.0 * jentry / (1.0 * nentries);
      int k = int (progress);
      if (k > decade)
        cout << 10 * k << " %" << endl;
      decade = k;


      // ===============read this entry == == == == == == == == == == ==                                                                                                   
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // ========================================================================


      wt = Weight*lumiInfb*1000.0;
      // if (jentry%1000==0)
      // 	cout<<wt<<"\t"<<"wt"<<"\t"<<Weight<<"\t"<<"weight"<<"\t"<<CrossSection<<"\t"<<"Xsec"<<"\t"<<"luminosity"<<lumiInfb<<"\t"<<"events"<<nentries<<endl;
      //cout<<CrossSection<<"\t"<<Weight<<endl;
      //cout<<"check1"<<endl;  
      h_selectBaselineYields_->Fill("No cuts, evt in 1/fb",wt);
      nocut++;
     
      //my code starts from here .... 19Aug2020
    int branch_size = (*GenParticles).size();
    int ele_branch=(*Electrons).size();
    int pho_branch = (*Photons).size();
    //variable to be used in getting dR
    float iGenEle_eta =99999.0, iGenEle_phi=99999.0, iRecEle_eta =99999.0, iRecEle_phi=99999.0, iRecphot_eta =99999.0, iRecphot_phi=99999.0,iGen_Wpt=99999.0,iGen_Wp=99999.0,iGen_WEta=99999.0,iGen_WPhi=99999.0;
    float dR_Ele=0, min_dR=9999, min_dR_pho=9999;
    int count_genEle=0,count_recEle=0;

    //getting the besphoton

    TLorentzVector genPho1,genLep1;
    int leadGenPhoIdx=-100;

    TLorentzVector bestPhoton=getBestPhoton();
    if(bestPhotonIndxAmongPhotons<0) continue;
    bool bestPhoHasPxlSeed=true;
    if((*Photons_hasPixelSeed)[bestPhotonIndxAmongPhotons]<0.001) bestPhoHasPxlSeed=false;
    if( bestPhoHasPxlSeed ) continue;
    
    //if(bestPhoton.size()==0) continue;
    int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0;
    double minDR=99999,ST=0,Ht=0;
    vector<TLorentzVector> hadJets;
    //      double gendRLepPho = getGendRLepPho();                                                                                                                                                         
    for(int i=0;i<Jets->size();i++)
      {
        if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
          double dR=bestPhoton.DeltaR((*Jets)[i]);
          if(dR<minDR){minDR=dR;minDRindx=i;}
	}
      }
    for(int i=0;i<Jets->size();i++)
      {
	if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
          if( !(minDR < 0.3 && i==minDRindx) )
            hadJets.push_back((*Jets)[i]);}
      }
    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    if(hadJets.size() == 0) continue;

    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());Ht=Ht+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    sortTLorVec(&hadJets);
    //    cout<<MET<<endl;
    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();
    //transverse mass of best photon+MET                                                                                                                                                                 
    double mTPhoMET=sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
    // Dphi between MET and Best Photon                                                                                                                                                                   
    double dPhi_PhoMET= abs(DeltaPhi(METPhi,bestPhoton.Phi()));
    //if (nHadJets<2) cout<<"wrong event"<<endl;
    //filling the histograms w/o any baseline selections applied
    h_Njets[0]->Fill(nHadJets,wt);
    h_Nbjets[0]->Fill(BTags,wt);
    h_MET_[0]->Fill(MET,wt);
    h_PhotonPt[0]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[0]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[0]->Fill(dPhi_PhoMET,wt);
    h_St[0]->Fill(ST,wt);
    h_HT[0]->Fill(Ht,wt);
    h_njets_vs_ST[0]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[0]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[0]->Fill(ST,bestPhoton.Pt(),wt);
    //    cout<<"check1"<<endl;
    if(bestPhoton.Pt()>30) 
    h_selectBaselineYields_->Fill("pt>20",wt);
    else continue;
    if (nHadJets>=2) 
    h_selectBaselineYields_->Fill("njets>=2",wt);
    else continue;
    if(MET>100) 
    h_selectBaselineYields_->Fill("MET>100",wt);
    else continue;

    h_selectBaselineYields_->Fill("skims",wt);
    h_Njets[1]->Fill(nHadJets,wt);
    h_Nbjets[1]->Fill(BTags,wt);
    h_MET_[1]->Fill(MET,wt);
    h_PhotonPt[1]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[1]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[1]->Fill(dPhi_PhoMET,wt);
    h_St[1]->Fill(ST,wt);
    h_HT[1]->Fill(Ht,wt);
    h_njets_vs_ST[1]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[1]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[1]->Fill(ST,bestPhoton.Pt(),wt);

    if( s_sample.Contains("TTJets-HT") && madHT<600) continue;
    if( s_sample.Contains("TTJets-inc") && madHT>600) continue;
    if(!genphocheck)
      {
        genphomatch_before++;
        double mindr_Pho_genlep=getGenLep(bestPhoton);
        if( s_sample.Contains("TTG") )
          {
            if(!hasGenPromptPhoton)
              {
                if(jentry==0)cout<<"**********processing "<<s_sample<<" with non-prompt Gen photon"<<endl;
              }
            else if(hasGenPromptPhoton)
              {
                if(!(madMinPhotonDeltaR >= 0.3 && mindr_Pho_genlep >=0.5 ))continue;
              }
          }//Gen prompt                                                                                                                                                                                  
        if(s_sample.Contains("WG"))
          {
            if(!hasGenPromptPhoton)
              {
                if(jentry==0)cout<<"**********processing "<<s_sample<<" with non-prompt Gen photon"<<endl;
              }
            else if(hasGenPromptPhoton)
              {
                if(!(madMinPhotonDeltaR >= 0.5 && mindr_Pho_genlep >=0.5 ))continue;
              }
          }//Gen prompt                                                                                                                                                                                   
        if(s_sample.Contains("WJets"))
          {
            if(!hasGenPromptPhoton)
              {
		if(jentry==0)cout<<"**********processing "<<s_sample<<" with non-prompt Gen photon"<<endl;
              }
            else if(hasGenPromptPhoton)
              {
                if(!(madMinPhotonDeltaR < 0.5 || mindr_Pho_genlep < 0.5)) continue;
              }
          }
        if(s_sample.Contains("TTJets-HT") || s_sample.Contains("TTJets-inc") || s_sample.Contains("TTJets2_v17"))
          {
            if(!hasGenPromptPhoton)
              {
                if(jentry==0)cout<<"**********processing "<<s_sample<<" with non-prompt Gen photon"<<endl;
              }
            else if(hasGenPromptPhoton)
              {
                if(!(madMinPhotonDeltaR < 0.3 || mindr_Pho_genlep < 0.5)) continue;
              }
          }
        if(hasGenPromptPhoton && (s_sample.Contains("GJets")))
          {
            if(!(madMinPhotonDeltaR>0.4)) continue;

          }
        if(hasGenPromptPhoton && (s_sample.Contains("QCD")))
          {
            if((madMinPhotonDeltaR>0.4 && hasGenPromptPhoton)) continue;
          }
	if(hasGenPromptPhoton && (s_sample.Contains("ZG")))
          {
            if(!(madMinPhotonDeltaR>0.5)) continue;
          }
        if(hasGenPromptPhoton && (s_sample.Contains("ZJets")))
          {
            if(!(madMinPhotonDeltaR<=0.5)) continue;
          }
        genphomatch_after++;

      }

    h_selectBaselineYields_->Fill("after bkg cmp",wt);
    bkg_comp++;
    h_Njets[2]->Fill(nHadJets,wt);
    h_Nbjets[2]->Fill(BTags,wt);
    h_MET_[2]->Fill(MET,wt);
    h_PhotonPt[2]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[2]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[2]->Fill(dPhi_PhoMET,wt);
    h_St[2]->Fill(ST,wt);
    h_HT[2]->Fill(Ht,wt);
    h_njets_vs_ST[2]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[2]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[2]->Fill(ST,bestPhoton.Pt(),wt);

    //MEt cleaning filters
    if(s_data.Contains("2016") || s_data.Contains("2017") || s_data.Contains("2018"))
      {
        // if(!(PrimaryVertexFilter ==1 && globalSuperTightHalo2016Filter == 1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && EcalDeadCellTriggerPrimitiveFilter == 1 && BadPFMuonFilter && ecalBadCalibReducedExtraFilter && NVtx>0 && eeBadScFilter)) continue;
	if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadPFMuonFilter && NVtx > 0) ) continue;

      }
    h_selectBaselineYields_->Fill("MetCleaning",wt);
    

    h_Njets[3]->Fill(nHadJets,wt);
    h_Nbjets[3]->Fill(BTags,wt);
    h_MET_[3]->Fill(MET,wt);
    h_PhotonPt[3]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[3]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[3]->Fill(dPhi_PhoMET,wt);
    h_St[3]->Fill(ST,wt);
    h_HT[3]->Fill(Ht,wt);
    h_njets_vs_ST[3]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[3]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[3]->Fill(ST,bestPhoton.Pt(),wt);

    if (NElectrons == 0 && NMuons == 0 ) h_selectBaselineYields_->Fill("veto electron & Muon",wt);
    else continue;
    h_Njets[4]->Fill(nHadJets,wt);
    h_Nbjets[4]->Fill(BTags,wt);
    h_MET_[4]->Fill(MET,wt);
    h_PhotonPt[4]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[4]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[4]->Fill(dPhi_PhoMET,wt);
    h_St[4]->Fill(ST,wt);
    h_HT[4]->Fill(Ht,wt);
    h_njets_vs_ST[4]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[4]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[4]->Fill(ST,bestPhoton.Pt(),wt);

    if(isoElectronTracks==0 && isoMuonTracks ==0 && isoPionTracks==0)
      {
	h_selectBaselineYields_->Fill("Iso track",wt);
      }
    else continue;

    h_Njets[5]->Fill(nHadJets,wt);
    h_Nbjets[5]->Fill(BTags,wt);
    h_MET_[5]->Fill(MET,wt);
    h_PhotonPt[5]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[5]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[5]->Fill(dPhi_PhoMET,wt);
    h_St[5]->Fill(ST,wt);
    h_HT[5]->Fill(Ht,wt);
    h_njets_vs_ST[5]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[5]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[5]->Fill(ST,bestPhoton.Pt(),wt);



    TLorentzVector Met;
    Met.SetPtEtaPhiE(MET,0,METPhi,0);
    double mT= 0.0, dPhi_METjet1=5, dPhi_METjet2=5, dPhi_phojet1=5, dPhi_phojet2=5, dPhi_phoMET=5;
    if(hadJets.size() > 0) dPhi_phojet1 = abs(bestPhoton.DeltaPhi(hadJets[0]));
    if(hadJets.size() > 1) dPhi_phojet2 = abs(bestPhoton.DeltaPhi(hadJets[1]));
    dPhi_METjet1 = abs(Met.DeltaPhi(hadJets[0]));
    dPhi_METjet2 = abs(Met.DeltaPhi(hadJets[1]));
    dPhi_phoMET = abs(bestPhoton.DeltaPhi(Met));

    dPhi_METjet1=3.8,dPhi_METjet2=3.8;//dphi3=3.8,dphi4=3.8,dphiG_MET=3.8;                                                                                          
    Met.SetPtEtaPhiE(MET,0,METPhi,0);
    dPhi_phojet1 = abs(bestPhoton.DeltaPhi(hadJets[0]));
    dPhi_phojet2 = abs(bestPhoton.DeltaPhi(hadJets[1]));
    dPhi_phoMET = abs(bestPhoton.DeltaPhi(Met));
    dPhi_METjet1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
    dPhi_METjet2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));

    if(dPhi_METjet1 > 0.3 && dPhi_METjet2 > 0.3 )
      {
	h_selectBaselineYields_->Fill("dPhi1 & dPhi2 >= 0.3",wt);
      }
    else continue;
    h_Njets[6]->Fill(nHadJets,wt);
    h_Nbjets[6]->Fill(BTags,wt);
    h_MET_[6]->Fill(MET,wt);
    h_PhotonPt[6]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[6]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[6]->Fill(dPhi_PhoMET,wt);
    h_St[6]->Fill(ST,wt);
    h_HT[6]->Fill(Ht,wt);
    h_njets_vs_ST[6]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[6]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[6]->Fill(ST,bestPhoton.Pt(),wt);

    if(phoMatchingJetIndx>=0 && ((*Jets)[phoMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
    if(phoMatchingJetIndx<0) continue;
    h_selectBaselineYields_->Fill("additional Bhumika",wt);
    h_Njets[7]->Fill(nHadJets,wt);
    h_Nbjets[7]->Fill(BTags,wt);
    h_MET_[7]->Fill(MET,wt);
    h_PhotonPt[7]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[7]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[7]->Fill(dPhi_PhoMET,wt);
    h_St[7]->Fill(ST,wt);
    h_HT[7]->Fill(Ht,wt);
    // int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
    // h_Sbins_v6_withOnlyBL_Selec_Met100->Fill(Sbin_met100,wt);

    // if(bestPhoton.Pt()>100) h_selectBaselineYields_->Fill("Pt>100",wt);
    // else continue;
    if( (ST <= 300) ) continue;
    h_selectBaselineYields_->Fill("St>300",wt);
    h_Njets[8]->Fill(nHadJets,wt);
    h_Nbjets[8]->Fill(BTags,wt);
    h_MET_[8]->Fill(MET,wt);
    h_PhotonPt[8]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[8]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[8]->Fill(dPhi_PhoMET,wt);
    h_St[8]->Fill(ST,wt);
    h_HT[8]->Fill(Ht,wt);
    h_njets_vs_ST[8]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[8]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[8]->Fill(ST,bestPhoton.Pt(),wt);

    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
    h_Sbins_v6_withOnlyBL_Selec_Met100->Fill(Sbin_met100,wt);
   
    if(Ht>1000)
      {
	h_selectBaselineYields_->Fill("Ht>1TeV",wt);
	h_Njets[12]->Fill(nHadJets,wt);
	h_Nbjets[12]->Fill(BTags,wt);
	h_MET_[12]->Fill(MET,wt);
	h_PhotonPt[12]->Fill(bestPhoton.Pt(),wt);
	h_Mt_PhoMET[12]->Fill(mTPhoMET,wt);
	h_dPhi_PhoMET[12]->Fill(dPhi_PhoMET,wt);
	h_St[12]->Fill(ST,wt);
	h_HT[12]->Fill(Ht,wt);
	h_njets_vs_ST[12]->Fill(nHadJets,ST,wt);
	h_njets_vs_HT[12]->Fill(nHadJets,Ht,wt);
	h_ST_vs_ptPho[12]->Fill(ST,bestPhoton.Pt(),wt);

	int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
	h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met100->Fill(Sbin_met100,wt);
	if(Ht>1500)
	  {
	    h_selectBaselineYields_->Fill("Ht>1.5TeV MET>100",wt);
	    h_Njets[15]->Fill(nHadJets,wt);
	    h_Nbjets[15]->Fill(BTags,wt);
	    h_MET_[15]->Fill(MET,wt);
	    h_PhotonPt[15]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[15]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[15]->Fill(dPhi_PhoMET,wt);
	    h_St[15]->Fill(ST,wt);
	    h_HT[15]->Fill(Ht,wt);
	    h_njets_vs_ST[15]->Fill(nHadJets,ST,wt);
	    h_njets_vs_HT[15]->Fill(nHadJets,Ht,wt);
	    h_ST_vs_ptPho[15]->Fill(ST,bestPhoton.Pt(),wt);

	    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
	    h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met100->Fill(Sbin_met100,wt);
	    if(Ht>1750)
	      {
		h_selectBaselineYields_->Fill("Ht>1.75TeV MET>100",wt);
		h_Njets[18]->Fill(nHadJets,wt);
		h_Nbjets[18]->Fill(BTags,wt);
		h_MET_[18]->Fill(MET,wt);
		h_PhotonPt[18]->Fill(bestPhoton.Pt(),wt);
		h_Mt_PhoMET[18]->Fill(mTPhoMET,wt);
		h_dPhi_PhoMET[18]->Fill(dPhi_PhoMET,wt);
		h_St[18]->Fill(ST,wt);
		h_HT[18]->Fill(Ht,wt);
		h_njets_vs_ST[18]->Fill(nHadJets,ST,wt);
		h_njets_vs_HT[18]->Fill(nHadJets,Ht,wt);
		h_ST_vs_ptPho[18]->Fill(ST,bestPhoton.Pt(),wt);

		int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met100->Fill(Sbin_met100,wt);
		if(Ht>2000)
		  {
		    h_selectBaselineYields_->Fill("Ht>2TeV, MET100",wt);
		    h_Njets[21]->Fill(nHadJets,wt);
		    h_Nbjets[21]->Fill(BTags,wt);
		    h_MET_[21]->Fill(MET,wt);
		    h_PhotonPt[21]->Fill(bestPhoton.Pt(),wt);
		    h_Mt_PhoMET[21]->Fill(mTPhoMET,wt);
		    h_dPhi_PhoMET[21]->Fill(dPhi_PhoMET,wt);
		    h_St[21]->Fill(ST,wt);
		    h_HT[21]->Fill(Ht,wt);
		    h_njets_vs_ST[21]->Fill(nHadJets,ST,wt);
		    h_njets_vs_HT[21]->Fill(nHadJets,Ht,wt);
		    h_ST_vs_ptPho[21]->Fill(ST,bestPhoton.Pt(),wt);

		    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		    h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met100->Fill(Sbin_met100,wt);
		  }
	      }
	  }

      }
    if(nHadJets>=2 && nHadJets <=4)
      h_HT_njets_2_4->Fill(Ht,wt);
    else if(nHadJets>=5 && nHadJets <=6)
      h_HT_njets_5_6->Fill(Ht,wt);
    else if(nHadJets>=7)
      h_HT_njets_7->Fill(Ht,wt);

    if (MET>250)
      h_selectBaselineYields_->Fill("MET>250",wt);
       else continue;
    h_selectBaselineYields_->Fill("St>300",wt);
    h_Njets[9]->Fill(nHadJets,wt);
    h_Nbjets[9]->Fill(BTags,wt);
    h_MET_[9]->Fill(MET,wt);
    h_PhotonPt[9]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[9]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[9]->Fill(dPhi_PhoMET,wt);
    h_St[9]->Fill(ST,wt);
    h_HT[9]->Fill(Ht,wt);
    h_njets_vs_ST[9]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[9]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[9]->Fill(ST,bestPhoton.Pt(),wt);
    
    int Sbin=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);                                                                                                                                
    h_Sbins_v6_withOnlyBL_Selec->Fill(Sbin,wt);  
    if(nHadJets>=2 && nHadJets <=4)
      h_HT_njets_2_4_Met250->Fill(Ht,wt);
    else if(nHadJets>=5 && nHadJets <=6)
      h_HT_njets_5_6_Met250->Fill(Ht,wt);
    else if(nHadJets>=7)
      h_HT_njets_7_Met250->Fill(Ht,wt);

    if(Ht>1000)
      {
	h_selectBaselineYields_->Fill("Ht>1TeV & Met 250",wt);
        h_Njets[13]->Fill(nHadJets,wt);
	h_Nbjets[13]->Fill(BTags,wt);
        h_MET_[13]->Fill(MET,wt);
        h_PhotonPt[13]->Fill(bestPhoton.Pt(),wt);
        h_Mt_PhoMET[13]->Fill(mTPhoMET,wt);
        h_dPhi_PhoMET[13]->Fill(dPhi_PhoMET,wt);
        h_St[13]->Fill(ST,wt);
        h_HT[13]->Fill(Ht,wt);
	h_njets_vs_ST[13]->Fill(nHadJets,ST,wt);
	h_njets_vs_HT[13]->Fill(nHadJets,Ht,wt);
	h_ST_vs_ptPho[13]->Fill(ST,bestPhoton.Pt(),wt);

        int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
        h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250->Fill(Sbin_met100,wt);
	if(Ht>1500)
          {
            h_selectBaselineYields_->Fill("Ht>1.5TeV MET>250",wt);
            h_Njets[16]->Fill(nHadJets,wt);
            h_Nbjets[16]->Fill(BTags,wt);
            h_MET_[16]->Fill(MET,wt);
            h_PhotonPt[16]->Fill(bestPhoton.Pt(),wt);
            h_Mt_PhoMET[16]->Fill(mTPhoMET,wt);
            h_dPhi_PhoMET[16]->Fill(dPhi_PhoMET,wt);
            h_St[16]->Fill(ST,wt);
            h_HT[16]->Fill(Ht,wt);
            h_njets_vs_ST[16]->Fill(nHadJets,ST,wt);
            h_njets_vs_HT[16]->Fill(nHadJets,Ht,wt);
            h_ST_vs_ptPho[16]->Fill(ST,bestPhoton.Pt(),wt);

            int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
            h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250->Fill(Sbin_met100,wt);
            if(Ht>1750)
              {
                h_selectBaselineYields_->Fill("Ht>1.75TeV MET>250",wt);
                h_Njets[19]->Fill(nHadJets,wt);
                h_Nbjets[19]->Fill(BTags,wt);
                h_MET_[19]->Fill(MET,wt);
                h_PhotonPt[19]->Fill(bestPhoton.Pt(),wt);
                h_Mt_PhoMET[19]->Fill(mTPhoMET,wt);
                h_dPhi_PhoMET[19]->Fill(dPhi_PhoMET,wt);
		h_St[19]->Fill(ST,wt);
                h_HT[19]->Fill(Ht,wt);
		h_njets_vs_ST[19]->Fill(nHadJets,ST,wt);
                h_njets_vs_HT[19]->Fill(nHadJets,Ht,wt);
                h_ST_vs_ptPho[19]->Fill(ST,bestPhoton.Pt(),wt);

                int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
                h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250->Fill(Sbin_met100,wt);
		if(Ht>2000)
                  {
                    h_selectBaselineYields_->Fill("Ht>2TeV, MET250",wt);
                    h_Njets[22]->Fill(nHadJets,wt);
                    h_Nbjets[22]->Fill(BTags,wt);
                    h_MET_[22]->Fill(MET,wt);
                    h_PhotonPt[22]->Fill(bestPhoton.Pt(),wt);
                    h_Mt_PhoMET[22]->Fill(mTPhoMET,wt);
                    h_dPhi_PhoMET[22]->Fill(dPhi_PhoMET,wt);
                    h_St[22]->Fill(ST,wt);
                    h_HT[22]->Fill(Ht,wt);
                    h_njets_vs_ST[22]->Fill(nHadJets,ST,wt);
		    h_ST_vs_ptPho[22]->Fill(ST,bestPhoton.Pt(),wt);

                    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
                    h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250->Fill(Sbin_met100,wt);
                  }
	      }
	  }

      }
    // if( (ST <= 300) ) continue;
    // h_selectBaselineYields_->Fill("St>300",wt);
    // if(bestPhoton.Pt()>100) h_selectBaselineYields_->Fill("Pt>100",wt);
    // else continue;
    //    cout<<"check1"<<endl; 
    if( bestPhoton.Pt()<=100  )continue;
    
      h_selectBaselineYields_->Fill("MET>250 & photon pt>100 selec",wt);
    h_Njets[10]->Fill(nHadJets,wt);
    h_Nbjets[10]->Fill(BTags,wt);
    h_MET_[10]->Fill(MET,wt);
    h_PhotonPt[10]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[10]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[10]->Fill(dPhi_PhoMET,wt);
    h_St[10]->Fill(ST,wt);
    h_HT[10]->Fill(Ht,wt);
    h_njets_vs_ST[10]->Fill(nHadJets,ST,wt);
    h_njets_vs_HT[10]->Fill(nHadJets,Ht,wt);
    h_ST_vs_ptPho[10]->Fill(ST,bestPhoton.Pt(),wt);


    if(Ht>1000)
      {
        h_selectBaselineYields_->Fill("Ht>1TeV & Met 250",wt);
        h_Njets[14]->Fill(nHadJets,wt);
        h_Nbjets[14]->Fill(BTags,wt);
        h_MET_[14]->Fill(MET,wt);
        h_PhotonPt[14]->Fill(bestPhoton.Pt(),wt);
        h_Mt_PhoMET[14]->Fill(mTPhoMET,wt);
        h_dPhi_PhoMET[14]->Fill(dPhi_PhoMET,wt);
        h_St[14]->Fill(ST,wt);
        h_HT[14]->Fill(Ht,wt);
	h_njets_vs_ST[14]->Fill(nHadJets,ST,wt);
	h_njets_vs_HT[14]->Fill(nHadJets,Ht,wt);
	h_ST_vs_ptPho[14]->Fill(ST,bestPhoton.Pt(),wt);

        int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
        h_Sbins_v6_withOnlyBL_Selec_HT1TeV_Met250_Pt100->Fill(Sbin_met100,wt);
	if(Ht>1500)
	  {
	    h_selectBaselineYields_->Fill("Ht>1.5TeV & Pt>100",wt);
	    h_Njets[17]->Fill(nHadJets,wt);
	    h_Nbjets[17]->Fill(BTags,wt);
	    h_MET_[17]->Fill(MET,wt);
	    h_PhotonPt[17]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[17]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[17]->Fill(dPhi_PhoMET,wt);
	    h_St[17]->Fill(ST,wt);
	    h_HT[17]->Fill(Ht,wt);
	    h_njets_vs_ST[17]->Fill(nHadJets,ST,wt);
	    h_njets_vs_HT[17]->Fill(nHadJets,Ht,wt);
	    h_ST_vs_ptPho[17]->Fill(ST,bestPhoton.Pt(),wt);

	    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
	    h_Sbins_v6_withOnlyBL_Selec_HT15TeV_Met250_Pt100->Fill(Sbin_met100,wt);
	    if(Ht>1750)
	      {
		h_selectBaselineYields_->Fill("Ht>1.75TeV & Pt>100",wt);
		h_Njets[20]->Fill(nHadJets,wt);
		h_Nbjets[20]->Fill(BTags,wt);
		h_MET_[20]->Fill(MET,wt);
		h_PhotonPt[20]->Fill(bestPhoton.Pt(),wt);
		h_Mt_PhoMET[20]->Fill(mTPhoMET,wt);
		h_dPhi_PhoMET[20]->Fill(dPhi_PhoMET,wt);
		h_St[20]->Fill(ST,wt);
		h_HT[20]->Fill(Ht,wt);
		h_njets_vs_ST[20]->Fill(nHadJets,ST,wt);
		h_njets_vs_HT[20]->Fill(nHadJets,Ht,wt);
		h_ST_vs_ptPho[20]->Fill(ST,bestPhoton.Pt(),wt);

		int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		h_Sbins_v6_withOnlyBL_Selec_HT175TeV_Met250_Pt100->Fill(Sbin_met100,wt);

		if(Ht>2000)
		  {
		    h_selectBaselineYields_->Fill("Ht>2TeV & Pt>100",wt);
		    h_Njets[23]->Fill(nHadJets,wt);
		    h_Nbjets[23]->Fill(BTags,wt);
		    h_MET_[23]->Fill(MET,wt);
		    h_PhotonPt[23]->Fill(bestPhoton.Pt(),wt);
		    h_Mt_PhoMET[23]->Fill(mTPhoMET,wt);
		    h_dPhi_PhoMET[23]->Fill(dPhi_PhoMET,wt);
		    h_St[23]->Fill(ST,wt);
		    h_HT[23]->Fill(Ht,wt);
		    h_njets_vs_ST[23]->Fill(nHadJets,ST,wt);
		    h_njets_vs_HT[23]->Fill(nHadJets,Ht,wt);
		    h_ST_vs_ptPho[23]->Fill(ST,bestPhoton.Pt(),wt);

		    int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		    h_Sbins_v6_withOnlyBL_Selec_HT2TeV_Met250_Pt100->Fill(Sbin_met100,wt);
		  }
		
	      }
	  }

      }


   // if(phoMatchingJetIndx>=0 && ((*Jets)[phoMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
   //  if(phoMatchingJetIndx<0) continue;
   //  h_selectBaselineYields_->Fill("additional Bhumika",wt);

    bool process= false;
    if(!bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && ST>300 && nHadJets>=2 && MET > 200 && dPhi_METjet1 > 0.3 && dPhi_METjet2 > 0.3 && NElectrons==0 && NMuons==0 &&((isoElectronTracks==0)&&(isoMuonTracks==0)&&(isoPionTracks==0)))
      process =true;
    else continue;
    h_selectBaselineYields_->Fill("final",wt);


    nSurvived++;
    h_selectBaselineYields_->Fill("survived",wt);
    int Sbin_prev=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);                                                                                                            
    h_Sbins_v6_withOnlyBL_Selec_PrevAna->Fill(Sbin_prev,wt);    
    //    cout<<"check1"<<endl; 
	
		  
    }
    
    
   
    // nSurvived++;
    //}_selectBaselineYields_->Fill("survived",wt);


  cout<<nocut<<"\t"<<nSurvived<<"\t"<<bkg_comp<<endl;
  //    }
}

int AnalyzeLightBSM::getBinNoV6_WithOnlyBLSelec(int nbjets, int nHadJets)
{
  int sBin=-100,m_i=0;
  if(nbjets==0 ){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=7;}
    else if(nHadJets>=7)               { sBin=13;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=19;}
    else if(nHadJets==5 || nHadJets==6){ sBin=25;}
    else if(nHadJets>=7)               { sBin=31;}
  }
  if(sBin==0){
    for(int i=0;i<METLowEdge_v3.size()-1;i++){
      if(METLowEdge_v3[i]<199.99) continue;
      int sBin1=sBin;
      m_i++;
      if(MET >= METLowEdge_v3[i] && MET < METLowEdge_v3[i+1]){ sBin = sBin+m_i;
	break; }
      else if(MET >= METLowEdge_v3[METLowEdge_v3.size()-1])  { sBin = 7         ;
	break; }
    }
  }
  else if(sBin==7 || sBin==13 || sBin==19 || sBin==25 || sBin==31){
    int sBin1=sBin;
    for(int i=0;i<METLowEdge_v3_1.size()-1;i++){
      if(METLowEdge_v3_1[i]<199.99) continue;
      m_i++;
      if(MET >= METLowEdge_v3_1[i] && MET < METLowEdge_v3_1[i+1]){ sBin = sBin+m_i;
	break;}
      else if(MET >= METLowEdge_v3_1[METLowEdge_v3_1.size()-1])  { sBin = sBin+6;
	break; }
    }
  }

  else if(sBin==37){
    for(int i=0;i<METLowEdge_v3.size()-1;i++){
      if(METLowEdge_v3[i]<199.99) continue;
      m_i++;
      if(MET >= METLowEdge_v3[i] && MET < METLowEdge_v3[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METLowEdge_v3[METLowEdge_v3.size()-1])  { sBin = 44   ;break; }
      // else if(MET >= METLowEdge_v3[METLowEdge_v3.size()-1])  { sBin = 44   ;break; }

    }
  }

  else if(sBin==44){
    for(int i=0;i<METLowEdge_v3.size()-1;i++){
      if(METLowEdge_v3[i]<199.99) continue;
      m_i++;
      if(MET >= METLowEdge_v3[i] && MET < METLowEdge_v3[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METLowEdge_v3[METLowEdge_v3.size()-1])  { sBin = 52   ;break; }
      // else if(MET >= METLowEdge_v3[METLowEdge_v3.size()-1])  { sBin = 51   ;break; }

    }
  }
  return sBin;
}
int AnalyzeLightBSM::getBinNoV7(int nHadJets){
  int sBin=-100,m_i=0;
  if(BTags==0){
    if(nHadJets>=2 && nHadJets<=4)     { sBin=0;}
    else if(nHadJets==5 || nHadJets==6){ sBin=6;}
    else if(nHadJets>=7)               { sBin=11;}
  }
  else{
    if(nHadJets>=2 && nHadJets<=4)     { sBin=16;}
    else if(nHadJets==5 || nHadJets==6){ sBin=21;}
    else if(nHadJets>=7)               { sBin=26;}
  }
  if(sBin==0){
    for(int i=0;i<METLowEdge1.size()-1;i++){
      if(METLowEdge1[i]<99.99) continue;
      m_i++;
      if(MET >= METLowEdge1[i] && MET < METLowEdge1[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METLowEdge1[METLowEdge1.size()-1])  { sBin = 6         ;break; }
    }
  }
  else{
    for(int i=0;i<METLowEdge2.size()-1;i++){
      if(METLowEdge2[i]<99.99) continue;
      m_i++;
      if(MET >= METLowEdge2[i] && MET < METLowEdge2[i+1]){ sBin = sBin+m_i;break; }
      else if(MET >= METLowEdge2[METLowEdge2.size()-1])  { sBin = sBin+5   ;break; }
    }
  }
  return sBin;
}

TLorentzVector AnalyzeLightBSM::getBestPhoton(){
  // TLorentzVector v1;
  // vector<TLorentzVector> goodPho;
  // for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
  //   if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back( (*Photons)[iPhoton] );
  // }

  // if(goodPho.size()==0) return v1;
  // sortTLorVec(&goodPho);
  // return goodPho[0];
  vector<TLorentzVector> goodPho;
  vector<int> goodPhoIndx;
  for(int iPho=0;iPho<Photons->size();iPho++){
    if( (*Photons_fullID)[iPho] && ((*Photons_hasPixelSeed)[iPho]<0.001)) {
      goodPho.push_back( (*Photons)[iPho] );
      goodPhoIndx.push_back(iPho);
    }
  }

  int highPtIndx=-100;
  for(int i=0;i<goodPho.size();i++){
    if(i==0) highPtIndx=0;
    else if( (goodPho[highPtIndx].Pt()) < (goodPho[i].Pt()) ){highPtIndx=i;}
  }

  if(highPtIndx>=0){
    bestPhotonIndxAmongPhotons = goodPhoIndx[highPtIndx];
  }
  else bestPhotonIndxAmongPhotons = -100;
  if(highPtIndx==-100){TLorentzVector v0;return v0;}
  else return goodPho[highPtIndx];

  
}

double AnalyzeLightBSM::getGendRLepPho(){//MC only
  TLorentzVector genPho1,genLep1;
  int leadGenPhoIdx=-100;
  // vector<TLorentzVector> goodPho;
  // for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
  //   if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back((*Photons)[iPhoton]);
  // }
  // if(goodPho.size()!=0) 
  genPho1 =getBestPhoton();
  
  for(int i=0;i<GenParticles->size();i++){
     if((*GenParticles)[i].Pt()!=0){
       if( (abs((*GenParticles_PdgId)[i])==11 || abs((*GenParticles_PdgId)[i])==13 || abs((*GenParticles_PdgId)[i])==15 ) && (abs((*GenParticles_ParentId)[i])<=25) && (abs((*GenParticles_ParentId)[i])!=15) ){
	 if(genLep1.Pt() < ((*GenParticles)[i]).Pt()) genLep1 = ((*GenParticles)[i]);
       }
     }
  }//for
  if(genPho1.Pt() > 0. && genLep1.Pt() > 0.) return genLep1.DeltaR(genPho1);
  else return 1000.0;
}

double AnalyzeLightBSM::getGenLep(TLorentzVector bestPhoton){//MC only                                                                                                                                    \
                                                                                                                                                                                                           
  vector<TLorentzVector> v_genLep2;
  TLorentzVector genMu1, genEle1;
  for(int i=0 ; i < GenElectrons->size(); i++)
    {
      if((*GenElectrons)[i].Pt()!=0)
        {
          genEle1 = ((*GenElectrons)[i]);
          v_genLep2.push_back(genEle1);
        }

    }
  for(int i=0 ; i < GenMuons->size(); i++)
    {
      if((*GenMuons)[i].Pt()!=0)
        {
          genMu1 = ((*GenMuons)[i]);
          v_genLep2.push_back(genMu1);
        }
    }
  return MinDr(bestPhoton,v_genLep2);
}


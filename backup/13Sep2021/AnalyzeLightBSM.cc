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
    cerr << "Please give 4 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" "<<"N2-Mass"<< endl;
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
  TString s_data=data;
  TString s_sample= sample;
  Long64_t nbytes = 0, nb = 0; 
  int decade = 0;
  int count=0;
  double lumiInfb=137.19;
  if(s_data.Contains("2016")) lumiInfb=35.922;
  if(s_data.Contains("2017")) lumiInfb=41.529;
  if(s_data.Contains("2018")) lumiInfb=59.74;
  if(s_data.Contains("signal"))lumiInfb= 137.19;
  int count_QCD=0;
  Long64_t nSurvived = 0,bkg_comp=0,MET_rej=0,nocut=0;
  int genphomatch_after=0,genphomatch_before=0;
  bool genphocheck=false;

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
      //cout<<CrossSection<<"\t"<<Weight<<endl;
      //cout<<madMinPhotonDeltaR<<endl;  
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
    double minDR=99999,ST=0;
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
    for(int i=0;i<hadJets.size();i++){
      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    }
    //    cout<<MET<<endl;
    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();
    //transverse mass of best photon+MET                                                                                                                                                                  
    sortTLorVec(&hadJets);

    double mTPhoMET=sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));
    // Dphi between MET and Best Photon                                                                                                                                                                   
    double dPhi_PhoMET= abs(DeltaPhi(METPhi,bestPhoton.Phi()));
    if (nHadJets<2) cout<<"wrong event"<<endl;
    //filling the histograms w/o any baseline selections applied
    h_Njets[13]->Fill(nHadJets,wt);
    h_Nbjets[13]->Fill(BTags,wt);
    h_MET_[13]->Fill(MET,wt);
    h_PhotonPt[13]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[13]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[13]->Fill(dPhi_PhoMET,wt);
    h_St[13]->Fill(ST,wt);
	       


    // stroing all the leptons in it
    // TLorentzVector genEle1, genMu1;
    // vector<TLorentzVector> v_genLep2;
    // for(int i=0 ; i < GenElectrons->size(); i++)
    //   {
    // 	if((*GenElectrons)[i].Pt()!=0)
    // 	  {	     
    // 	    genEle1 = ((*GenElectrons)[i]);
    // 	    v_genLep2.push_back(genEle1);
    // 	   }	      
    //   }
    // for(int i=0 ; i < GenMuons->size(); i++)
    //   {
    // 	if((*GenMuons)[i].Pt()!=0)
    // 	  {
    // 	    genMu1 = ((*GenMuons)[i]);
    // 	    v_genLep2.push_back(genMu1);
    // 	  }
    //   }
    // h_madHT->Fill(madHT,wt);
    // h_minPho_lep->Fill(MinDr(bestPhoton,v_genLep2),wt);
    // h_madminPhotonDeltaR_->Fill(madMinPhotonDeltaR,wt);
    //cout<<v_genLep2.size()<<endl;
    ///        BACKGROUND COMPOSITION: Removing overlapping events (W jets & WGJets, TTG & TTJets, ZG, ZJets)      /////
    if( s_sample.Contains("TTJets-HT") && madHT<600) continue;
    if( s_sample.Contains("TTJets-inc") && madHT>600) continue;
    
    // if(s_sample.Contains("QCD_Jets"))
    //   {
    // 	if(madMinPhotonDeltaR>0.4 &&  hasGenPromptPhoton) continue;
    // 	if(jentry<3) cout<<"QCD jets"<<endl;
    //   }
    // else  if (s_sample.Contains("GJets_DR"))
    //   {
    // 	if(madMinPhotonDeltaR<0.4 &&  hasGenPromptPhoton) continue;
    // 	if(jentry<3) cout<<"G jets"<<endl;
    //   }
    // else if(s_sample.Contains("ZJets"))
    //   {
    // 	if(hasGenPromptPhoton && madMinPhotonDeltaR>0.5) continue;
    //     if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.3 ";
    //   }
    // else if(s_sample.Contains("ZGJets"))
    //   { if(hasGenPromptPhoton && madMinPhotonDeltaR < 0.5)continue;
    //   }
    // else if( s_sample.Contains("TTJets-HT") ||  s_sample.Contains("TTJets-inc"))
    //   {
    // 	if(hasGenPromptPhoton){
    // 	  if(!(madMinPhotonDeltaR < 0.3 || MinDr(bestPhoton,v_genLep2) < 0.5)) continue;
    // 	}   
    //   }
    // else if( s_sample.Contains("TTGJets"))// ||  s_sample.Contains("ZGJets"))
    //   {
    // 	if(hasGenPromptPhoton){
    // 	  if(!(madMinPhotonDeltaR >= 0.3 && MinDr(bestPhoton,v_genLep2) >=0.5 ))continue;  }
    //   }
    // else if(s_sample.Contains("WGJets"))
    //   {
    // 	if(hasGenPromptPhoton){
    // 	  if(!(madMinPhotonDeltaR >= 0.5 && MinDr(bestPhoton,v_genLep2) >=0.5 ))continue;
    // 	}
    // 	if(jentry<3) cout<<"WGjets"<<endl;
    //   }
    // else if(s_sample.Contains("WJets"))
    //   {
    // 	if(hasGenPromptPhoton){
    // 	  if(!(madMinPhotonDeltaR < 0.5 || MinDr(bestPhoton,v_genLep2) < 0.5)) continue;}
    //   }

    if(!genphocheck)
      {
        genphomatch_before++;
        double mindr_Pho_genlep=getGenLep(bestPhoton);
        if(s_sample=="genprompt" || s_sample.Contains("TTG") )
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
        if(s_sample=="genprompt" || s_sample.Contains("WG"))
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
      

    ////    BASELINE SELECTION STARTS HERE   ///////////
    // if(bestPhoton.Pt()<20) continue;
    // h_selectBaselineYields_->Fill("pt>20",wt);

    // if (nHadJets<2) continue;
    // h_selectBaselineYields_->Fill("njets>=2",wt);

    // if(MET<=100) continue;
    // h_selectBaselineYields_->Fill("MET>100",wt);


    //MET cleaning filters
    //  cout<<PrimaryVertexFilter<<endl;
    // if(s_data.Contains("2018")
    //   {
    // 	if(!(PrimaryVertexFilter ==1 && 
    //   }
    //    if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadPFMuonFilter && NVtx > 0) ) continue;
    if(s_data.Contains("2016") || s_data.Contains("2017") || s_data.Contains("2018"))
      {
	if(!(CSCTightHaloFilter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadPFMuonFilter && NVtx > 0) ) continue;
	//  if(!(PrimaryVertexFilter ==1 && globalSuperTightHalo2016Filter == 1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && EcalDeadCellTriggerPrimitiveFilter == 1 && BadPFMuonFilter && ecalBadCalibReducedExtraFilter && NVtx>0 && eeBadScFilter)) continue;
      }
    h_selectBaselineYields_->Fill("MetCleaning",wt);

    if(bestPhoton.Pt()>100)     h_selectBaselineYields_->Fill("Good #gamma with Pt > 100",wt);
    else continue;

    //    if(bestPhoton.Pt()<100) continue;
    h_selectBaselineYields_->Fill("pt>100",wt);
    if (NElectrons == 0 && NMuons == 0 ) h_selectBaselineYields_->Fill("veto electron & Muon",wt);
    else continue;

    // if(NMuons ==0 && NElectrons==0)
    //   {
    //h_selectBaselineYields_->Fill("veto lep",wt);

	h_Njets[2]->Fill(nHadJets,wt);
	h_Nbjets[2]->Fill(BTags,wt);
	h_MET_[2]->Fill(MET,wt);
	h_PhotonPt[2]->Fill(bestPhoton.Pt(),wt);
	h_Mt_PhoMET[2]->Fill(mTPhoMET,wt);
	h_dPhi_PhoMET[2]->Fill(dPhi_PhoMET,wt);
	h_St[2]->Fill(ST,wt);

    	// if(isoElectronTracks==0 && isoMuonTracks ==0 && isoPionTracks==0)//veto isolated leptons                                                                                      
	//   {
	if(isoElectronTracks==0 && isoMuonTracks ==0 && isoPionTracks==0)
	  {
	    h_selectBaselineYields_->Fill("Iso track",wt);
	  }
	else continue;

	//	    h_selectBaselineYields_->Fill("veto Isotrk",wt);

	    h_Njets[3]->Fill(nHadJets,wt);
	    h_Nbjets[3]->Fill(BTags,wt);
	    h_MET_[3]->Fill(MET,wt);
	    h_PhotonPt[3]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[3]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[3]->Fill(dPhi_PhoMET,wt);
	    h_St[3]->Fill(ST,wt);

	    //if(bestPhoton.Pt()<20) continue;// cout<< "notPt cut"<<endl;
	    
	    //	    h_selectBaselineYields_->Fill("Pt >20GeV",wt);
	    h_Njets[4]->Fill(nHadJets,wt);
            h_Nbjets[4]->Fill(BTags,wt);
            h_MET_[4]->Fill(MET,wt);
            h_PhotonPt[4]->Fill(bestPhoton.Pt(),wt);
            h_Mt_PhoMET[4]->Fill(mTPhoMET,wt);
            h_dPhi_PhoMET[4]->Fill(dPhi_PhoMET,wt);
            h_St[4]->Fill(ST,wt);

	    // if (nHadJets>=2) 
	    //   {		
	    //h_selectBaselineYields_->Fill("nhadjets>=2",wt);
	    // if(bestPhoton.Pt()<=100) continue;
	    // h_selectBaselineYields_->Fill("pt>100",wt);

	    // if(MET<=100) continue;
            // h_selectBaselineYields_->Fill("MET>100",wt);

	    // if (nHadJets<2) continue;
	    // h_selectBaselineYields_->Fill("njets>=2",wt);
	    if(hadJets.size() == 0) continue;

	    if( MET>100)
	      {
		h_selectBaselineYields_->Fill("MET>100",wt);
	      }
	    else continue;
	    if( nHadJets >= 2 )
	      {
		h_selectBaselineYields_->Fill("nJets >= 2",wt);
	      }

	    else
           
		continue;

	    // if(MET<=100) continue;
	    // h_selectBaselineYields_->Fill("MET>100",wt);

	    h_Njets[5]->Fill(nHadJets,wt);
	    h_Nbjets[5]->Fill(BTags,wt);
	    h_MET_[5]->Fill(MET,wt);
	    h_PhotonPt[5]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[5]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[5]->Fill(dPhi_PhoMET,wt);
	    h_St[5]->Fill(ST,wt);
		
		// for(int i=0;i<hadJets.size();i++)
		//   {
		//     if(i<6)
		//       {
		// 	h_dPhi_Met_hadJets[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),wt);
		// 	h_dPhivsMET[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),MET,wt);
		//       }
		//     else continue;
		//   }

		//Dphi between MET and Nhadjets ---for met cleaning  
		// double mT= 0.0, dPhi_METjet1=5, dPhi_METjet2=5;
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

	    if(phoMatchingJetIndx>=0 && ((*Jets)[phoMatchingJetIndx].Pt())/(bestPhoton.Pt()) < 1.0) continue;
	    if(phoMatchingJetIndx<0) continue;
	    h_selectBaselineYields_->Fill("additional Bhumika",wt);

	    //double mTPhoMET=sqrt(2*(bestPhoton.Pt())*MET*(1-cos(DeltaPhi(METPhi,bestPhoton.Phi()))));

		// dPhi_METjet1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
		// dPhi_METjet2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
		// h_dPhi1vsdPhi2->Fill(dPhi_METjet1,dPhi_METjet2,wt);
	    //		if(dPhi_METjet1 < 0.3 || dPhi_METjet2 < 0.3 ) continue; /// dPhi cut
		//		h_selectBaselineYields_->Fill("dphiCut>0.3",wt);
		// for(int i=0;i<hadJets.size();i++)
                //   {
                //     if(i<6)
                //       {
                //         h_dPhi_Met_hadJets_after[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),wt);
                //         h_dPhivsMET_after[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),MET,wt);
                //       }
                //     else continue;
                //   }

		// h_selectBaselineYields_->Fill("dPhi(Met,leading jets)",wt);
		int Sbin_met100=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		h_Sbins_v6_withOnlyBL_Selec_Met100->Fill(Sbin_met100,wt);
		if( (ST <= 300) ) continue;
		h_selectBaselineYields_->Fill("St>300",wt);
		if( !((MET>250 && bestPhoton.Pt()>100) ) )continue;
		else
		  h_selectBaselineYields_->Fill("MET>250 & photon pt>100 selec",wt);
		bool process= false;
		if(!bestPhoHasPxlSeed && bestPhoton.Pt()>=100 && ST>300 && nHadJets>=2 && MET > 200 && dPhi_METjet1 > 0.3 && dPhi_METjet2 > 0.3 && NElectrons==0 && NMuons==0 &&((isoElectronTracks==0)&&(isoMuonTracks==0)&&(isoPionTracks==0)))
		  process =true;
		else continue;
		h_selectBaselineYields_->Fill("final",wt);
		    // if(ST<=300) continue;
		    // h_selectBaselineYields_->Fill("ST>300",wt);
		    
		    // if(MET<=250) continue;
		    int Sbin=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		    h_Sbins_v6_withOnlyBL_Selec->Fill(Sbin,wt);
		    
		    // h_selectBaselineYields_->Fill("Met>250",wt);
		    h_Njets[8]->Fill(nHadJets,wt);
		    h_Nbjets[8]->Fill(BTags,wt);
		    h_MET_[8]->Fill(MET,wt);
		    h_PhotonPt[8]->Fill(bestPhoton.Pt(),wt);
		    h_Mt_PhoMET[8]->Fill(mTPhoMET,wt);
		    h_dPhi_PhoMET[8]->Fill(dPhi_PhoMET,wt);
		    h_St[8]->Fill(ST,wt);
		  //   if(ST>300)
		  //     {
		  // 	h_selectBaselineYields_->Fill("ST>300 & MET>250",wt);
		  // 	h_Njets[12]->Fill(nHadJets,wt);
                  //       h_Nbjets[12]->Fill(BTags,wt);
		  // 	h_MET_[12]->Fill(MET,wt);
                  //       h_PhotonPt[12]->Fill(bestPhoton.Pt(),wt);
                  //       h_Mt_PhoMET[12]->Fill(mTPhoMET,wt);
                  //       h_dPhi_PhoMET[12]->Fill(dPhi_PhoMET,wt);
                  //       h_St[12]->Fill(ST,wt);
		  // 	if(bestPhoton.Pt()>100)
                  //         {
		  // 	    h_selectBaselineYields_->Fill("ST>300 & MET>250& pt>100",wt);
		  // 	    if(MET<=300) continue;
			      
		  // 	    int Sbin1=getBinNoV6_WithOnlyBLSelec(BTags,nHadJets);
		  // 	    h_Sbins_v6_withOnlyBL_Selec_PrevAna->Fill(Sbin1,wt);
			    
		  // 	    h_selectBaselineYields_->Fill("ST>300 & MET>300& pt>100",wt);
                  //           h_Njets[11]->Fill(nHadJets,wt);
                  //           h_Nbjets[11]->Fill(BTags,wt);
                  //           h_MET_[11]->Fill(MET,wt);
                  //           h_PhotonPt[11]->Fill(bestPhoton.Pt(),wt);
                  //           h_Mt_PhoMET[11]->Fill(mTPhoMET,wt);
                  //           h_dPhi_PhoMET[11]->Fill(dPhi_PhoMET,wt);
                  //           h_St[11]->Fill(ST,wt);
                  //         }

		  //     }
		  // }

		    // }	// h_NJets_Met250GeV->Fill(nHadJets,wt);
	      	// h_NbJets_Met250GeV->Fill(BTags,wt);
			// h_dPhi_PhoMET_Met250GeV->Fill(dPhi_PhoMET,wt);
			// h_Mt_PhoMET_Met250GeV->Fill(mTPhoMET,wt);
			// if(bestPhoton.Pt()<1980)
			//   h_check_PhoPt_Met250GeV->Fill(bestPhoton.Pt(),wt);
			// else
			//   h_check_PhoPt_Met250GeV->SetBinContent(h_check_PhoPt_Met250GeV->GetNbinsX(),wt);
			// h_HT_Met250GeV->Fill(ST,wt);
			// if(MET<1470)
			//   h_MeT_Met250GeV->Fill(MET,wt);
			// else
			//   h_MeT_Met250GeV->SetBinContent(h_MeT->GetNbinsX(),wt);

		// if(MET>600) //rejecting MET>600
		//   {
		//     h_selectBaselineYields_->Fill("Met>600",wt);
		//     h_Njets[9]->Fill(nHadJets,wt);
		//     h_Nbjets[9]->Fill(BTags,wt);
		//     h_MET_[9]->Fill(MET,wt);
		//     h_PhotonPt[9]->Fill(bestPhoton.Pt(),wt);
		//     h_Mt_PhoMET[9]->Fill(mTPhoMET,wt);
		//     h_dPhi_PhoMET[9]->Fill(dPhi_PhoMET,wt);
		//     h_St[9]->Fill(ST,wt);

		//     // h_NJets_Met600GeV->Fill(nHadJets,wt);
		//     // h_NbJets_Met600GeV->Fill(BTags,wt);
		//     // h_dPhi_PhoMET_Met600GeV->Fill(dPhi_PhoMET,wt);
		//     // h_Mt_PhoMET_Met600GeV->Fill(mTPhoMET,wt);

		//     // if(bestPhoton.Pt()<1980)
		//     //   h_check_PhoPt_Met600GeV->Fill(bestPhoton.Pt(),wt);
		//     // else
		//     //   h_check_PhoPt_Met600GeV->SetBinContent(h_check_PhoPt_Met600GeV->GetNbinsX(),wt);
		//     // h_HT_Met600GeV->Fill(ST,wt);
		//     // if(MET<1480)
		//     //   h_MeT_Met600GeV->Fill(MET,wt);
		//     // else
		//     //   h_MeT_Met600GeV->SetBinContent(h_MeT->GetNbinsX(),wt);
			    
		//     nSurvived++;
		//     h_selectBaselineYields_->Fill("survived",wt);

		//   }
	  
		    //	  }
       
		    
		    // if(bestPhoton.Pt()<1000)
		    //   h_PhoPt->Fill(bestPhoton.Pt(),wt);
		    // else
		    //   h_PhoPt->SetBinContent(h_PhoPt->GetNbinsX(),wt);
		    // h_check_PhoPt->Fill(bestPhoton.Pt(),wt);
		    // h_HT->Fill(ST,wt);
		    // h_madminPhotDeltaR->Fill(madMinPhotonDeltaR,wt);
		    // h_minDR_PhoLep->Fill( MinDr(bestPhoton,v_genLep2),wt);

	
		  
      
    
    
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
  vector<TLorentzVector> goodPho;
  for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back((*Photons)[iPhoton]);
  }
  if(goodPho.size()!=0) 
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

// double AnalyzeLightBSM::getGendRLepBestPho(TLorentzVector goodPho){//MC only                                                                                                                               
//   TLorentzVector genPho1,genLep1;
//   vector<TLorentzVector> v_genPho1,v_genLep1;
//   int leadGenPhoIdx=-100;

//   for(int i=0;i<GenParticles->size();i++){
//     if((*GenParticles)[i].Pt()!=0){

//       if((abs((*GenParticles_PdgId)[i])==22) && ((abs((*GenParticles_ParentId)[i])<=25) || ((*GenParticles_ParentId)[i]==2212) ) && (*GenParticles_Status)[i]==1 ){
//         leadGenPhoIdx = i;
//         genPho1 = ((*GenParticles)[i]);
//         v_genPho1.push_back(genPho1);
//       }
//     }
//   }

//   bool hadtau = false;

//   for(int i=0 ; i < GenElectrons->size(); i++)
//     {
//       if((*GenElectrons)[i].Pt()!=0)
//         {
//           genLep1=(*GenElectrons)[i];
//           v_genLep1.push_back(genLep1);
//         }
//     }
//   for(int i=0 ; i < GenMuons->size(); i++)
//     {
//       if((*GenMuons)[i].Pt()!=0)
//         {
//           genLep1 = ((*GenMuons)[i]);
//           v_genLep1.push_back(genLep1);
//         }
//     }
//   for(int i=0 ; i < GenTaus->size(); i++)
//     {
//       if((*GenTaus)[i].Pt()!=0)
//         {
//           genLep1 = ((*GenTaus)[i]);
//           v_genLep1.push_back(genLep1);
//         }
//     }

//   double mindr=  MinDr(goodPho,v_genLep1);
//   if(mindr >= 0) return mindr;
//   // else 1000;

// }

double AnalyzeLightBSM::getGenLep(TLorentzVector bestPhoton){//MC only                                                                                                                                     
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

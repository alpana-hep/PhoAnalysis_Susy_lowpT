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
  double lumiInfb=137;
  if(s_data.Contains("2016")) lumiInfb=35.9;
  if(s_data.Contains("2017")) lumiInfb=41.59;
  if(s_data.Contains("2018")) lumiInfb=59.74;
  if(s_data.Contains("signal"))lumiInfb= 137;
  int count_QCD=0;
  Long64_t nSurvived = 0,bkg_comp=0,MET_rej=0,nocut=0;
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
    TLorentzVector bestPhoton=getBestPhoton();
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
    TLorentzVector genEle1, genMu1;
    vector<TLorentzVector> v_genLep2;
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
    h_madHT->Fill(madHT,wt);
    h_minPho_lep->Fill(MinDr(bestPhoton,v_genLep2),wt);
    h_madminPhotonDeltaR_->Fill(madMinPhotonDeltaR,wt);
    //cout<<v_genLep2.size()<<endl;
    ///        BACKGROUND COMPOSITION: Removing overlapping events (W jets & WGJets, TTG & TTJets, ZG, ZJets)      /////
    if(s_sample.Contains("QCD_Jets"))
      {
	if(madMinPhotonDeltaR>0.4 &&  hasGenPromptPhoton) continue;
	if(jentry<3) cout<<"QCD jets"<<endl;
      }
    else  if (s_sample.Contains("GJets_DR"))
      {
	if(madMinPhotonDeltaR<0.4 &&  hasGenPromptPhoton) continue;
	if(jentry<3) cout<<"G jets"<<endl;
      }
    else if(s_sample.Contains("ZJets"))
      {
	if(hasGenPromptPhoton && madMinPhotonDeltaR>0.3) continue;
        if(jentry<3) cout<<"Non-Prompt, dR(pho,q/g/lep) < 0.3 ";
      }
    else if(s_sample.Contains("ZGJets"))
      { if(hasGenPromptPhoton && madMinPhotonDeltaR < 0.3)continue;
      }
    else if( s_sample.Contains("TTJets-HT") ||  s_sample.Contains("TTJets-inc"))
      {
	if(hasGenPromptPhoton){
	  if(!(madMinPhotonDeltaR < 0.3 || MinDr(bestPhoton,v_genLep2) < 0.5)) continue;
	}   
      }
    else if( s_sample.Contains("TTGJets"))// ||  s_sample.Contains("ZGJets"))
      {
	if(hasGenPromptPhoton){
	  if(!(madMinPhotonDeltaR >= 0.3 && MinDr(bestPhoton,v_genLep2) >=0.5 ))continue;  }
      }
    else if(s_sample.Contains("WGJets"))
      {
	if(hasGenPromptPhoton){
	  if(!(madMinPhotonDeltaR >= 0.5 && MinDr(bestPhoton,v_genLep2) >=0.5 ))continue;
	}
	if(jentry<3) cout<<"WGjets"<<endl;
      }
    else if(s_sample.Contains("WJets"))
      {
	if(hasGenPromptPhoton){
	  if(!(madMinPhotonDeltaR < 0.5 || MinDr(bestPhoton,v_genLep2) < 0.5)) continue;}
      }
    h_selectBaselineYields_->Fill("after bkg cmp",wt);
    bkg_comp++;
      
      h_madHT_after->Fill(madHT,wt);
      h_minPho_lep_after->Fill(MinDr(bestPhoton,v_genLep2),wt);
      h_madminPhotonDeltaR_after->Fill(madMinPhotonDeltaR,wt);

    h_Njets[7]->Fill(nHadJets,wt);
    h_Nbjets[7]->Fill(BTags,wt);
    h_MET_[7]->Fill(MET,wt);
    h_PhotonPt[7]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[7]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[7]->Fill(dPhi_PhoMET,wt);
    h_St[7]->Fill(ST,wt);

    ///

       
    ////    BASELINE SELECTION STARTS HERE   ///////////
    if(bestPhoton.Pt()<20) continue;
    if (nHadJets<2) continue;
    if(MET<=100) continue;
    h_Njets[1]->Fill(nHadJets,wt);
    h_Nbjets[1]->Fill(BTags,wt);
    h_MET_[1]->Fill(MET,wt);
    h_PhotonPt[1]->Fill(bestPhoton.Pt(),wt);
    h_Mt_PhoMET[1]->Fill(mTPhoMET,wt);
    h_dPhi_PhoMET[1]->Fill(dPhi_PhoMET,wt);
    h_St[1]->Fill(ST,wt);

    if(NMuons ==0 && NElectrons==0)
      {
	h_selectBaselineYields_->Fill("veto lep",wt);

	h_Njets[2]->Fill(nHadJets,wt);
	h_Nbjets[2]->Fill(BTags,wt);
	h_MET_[2]->Fill(MET,wt);
	h_PhotonPt[2]->Fill(bestPhoton.Pt(),wt);
	h_Mt_PhoMET[2]->Fill(mTPhoMET,wt);
	h_dPhi_PhoMET[2]->Fill(dPhi_PhoMET,wt);
	h_St[2]->Fill(ST,wt);

    	if(isoElectronTracks==0 && isoMuonTracks ==0)// continue;// && isoPionTracks==0)//veto isolated leptons                                                                                      
	  {
	    h_selectBaselineYields_->Fill("veto Isolep",wt);

	    h_Njets[3]->Fill(nHadJets,wt);
	    h_Nbjets[3]->Fill(BTags,wt);
	    h_MET_[3]->Fill(MET,wt);
	    h_PhotonPt[3]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[3]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[3]->Fill(dPhi_PhoMET,wt);
	    h_St[3]->Fill(ST,wt);

	    //if(bestPhoton.Pt()<20) continue;// cout<< "notPt cut"<<endl;
	    
	    h_selectBaselineYields_->Fill("Pt >20GeV",wt);
	    h_Njets[4]->Fill(nHadJets,wt);
            h_Nbjets[4]->Fill(BTags,wt);
            h_MET_[4]->Fill(MET,wt);
            h_PhotonPt[4]->Fill(bestPhoton.Pt(),wt);
            h_Mt_PhoMET[4]->Fill(mTPhoMET,wt);
            h_dPhi_PhoMET[4]->Fill(dPhi_PhoMET,wt);
            h_St[4]->Fill(ST,wt);

	    // if (nHadJets>=2) 
	    //   {		
	    h_selectBaselineYields_->Fill("nhadjets>=2",wt);
	    
	    h_Njets[5]->Fill(nHadJets,wt);
	    h_Nbjets[5]->Fill(BTags,wt);
	    h_MET_[5]->Fill(MET,wt);
	    h_PhotonPt[5]->Fill(bestPhoton.Pt(),wt);
	    h_Mt_PhoMET[5]->Fill(mTPhoMET,wt);
	    h_dPhi_PhoMET[5]->Fill(dPhi_PhoMET,wt);
	    h_St[5]->Fill(ST,wt);
		
		for(int i=0;i<hadJets.size();i++)
		  {
		    if(i<6)
		      {
			h_dPhi_Met_hadJets[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),wt);
			h_dPhivsMET[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),MET,wt);
		      }
		    else continue;
		  }

		//Dphi between MET and Nhadjets ---for met cleaning  
		double mT= 0.0, dPhi_METjet1=5, dPhi_METjet2=5;
		dPhi_METjet1 = abs(DeltaPhi(METPhi,(hadJets)[0].Phi()));
		dPhi_METjet2 = abs(DeltaPhi(METPhi,(hadJets)[1].Phi()));
		h_dPhi1vsdPhi2->Fill(dPhi_METjet1,dPhi_METjet2,wt);
		if(dPhi_METjet1 < 0.3 || dPhi_METjet2 < 0.3 ) continue; /// dPhi cut
		for(int i=0;i<hadJets.size();i++)
                  {
                    if(i<6)
                      {
                        h_dPhi_Met_hadJets_after[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),wt);
                        h_dPhivsMET_after[i]->Fill(abs(DeltaPhi(METPhi,hadJets[i].Phi())),MET,wt);
                      }
                    else continue;
                  }

		h_Njets[6]->Fill(nHadJets,wt);
		h_Nbjets[6]->Fill(BTags,wt);
		h_MET_[6]->Fill(MET,wt);
		h_PhotonPt[6]->Fill(bestPhoton.Pt(),wt);
		h_Mt_PhoMET[6]->Fill(mTPhoMET,wt);
		h_dPhi_PhoMET[6]->Fill(dPhi_PhoMET,wt);
		h_St[6]->Fill(ST,wt);

		h_selectBaselineYields_->Fill("Met Cleaning",wt);
		// if(MET>100) 
		//   {
		    // h_selectBaselineYields_->Fill("Met>100",wt);
		    // h_Njets[7]->Fill(nHadJets,wt);
		    // h_Nbjets[7]->Fill(BTags,wt);
		    // h_MET_[7]->Fill(MET,wt);
		    // h_PhotonPt[7]->Fill(bestPhoton.Pt(),wt);
		    // h_Mt_PhoMET[7]->Fill(mTPhoMET,wt);
		    // h_dPhi_PhoMET[7]->Fill(dPhi_PhoMET,wt);
		    // h_St[7]->Fill(ST,wt);
		    
		    // if(ST>300)
		    //   {
		    // 	h_selectBaselineYields_->Fill("ST>300 & MET>100",wt);

		    // 	h_Njets[10]->Fill(nHadJets,wt);
		    // 	h_Nbjets[10]->Fill(BTags,wt);
		    // 	h_MET_[10]->Fill(MET,wt);
		    // 	h_PhotonPt[10]->Fill(bestPhoton.Pt(),wt);
		    // 	h_Mt_PhoMET[10]->Fill(mTPhoMET,wt);
		    // 	h_dPhi_PhoMET[10]->Fill(dPhi_PhoMET,wt);
		    // 	h_St[10]->Fill(ST,wt);
		    //   }
			// if(bestPhoton.Pt()>100)
			//   {
			//     h_Njets[11]->Fill(nHadJets,wt);
			//     h_Nbjets[11]->Fill(BTags,wt);
			//     h_MET_[11]->Fill(MET,wt);
			//     h_PhotonPt[11]->Fill(bestPhoton.Pt(),wt);
			//     h_Mt_PhoMET[11]->Fill(mTPhoMET,wt);
			//     h_dPhi_PhoMET[11]->Fill(dPhi_PhoMET,wt);
			//     h_St[11]->Fill(ST,wt);
			//   }
		    //}
		  
		    // h_NJets->Fill(nHadJets,wt);
		    // h_NbJets->Fill(BTags,wt);
		    // h_dPhi_PhoMET->Fill(dPhi_PhoMET,wt);
		    // if(MET<1470)
		    //   h_MeT->Fill(MET,wt);
		    // else
		    //   h_MeT->SetBinContent(h_MeT->GetNbinsX(),wt);
		    // if(bestPhoton.Pt()<1980)
		    //   h_check_PhoPt->Fill(bestPhoton.Pt(),wt);
		    // else
		    //   h_check_PhoPt->SetBinContent(h_check_PhoPt->GetNbinsX(),wt);
		    // //if (mTPhoMET<1980)
		    // h_Mt_PhoMET->Fill(mTPhoMET,wt);		    
                    h_HT->Fill(ST,wt);
                    h_madminPhotDeltaR->Fill(madMinPhotonDeltaR,wt);
                    h_minDR_PhoLep->Fill( MinDr(bestPhoton,v_genLep2),wt);
		  
		if(MET>250) //rejetcing MET>250
		  {
		    h_selectBaselineYields_->Fill("Met>250",wt);
		    h_Njets[8]->Fill(nHadJets,wt);
		    h_Nbjets[8]->Fill(BTags,wt);
		    h_MET_[8]->Fill(MET,wt);
		    h_PhotonPt[8]->Fill(bestPhoton.Pt(),wt);
		    h_Mt_PhoMET[8]->Fill(mTPhoMET,wt);
		    h_dPhi_PhoMET[8]->Fill(dPhi_PhoMET,wt);
		    h_St[8]->Fill(ST,wt);
		    if(ST>300)
		      {
			h_selectBaselineYields_->Fill("ST>300 & MET>250",wt);
			h_Njets[12]->Fill(nHadJets,wt);
                        h_Nbjets[12]->Fill(BTags,wt);
			h_MET_[12]->Fill(MET,wt);
                        h_PhotonPt[12]->Fill(bestPhoton.Pt(),wt);
                        h_Mt_PhoMET[12]->Fill(mTPhoMET,wt);
                        h_dPhi_PhoMET[12]->Fill(dPhi_PhoMET,wt);
                        h_St[12]->Fill(ST,wt);
			if(bestPhoton.Pt()>100)
                          {
			    h_selectBaselineYields_->Fill("ST>300 & MET>250& pt>100",wt);
                            h_Njets[11]->Fill(nHadJets,wt);
                            h_Nbjets[11]->Fill(BTags,wt);
                            h_MET_[11]->Fill(MET,wt);
                            h_PhotonPt[11]->Fill(bestPhoton.Pt(),wt);
                            h_Mt_PhoMET[11]->Fill(mTPhoMET,wt);
                            h_dPhi_PhoMET[11]->Fill(dPhi_PhoMET,wt);
                            h_St[11]->Fill(ST,wt);
                          }

		      }
		  }

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

		if(MET>600) //rejecting MET>600
		  {
		    h_selectBaselineYields_->Fill("Met>600",wt);
		    h_Njets[9]->Fill(nHadJets,wt);
		    h_Nbjets[9]->Fill(BTags,wt);
		    h_MET_[9]->Fill(MET,wt);
		    h_PhotonPt[9]->Fill(bestPhoton.Pt(),wt);
		    h_Mt_PhoMET[9]->Fill(mTPhoMET,wt);
		    h_dPhi_PhoMET[9]->Fill(dPhi_PhoMET,wt);
		    h_St[9]->Fill(ST,wt);

		    // h_NJets_Met600GeV->Fill(nHadJets,wt);
		    // h_NbJets_Met600GeV->Fill(BTags,wt);
		    // h_dPhi_PhoMET_Met600GeV->Fill(dPhi_PhoMET,wt);
		    // h_Mt_PhoMET_Met600GeV->Fill(mTPhoMET,wt);

		    // if(bestPhoton.Pt()<1980)
		    //   h_check_PhoPt_Met600GeV->Fill(bestPhoton.Pt(),wt);
		    // else
		    //   h_check_PhoPt_Met600GeV->SetBinContent(h_check_PhoPt_Met600GeV->GetNbinsX(),wt);
		    // h_HT_Met600GeV->Fill(ST,wt);
		    // if(MET<1480)
		    //   h_MeT_Met600GeV->Fill(MET,wt);
		    // else
		    //   h_MeT_Met600GeV->SetBinContent(h_MeT->GetNbinsX(),wt);
			    
		    nSurvived++;
		    h_selectBaselineYields_->Fill("survived",wt);

		  }
	  
	  }
      	    
		    
		    // if(bestPhoton.Pt()<1000)
		    //   h_PhoPt->Fill(bestPhoton.Pt(),wt);
		    // else
		    //   h_PhoPt->SetBinContent(h_PhoPt->GetNbinsX(),wt);
		    // h_check_PhoPt->Fill(bestPhoton.Pt(),wt);
		    // h_HT->Fill(ST,wt);
		    // h_madminPhotDeltaR->Fill(madMinPhotonDeltaR,wt);
		    // h_minDR_PhoLep->Fill( MinDr(bestPhoton,v_genLep2),wt);

	
		  
      }
    
    
    }
    // nSurvived++;
    // h_selectBaselineYields_->Fill("survived",wt);


  cout<<nocut<<"\t"<<nSurvived<<"\t"<<bkg_comp<<endl;
  //    }
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


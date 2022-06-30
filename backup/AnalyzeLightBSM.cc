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

  if (argc < 3) {
    cerr << "Please give 4 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" <<" "<<"N2-Mass"<< endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  AnalyzeLightBSM ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  ana.EventLoop(data,inputFileList);

  return 0;
}

void AnalyzeLightBSM::EventLoop(const char *data,const char *inputFileList) {
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
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;
  int count=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
      //wt = (0.165*137)/nentries;
      wt = Weight*1000.0*59.64;
      //           cout<<Weight<<endl;
      // ==============print number of events done == == == == == == == =
      double progress = 10.0 * jentry / (1.0 * nentries);
      int k = int (progress);
      if (k > decade)
	cout << 10 * k << " %" << endl;
      decade = k;
    

    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //my code starts from here .... 19Aug2020
    int branch_size = (*GenParticles).size();
    int ele_branch=(*Electrons).size();
    int pho_branch = (*Photons).size();
    float iGenEle_eta =99999.0, iGenEle_phi=99999.0, iRecEle_eta =99999.0, iRecEle_phi=99999.0, iRecphot_eta =99999.0, iRecphot_phi=99999.0,iGen_Wpt=99999.0,iGen_Wp=99999.0,iGen_WEta=99999.0,iGen_WPhi=99999.0;
    float dR_Ele=0, min_dR=9999, min_dR_pho=9999;
    int count_genEle=0,count_recEle=0;
    //cout<<branch_size<<"\t"<<"ele_size"<<"\t"<<ele_branch<<endl;
    for (int i_gen=0; i_gen<branch_size;i_gen++)
      {
	if(abs((*GenParticles_PdgId)[i_gen])==24)
	  {
	    iGen_Wpt=(*GenParticles)[i_gen].Pt();
	    iGen_Wp=(*GenParticles)[i_gen].P();
	    iGen_WEta= (*GenParticles)[i_gen].Eta();
	    iGen_WPhi= (*GenParticles)[i_gen].Phi();
	    //cout<<i_gen<<endl;
	    // h_GenW_Pt->Fill((*GenParticles)[i_gen].Pt(),wt);
	    // h_GenW_P->Fill((*GenParticles)[i_gen].P(),wt);
	    // h_GenW_Eta->Fill((*GenParticles)[i_gen].Eta(),wt);
	    // h_GenW_Phi->Fill((*GenParticles)[i_gen].Phi(),wt);
	    // h_GenW_PtvsP->Fill((*GenParticles)[i_gen].Pt(),(*GenParticles)[i_gen].P(),wt);
	    // h_GenW_PtvsEta->Fill((*GenParticles)[i_gen].Eta(),(*GenParticles)[i_gen].Pt(),wt);
	    // h_GenW_PvsEta->Fill((*GenParticles)[i_gen].Eta(),(*GenParticles)[i_gen].P(),wt);
	    // h_GenW_PtvsPhi->Fill((*GenParticles)[i_gen].Phi(),(*GenParticles)[i_gen].Pt(),wt);
	  }
	    
	if(abs((*GenParticles_PdgId)[i_gen])==11 && abs((*GenParticles_ParentId)[i_gen])==24 && (*GenParticles_Status)[i_gen]==1) //selecting only e coming from W
	  {
	    count_genEle++;
	    //cout<<i_gen<<"e-"<<endl;
	    //to calculat// e dR
	    if(iGen_Wpt>10)
	      {
	    h_GenW_Pt->Fill(iGen_Wpt,wt);
	    h_GenW_P->Fill(iGen_Wp,wt);
	    h_GenW_Eta->Fill(iGen_WEta,wt);
	    h_GenW_Phi->Fill(iGen_WPhi,wt);
	    h_GenW_PtvsP->Fill(iGen_Wpt,iGen_Wp,wt);
	    h_GenW_PtvsEta->Fill(iGen_Wpt,iGen_WEta,wt);
	    h_GenW_PvsEta->Fill(iGen_Wp,iGen_WEta,wt);
	    h_GenW_PtvsPhi->Fill(iGen_WPhi,iGen_Wpt,wt);
	    }
	    iGenEle_eta = (*GenParticles)[i_gen].Eta();
	    iGenEle_phi = (*GenParticles)[i_gen].Phi();
	    h_Pt_WVselec->Fill(iGen_Wpt,(*GenParticles)[i_gen].Pt(),wt);
	    h_Pt_WVsMet->Fill(iGen_Wpt,MET,wt);
	    h_ElecPtVs_MET->Fill((*GenParticles)[i_gen].Pt(), GenMET,wt);
	    h_Pt_WVs_GenMet->Fill(iGen_Wpt,GenMET,wt);
	    //filling the histograms
	    h_GenEle_Pt->Fill((*GenParticles)[i_gen].Pt(),wt);
	    h_GenEle_Pt_reBin->Fill((*GenParticles)[i_gen].Pt(),wt);
	    h_GenEle_Phi->Fill(iGenEle_phi,wt);
	    h_GenEle_Eta->Fill(iGenEle_eta,wt);
	    h_PtVsEta_GenElec->Fill(iGenEle_eta,(*GenParticles)[i_gen].Pt(),wt);
	    h_PtVsEta_GenElec_zoomed->Fill(iGenEle_eta,(*GenParticles)[i_gen].Pt(),wt);
	    h_PtVsPhi_GenElec->Fill(iGenEle_phi,(*GenParticles)[i_gen].Pt(),wt);
	    h_EtavsPhi_GenElec->Fill(iGenEle_eta, iGenEle_phi,wt);
	    h_GenEle_Pt_lastbin->Fill((*GenParticles)[i_gen].Pt(),wt);
	    if((*GenParticles)[i_gen].Pt()<=300) h_GenEle_Pt_zoomed->Fill((*GenParticles)[i_gen].Pt(),wt);
	    
	    //cout<<iGenEle_eta<<"\t"<<iGenEle_phi<<"\t"<<".."<<endl;
	    for(int i_rec=0; i_rec<ele_branch;i_rec++)
	      {
		
		iRecEle_eta = (*Electrons)[i_rec].Eta();
		iRecEle_phi= (*Electrons)[i_rec].Phi();
		
		//calculating dR between Gen electron and reco Electrons
		float dEta= iGenEle_eta - iRecEle_eta;
		float dPhi= iGenEle_phi-iRecEle_phi;		
		h_dPhiVsdEta_Electron->Fill(dEta,dPhi,wt);
		dR_Ele= abs(sqrt((dEta * dEta) + (dPhi * dPhi) ));
		h_dR_Electron->Fill(dR_Ele,wt);
		h_dEta_Electron->Fill(dEta,wt);
		h_dPhi_Electron->Fill(dPhi,wt);
		float updated_dR=DeltaR(iGenEle_eta,iGenEle_phi,iRecEle_eta, iRecEle_phi);
		h_dEtaVsdR_zoomed->Fill(dEta,updated_dR,wt);
		h_dPhiVsdR_zoomed->Fill(dPhi,updated_dR,wt);
		h_dPhiVsdR->Fill(dPhi,updated_dR,wt);
		h_dEtaVsdR->Fill(dEta,updated_dR,wt);
		h_dR_DiffMethod->Fill(updated_dR,wt);				    
		//cout<<updated_dR<<"\t"<<".."<<endl;
		if(updated_dR<min_dR)
		  min_dR=updated_dR;
		h_dR_Ele_zoomed->Fill( min_dR,wt);
		//int Sbin= getBin(min_dR);
		//h_dR_Ele_lastBin->Fill(Sbin,wt);		
	      }
	    if(min_dR<0.1) //Gen e- matches with reco
	      {
		h_GenRecEle_Pt_reBin->Fill((*GenParticles)[i_gen].Pt(),wt);
		h_GenRecEle_Pt_rebin->Fill((*GenParticles)[i_gen].Pt(),wt);
		h_GenRecEle_Pt->Fill((*GenParticles)[i_gen].Pt(),wt);
		h_GenRecEle_Eta->Fill(iGenEle_eta,wt);
	      }
	    else
	      {
		for(int i_phot=0; i_phot<pho_branch;i_phot++)
		  {
		    iRecphot_eta = (*Photons)[i_phot].Eta();
		    iRecphot_phi= (*Photons)[i_phot].Phi();
		    float updated_dR=DeltaR(iGenEle_eta,iGenEle_phi,iRecphot_eta, iRecphot_phi);
		    h_dR_GenEleVsPhoton->Fill(updated_dR,wt);
		    if(updated_dR<min_dR_pho)
		      min_dR_pho = updated_dR;
		    
		  }
		if(min_dR_pho<0.1)
		  {
		    h_GenElec_RecPho_Pt_rebin->Fill((*GenParticles)[i_gen].Pt(),wt);
		    h_GenElec_RecPho_Pt_reBin->Fill((*GenParticles)[i_gen].Pt(),wt);
		    h_GenElec_RecPho_Pt->Fill((*GenParticles)[i_gen].Pt(),wt);
		    h_GenElec_RecPho_Eta->Fill(iGenEle_eta,wt);
		  }
	      }
	   
	  }
	
      }
    h_mindR_GenEleVsPhoton->Fill(min_dR_pho,wt);
    //filling reconstructed e-histograms
    for(int i_rec=0; i_rec<ele_branch;i_rec++)
      {
	//if(ele_branch==2) cout<<"electron"<<endl;
	iRecEle_eta = (*Electrons)[i_rec].Eta();
	iRecEle_phi= (*Electrons)[i_rec].Phi();

	//filling the histrograms                                                                                                                                                                  
	h_RecEle_Pt->Fill((*Electrons)[i_rec].Pt(),wt);
	h_RecEle_Phi->Fill(iRecEle_phi,wt);
	h_RecEle_Eta->Fill(iRecEle_eta,wt);
	//cout<<(*Electrons)[i_rec].Pt()<<"\t"<<iRecEle_eta<<"\t"<<iRecEle_phi<<endl;                                                                                                              
	h_PtVsEta_RecElec->Fill(iRecEle_eta,(*Electrons)[i_rec].Pt(),wt);
	h_PtVsEta_RecElec_zoomed->Fill(iRecEle_eta,(*Electrons)[i_rec].Pt(),wt);
	h_temp->Fill(iRecEle_eta ,(*Electrons)[i_rec].Pt(),wt);
	h_PtVsPhi_RecElec->Fill(iRecEle_phi,(*Electrons)[i_rec].Pt(),wt);
	h_EtavsPhi_RecElec->Fill(iRecEle_eta,iRecEle_phi,wt);
      }

    h_NGenVsReco_Elec->Fill(ele_branch,count_genEle,wt);

    h_mindR_Electron->Fill(min_dR,wt);


    /////////////////////////////////calculating TRANSVERSE MASS ////////////////////////////

    float iPhi_e=-9999.0,iPhi_nu=-9999.0 ,iPt_e=-9999.0,iPt_nu=-9999.0,iEta_e=0,iEta_nu=0;
    int count=0;
      for (int i_gen=0; i_gen<branch_size;i_gen++)
	{
	  if(abs((*GenParticles_PdgId)[i_gen])==11 && abs((*GenParticles_ParentId)[i_gen])==24 && (*GenParticles_Status)[i_gen]==1) //selecting only e coming from W                            
	    {
	      
	      iPt_e=(*GenParticles)[i_gen].Pt();
	      iPhi_e=(*GenParticles)[i_gen].Phi();
	      iEta_e=(*GenParticles)[i_gen].Eta();
	    }
	  if(abs((*GenParticles_PdgId)[i_gen])==12 && abs((*GenParticles_ParentId)[i_gen])==24 && (*GenParticles_Status)[i_gen]==1)
	    {
	      iPt_nu=(*GenParticles)[i_gen].Pt();
              iPhi_nu=(*GenParticles)[i_gen].Phi();
              iEta_nu=(*GenParticles)[i_gen].Eta();
	      h_nu_pt->Fill(iPt_nu,wt);
	      h_Pt_WVsGen_nu->Fill(iGen_Wpt,iPt_nu,wt);
	    }
	  
	}
      float Trans_M= TransMass(iPhi_e,iPhi_nu, iPt_e,iPt_nu);
      if(Trans_M !=-1) 
	{
	  h_MtvsEta->Fill(iEta_e,Trans_M,wt);
	  h_Mt->Fill(Trans_M,wt);
	  h_Mt_zoomed->Fill(Trans_M,wt);
	  h_Pt_eVsnu->Fill(iPt_e,iPt_nu,wt);
	  h_Eta_eVsnu->Fill(iEta_e, iEta_nu,wt);
	  h_Phi_eVsnu->Fill(iPhi_e,iPhi_nu,wt);
	  h_Pt_W_vsMt->Fill(iGen_Wpt,Trans_M,wt);
	}
	  //cout<<Trans_M<<endl;
	  //}
    //    cout<<min_dR<<endl;
    }
    // h_NJets_inclusive->Fill(Jets->size());
    // h_NbJets_inclusive->Fill(BTags,1000*Weight);
    // //veto events with muon and lepton 
    // if(NMuons ==0 && NElectrons==0)
    //   {
    // 	if(isoElectronTracks==0 && isoMuonTracks ==0)// && isoPionTracks==0)//veto isolated leptons
    // 	  {
    // 	    h_NJets_exclusive->Fill(Jets->size());
    // 	    h_NbJets_exclusive->Fill(BTags);
    // 	    h_Met_exclusive->Fill(MET);
	   
    // 	    //cout<<BTags<<endl;
    // 	    // if(MET>100)
    // 	    //   {
    // 	    vector<TLorentzVector> goodPho;
    // 	    for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    // 	      if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back( (*Photons)[iPhoton] );
    // 	    }

    // 	    if(goodPho.size()==0) {count++;continue;}
    // 	    //else cout<<goodPho.size()<<endl;
    // 	    TLorentzVector bestPhoton=goodPho[0];//getBestPhoton();
    // 	    //if (bestPhoton.Pt()==0) {count++;continue;}
	      
    // 	    h_PhoPt_exclusive->Fill(bestPhoton.Pt());
    // 	    h_PhoPhi_exclusive->Fill(bestPhoton.Phi());
	    
    // 		//cout<<bestPhoton.Pt()<<endl;
    // 		// if(bestPhoton.Pt()>100)
    // 		//   {
    // 		//selection//  for best jets
    // 		int minDRindx=-100,phoMatchingJetIndx=-100,nHadJets=0;
    // 		double minDR=99999,ST=0;
    // 		vector<TLorentzVector> hadJets;
		
    // 	    for(int i=0;i<Jets->size();i++)
    // 	      {
    // 		if( ((*Jets)[i].Pt() > 30.0) && (abs((*Jets)[i].Eta()) <= 2.4) ){
    // 		  double dR=bestPhoton.DeltaR((*Jets)[i]);
    // 		  if(dR<minDR){minDR=dR;minDRindx=i;}
    // 		  if( !(minDR < 0.3 && i==minDRindx) )
    // 		      hadJets.push_back((*Jets)[i]);}
    // 	      }
    // 	    if( minDR<0.3 ) phoMatchingJetIndx=minDRindx;
    // 	    for(int i=0;i<hadJets.size();i++){
    // 	      if( (abs(hadJets[i].Eta()) < 2.4) ){ST=ST+(hadJets[i].Pt());}
    // 	      if( (abs(hadJets[i].Eta()) < 2.4) ){nHadJets++;}
    // 	    }
    // 	    if( minDR<0.3 ) ST=ST+bestPhoton.Pt();

    // 	    h_NJets_LepIsoVeto->Fill(nHadJets);
    //         h_NbJets_LepIsoVeto->Fill(BTags);
    //         h_Met_LepIsoVeto->Fill(MET);
    // 	    h_PhoPt_LepIsoVeto->Fill(bestPhoton.Pt());
    // 	    h_PhoPhi_LepIsoVeto->Fill(bestPhoton.Phi());
    // 	    h_HT_LepIsoVeto->Fill(ST);
	    
    // 	    //applying jet cut
    // 	    if(nHadJets>=2)
    // 	      {
    // 		h_NJets_aJetcut->Fill(nHadJets);
    // 		h_NbJets_aJetcut->Fill(BTags);
    // 		h_Met_aJetcut->Fill(MET);
    // 		h_PhoPt_aJetcut->Fill(bestPhoton.Pt());
    // 		h_PhoPhi_aJetcut->Fill(bestPhoton.Phi());
    // 		h_HT_aJetcut->Fill(ST);
    // 		if(MET>100)
    // 		  {
    // 		    h_NJets_aMetcut->Fill(nHadJets);
    // 		    h_NbJets_aMetcut->Fill(BTags);
    // 		    h_Met_aMetcut->Fill(MET);
    // 		    h_PhoPt_aMetcut->Fill(bestPhoton.Pt());
    // 		    h_PhoPhi_aMetcut->Fill(bestPhoton.Phi());
    // 		    h_HT_aMetcut->Fill(ST);
    // 		    if(bestPhoton.Pt()>10)
    // 		      {
    // 			h_NJets_PhoPt->Fill(nHadJets);
    // 			h_NbJets_PhoPt->Fill(BTags);
    // 			h_Met_PhoPt->Fill(MET);
    // 			h_PhoPt_PhoPt->Fill(bestPhoton.Pt());
    // 			h_PhoPhi_PhoPt->Fill(bestPhoton.Phi());
    // 			h_HT_PhoPt->Fill(ST);
    // 		      }
    // 		  }
		
    // 	      }
    // 	    if(MET>100)
    // 	      {
    // 		h_NJets_ExMET->Fill(nHadJets);
    //             h_NbJets_ExMET->Fill(BTags);
    //             h_Met_ExMET->Fill(MET);
    //             h_PhoPt_ExMET->Fill(bestPhoton.Pt());
    //             h_PhoPhi_ExMET->Fill(bestPhoton.Phi());
    // 		h_HT_ExMET->Fill(ST);
    // 	      }
	    //   {
	    // 	h_NJets_aJetcut->Fill(nHadJets);
            //     h_NbJets_aJetcut->Fill(BTags);
            //     h_Met_aJetcut->Fill(MET);
            //     h_PhoPt_aJetcut->Fill(bestPhoton.Pt());
            //     h_PhoPhi_aJetcut->Fill(bestPhoton.Phi());

	    

		    /////cout<<nHadJets<<endl;
		    // if( (nHadJets >= 2) )
		    //   {
		    // 	//St selection
		    // 	if(bestPhoton.Pt()>190 && ST >500)
		    // 	  {
	    // h_photonPt->Fill(bestPhoton.Pt());//);
	    // h_njets->Fill(NJets);//);
	    // h_MET->Fill(MET);//);
	    // h_bjets->Fill(BTags);//);
		    //}
		 	// else if(bestPhoton.Pt()>100 && ST >800)
			//   {
			   //  h_photonPt->Fill(bestPhoton.Pt());
                        //     h_njets->Fill(NJets);
                        //     h_MET->Fill(MET);
                        //     h_bjets->Fill(BTags);
                        // // 
  // 	  }
  // 			// else
  // 			//   continue;
  // 	//}
  // 	  // 	  }
  // 	  //     }
  // 	  // }
  //     }
  //   }   // loop over entries
  // //h_photonPt->Draw();
  // //  h->Write();
  // cout<<count<<endl;
}
TLorentzVector AnalyzeLightBSM::getBestPhoton(){
  TLorentzVector v1;
  vector<TLorentzVector> goodPho;
  for(int iPhoton=0;iPhoton<Photons->size();iPhoton++){
    if( ((*Photons_fullID)[iPhoton]) && ((*Photons_hasPixelSeed)[iPhoton]<0.001) ) goodPho.push_back( (*Photons)[iPhoton] );
  }

  if(goodPho.size()==0) return v1;
  sortTLorVec(&goodPho);
  return goodPho[0];
  
}




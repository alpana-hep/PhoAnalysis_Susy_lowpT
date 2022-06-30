#include <stdio.h>

void overlay_cutflows(string f_2016, string f_2017, string f_2018)
{
  char* hname = new char[2000];
  char* hname1 = new char[2000];
  char* hname2 = new char[2000];
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* full_path2 = new char[2000];
  char* path2 = new char[2000];
  sprintf(hname,"/home/bkansal/t3store3/work/MET_analysis/v17_samples/%s",f_2016.c_str());
  sprintf(hname1,"/home/bkansal/t3store3/work/MET_analysis/v17_samples/%s",f_2017.c_str());
  sprintf(hname2,"/home/bkansal/t3store3/work/MET_analysis/v17_samples/%s",f_2018.c_str());
  // path to save png files                                                                                                                    
  sprintf(path2,"/home/kalpana/t3store3/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/Susy_Analysis_2020/");
  TFile * inputfile = new TFile(hname,"READ");
  TFile * inputfile1 = new TFile(hname1,"READ");
  TFile * inputfile2 = new TFile(hname2,"READ");
  sprintf(full_path2,"%s/cutflows.png",path2);

  TH1F *resp = (TH1F*)inputfile->Get("cutflows");
  TH1F *resp1 = (TH1F*)inputfile1->Get("cutflows");
  TH1F *resp2 = (TH1F*)inputfile2->Get("cutflows");
  TCanvas *Canvas_1_n2 = new TCanvas(hist_name, hist_name,65,52,725,527);
  Canvas_1_n2->Range(-60.25,-0.625,562.25,0.625);
  Canvas_1_n2->SetFillColor(0);
  Canvas_1_n2->SetBorderMode(0);
  Canvas_1_n2->SetBorderSize(2);
  Canvas_1_n2->SetFrameBorderMode(0);
  Canvas_1_n2->SetFrameBorderMode(0);
  resp->SetName("2016");
  resp1->SetName("2017");                                                                                            
  resp2->SetName("2018");
  resp2->SetLineColor(kBlue);
  resp1->SetLineColor(kRed);                                                                                                             
  resp->SetLineColor(kBlue);
  THStack *hs = new THStack("hs","; Number of pile-up interactions; Number of ADC hits per HGCAL");
                                                                                           
  hs->Add(resp);
   hs->Add(resp2);
  hs->Add(resp1);
  hs->Draw("nostack");
  auto legend = new TLegend(0.75,0.75,0.99,0.9);
  legend->SetHeader("","C"); 
  legend->AddEntry(resp, resp->GetName(),"l");
  legend->AddEntry(resp1, resp1->GetName(),"l");                                                                                         
  legend->AddEntry(resp2, resp2->GetName(),"l");
  gStyle->SetLegendTextSize(0.03);
  legend->Draw("same");



  Canvas_1_n2->Modified();
  Canvas_1_n2->cd();
  Canvas_1_n2->SetSelected(Canvas_1_n2);


  Canvas_1_n2->SaveAs(full_path2);


}

#!/bin/bash                                                                                                                                      
path=/store/group/lpcsusyphotons/TreeMaker/

#/uscms/home/kalpana/nobackup/public/work/Susy_lowPho_analysis/CMSSW_11_1_0_pre3/src/lowPhopT_Analysis_June22/runList/


for year in Autumn18
do
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.WGJets_MonoPhoton_PtG-40to130_TuneCP5_13TeV-madgraph  >runList_WGJets_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.TTGJets >runList_TTGJets_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.TTJets_HT >runList_TTJets_HT_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.WJets >runList_WJets_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.ZNuNuGJets >runList_ZNuNuGJets_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.QCD >runList_QCD_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.GJets >runList_GJets_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.TTJets_SingleLept >runList_TTJets_inc_${year}.txt
    xrdfs root://cmseos.fnal.gov/ ls ${path} | grep Autumn18.TTJets_DiLept >runList_TTJets_inc_${year}.txt

    
done
#     ls ${path}/SR_${year}.TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_v18.root > runList_TTGJets_${year}_v18.txt
#     ls ${path}/SR_${year}.TTJets_HT*.root > runList_TTJets-HT_${year}_v18.txt
#     ls ${path}/SR_${year}.TTJets_*Lept*.root > runList_TTJets-inc_${year}_v18.txt
#     ls ${path}/SR_${year}.WGJets_MonoPhoton_PtG-*_TuneCP5_13TeV-madgraph_v18.root > runList_WGJets_${year}_v18.txt
#     ls ${path}/SR_${year}.WJetsToLNu_HT-*.root > runList_WJets_${year}_v18.txt
#     ls ${path}/SR_${year}.ZJetsToNuNu_HT*.root > runList_ZJets_${year}_v18.txt
#     ls ${path}/SR_${year}.ZNuNuGJets_MonoPhoton_PtG-*.root > runList_ZGJets_${year}_v18.txt
#     ls ${path}/SR_${year}.QCD_HT*.root > runList_QCD_${year}_v18.txt
#     ls ${path}/SR_${year}.GJets_DR*.root > runList_GJets_DR_${year}_v18.txt
# done

# year=Summer16v3

# ls ${path}/SR_${year}.TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_v18.root > runList_TTGJets_${year}_v18.txt
# ls ${path}/SR_${year}.TTJets_HT*.root > runList_TTJets-HT_${year}_v18.txt
# ls ${path}/SR_${year}.TTJets_*Lept*.root > runList_TTJets-inc_${year}_v18.txt
# ls ${path}/SR_${year}.WGJets_MonoPhoton_PtG-*_TuneCUETP8M1_13TeV-madgraph_v18.root > runList_WGJets_${year}_v18.txt
# ls ${path}/SR_${year}.WJetsToLNu_HT-*.root > runList_WJets_${year}_v18.txt
# ls ${path}/SR_${year}.ZJetsToNuNu_HT*.root > runList_ZJets_${year}_v18.txt
# ls ${path}/SR_${year}.ZNuNuGJets_MonoPhoton_PtG-*.root > runList_ZGJets_${year}_v18.txt
# ls ${path}/SR_${year}.QCD_HT*.root > runList_QCD_${year}_v18.txt
# ls ${path}/SR_${year}.GJets_DR*.root > runList_GJets_DR_${year}_v18.txt

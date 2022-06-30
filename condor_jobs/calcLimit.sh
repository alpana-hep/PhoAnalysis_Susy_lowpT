#!/bin/sh
#input_Scan=$1
anaExe="analyzeLightBSM"
anaArg="signal"
exeAtWorker="worker_SP.sh"
#dataSetType="T6ttZg"
#dataSetType="T5ttttZg"
dataSetType="T5bbbbZg"
#dataSetType="TChiNG"
#dataSetType="TChiWG"
#dataSetType="T5bbbbZg"
ext="Autumn18"
#cp rootoutput_newbin/*v18.root . 
#hist1=$2
#mkdir /eos/uscms/store/user/bkansal/myProduction/limits_rootout/T5bbbbZg/njet6/${hist1}
filesToTransfer="${anaExe}, ${exeAtWorker},runList_QCD_Autumn18.txt, runList_TTGJets_Autumn18.txt, runList_TTJets_HT_Autumn18.txt, runList_WGJets_Autumn18.txt, runList_WJets_Autumn18.txt,runList_ZNuNuGJets_Autumn18.txt,runList_GJets_Autumn18.txt"

#, T5bbbbZg_MassScan.root" 
#PileupHistograms_2018_69mb_pm5.root,PileupHistograms_2016_69mb_pm5.root,PileupHistograms_2017_69mb_pm5.root,T5bbbbZg_MassScan.root"
#,TChiWG_MassScan.root,T5qqqqHg_Summer16v3_MassScan.root,TChiNG_MassScan.root,T5ttttZG_Summer16v3Fast_MassScan.root,T6ttZG_Summer16v3_MassScan.root"
#filesToTransfer="makeDatacard_SBins.C,higgsCombine.tar,GJets_v12.root,QCD_v12.root,TTGJets_v12.root,TTJetsHT_v12.root,WGJetsToLNuG_v12.root,WJetsToLNu_v12.root,ZJetsToNuNu_v12.root,ZGJetsToNuNuG_v12.root,ZGZJ_NuNuG_v12.root,${anaExe},PileupHistograms_0121_69p2mb_pm4p6.root,T5bbbbZg_MassScan.root"
echo "yes"
year=$1
bkg=$2 #"GJets" # "QCD" "ZNuNuGJets" "TTGJets" "TTJets_HT" "WGJets" "WJets" "TTJets_inc")
# while read  year
# do 
# while read -a bkg
# do

echo "no"
jdl_file="condor_${year}_${bkg}_job.jdl"
log_prefix="condor_${year}_${bkg}_job"
file_list="runList_${bkg}_Autumn18.txt"
outfile="Out_${bkg}_Autumn18.root"
echo "universe = vanilla">$jdl_file
echo "Executable = $exeAtWorker">>$jdl_file
echo "Should_Transfer_Files = YES">>$jdl_file
echo "WhenToTransferOutput = ON_EXIT_OR_EVICT">>$jdl_file
echo "Transfer_Input_Files = ${filesToTransfer}">>$jdl_file
echo "Output = ${log_prefix}.stdout">>$jdl_file
echo "Error = ${log_prefix}.stderr">>$jdl_file
echo "Log = ${log_prefix}.condor">>$jdl_file
echo "notification = never">>$jdl_file
echo "x509userproxy = $X509_USER_PROXY">>$jdl_file
echo "use_x509userproxy = True">>$jdl_file
echo "Arguments = ${anaExe} ${file_list} ${outfile} ${year} ${bkg}">>$jdl_file
echo "Queue">>$jdl_file
condor_submit $jdl_file
#done <$bkg_samples
     
#done < $yeear

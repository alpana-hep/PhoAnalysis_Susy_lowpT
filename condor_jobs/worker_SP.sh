#!/bin/sh
#analyzeLightBSM 800 127 FastSim T5qqqqHg
executable=$1
filelist=$2
out_root=$3
year=$4
process=$5

currDir=$(pwd)

######################################
# SETUP CMSSW STUFF...
######################################


echo "======== ${PWD} ==============="

source /cvmfs/cms.cern.ch/cmsset_default.sh
#source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
scram p CMSSW CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src

# export SCRAM_ARCH=slc7_amd64_gcc700
# scram p CMSSW CMSSW_10_2_21
# cd CMSSW_10_2_21/src
eval `scramv1 runtime -sh`
echo $CMSSW_RELEASE_BASE
cd $currDir
echo $currDir

#outRootFile="${anaArg}_${outName}_${gluinoMass}_${nlspMass}_v18"
./$executable $filelist $out_root $year $process

#rm runFileList.txt
#rm -rf CMSSW_11_1_0_pre3/
# mv runFileList.txt tmp/runFileList.txt

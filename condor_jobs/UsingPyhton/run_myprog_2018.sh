#!/bin/bash
# export PATH=/opt/apps/gcc-4.8.5-install/bin:/apps/Python-3.5.10-install-new/bin:/apps/cmake-3.19.4-install/bin:$PATH
# export LD_LIBRARY_PATH=/opt/apps/gmp-6.1.1-install/lib:/opt/apps/mpc-1.0.3-install/lib:/opt/apps/mpfr-3.1.4-install/lib:/opt/apps/gcc-4.8.5-install/lib:/opt/apps/gcc-4.8.5-install/lib64:/apps/Python-3.5.10-install-new/lib:$LD_LIBRARY_PATH
# export INCLUDE=/opt/apps/gmp-6.1.1-install/include:/opt/apps/mpc-1.0.3-install/include:/opt/apps/mpfr-3.1.4-install/include:/opt/apps/gcc-4.8.5-install/include:/apps/Python-3.5.10-install-new/include:$INCLUDE
# export PYTHONPATH=/apps/Python-3.5.10-install-new/lib:$PYTHONPATH
cd /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/Susy_Analysis_2020
# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/gcc-install.tar.gz

# export PATH=$PWD/gcc-4.8.5-install/bin:$PATH
# export LD_LIBRARY_PATH=$PWD/gmp-6.1.1-install/lib:$PWD/mpc-1.0.3-install/lib:$PWD/mpfr-3.1.4-install/lib:$PWD/gcc-4.8.5-install/lib:$PWD/gcc-4.8.5-install/lib64:$LD_LIBRARY_PATH
# export INCLUDE=$PWD/gmp-6.1.1-install/include:$PWD/mpc-1.0.3-install/include:$PWD/mpfr-3.1.4-install/include:$PWD/gcc-4.8.5-install/include:$INCLUDE

# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/Python-3.5.10-install-new.tar.gz
# export PATH=$PWD/Python-3.5.10-install-new/bin:$PATH
# export PYTHONPATH=$PWD/Python-3.5.10-install-new:$PYTHONPATH
# export LD_LIBRARY_PATH=$PWD/Python-3.5.10-install-new/lib:$LD_LIBRARY_PATH
# export HOME=$PWD

# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/root-6.18.04-install.tar.gz
# export PATH=$PWD/root-6.18.04-install/bin:$PATH
# source /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/root-6.18.04-install/bin/thisroot.sh
# cd /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example

# Following two lines are just for cmsenv
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`


echo $PWD
cd /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/Susy_Analysis_2020
./analyzeLightBSM $1 $2 $3 $4
# /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_GJets_DR_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_GJets_DR_v18_Sbin_EventYield.root 2017 GJets_DR

# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_QCD_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_QCD_v18_Sbin_EventYield.root 2017 QCD

# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_ZGJets_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_ZGJets_v18_Sbin_EventYield.root 2017 ZGJets
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_ZJets_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_ZJets_v18_Sbin_EventYield.root 2017 ZJets
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_TTGJets_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_TTGJets_v18_Sbin_EventYield.root 2017 TTGJets
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_TTJets-HT_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_TTJets-HT_v18_Sbin_EventYield.root 2017 TTJets-HT
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_WGJets_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_WGJets_v18_Sbin_EventYield.root 2017 WGJets
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_WJets_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_WJets_v18_Sbin_EventYield.root 2017 WJets
# ./analyzeLightBSM /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/runList/filelist/bkg_samples/v1/runList_TTJets-inc_Fall17_v18.txt  /home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/temp/Out_Fall17_TTJets-inc_v18_Sbin_EventYield.root 2017 TTJets-inc

#python3 ./datapackets/quantile_transf_condorjobs.py $1 $2 $3
# python3 quantile_transf_condorjobs.py $1 $2 $3
#python3 randomSampling_fit.py
#python3 matrix_multiplication.py $1 $2 $3 
#python3 randomSampling_fit.py $1 $2 
#python3  myprog.py $1 $2 $3

#python3 myprog.py $1 $2 $3

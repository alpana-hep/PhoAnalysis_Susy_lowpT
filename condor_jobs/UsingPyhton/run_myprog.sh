#!/bin/bash
#!/bin/sh                                                                                                                                         
currDir=$(pwd)
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
cmsrel CMSSW_11_1_0_pre3
#cd CMSSW_10_2_13/src                                                                                                                             
cd CMSSW_11_1_0_pre3/src
cmsenv

echo $CMSSW_RELEASE_BASE
cd $currDir
echo $currDir
eval `scram runtime -sh`

echo $PWD
./analyzeLightBSM $1 $2 $3 $4

# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_10.txt Out_T5qqqHG_2350_10_v17.root signal  T5bbbbZg_2350_10
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_50.txt Out_T5qqqHG_2350_50_v17.root signal T5bbbbZg_2350_50
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_100.txt Out_T5qqqHG_2350_100_v17.root signal T5bbbbZg_2350_100
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5bbbbZg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_400.txt Out_T5qqqHG_2350_400_v17.root signal T5bbbbZg_2350_400
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_600.txt Out_T5qqqHG_2350_600_v17.root signal T5bbbbZg_2350_600
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1000.txt Out_T5qqqHG_2350_1000_v17.root signal T5bbbbZg_2350_1000
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1200.txt Out_T5qqqHG_2350_1200_v17.root signal T5bbbbZg_2350_1200
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_2000.txt Out_T5qqqHG_2350_2000_v17.root signal T5bbbbZg_2350_2000
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1800.txt Out_T5qqqHG_2350_1800_v17.root signal T5bbbbZg_2350_1800
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_2200.txt Out_T5qqqHG_2350_2200_v17.root signal T5bbbbZg_2350_2200




./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1700.txt out_pMSSM_MCMC_86_7257_Mg1700_BDTcut.root signal MCMC_86_7257_1700
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1600.txt out_pMSSM_MCMC_86_7257_Mg1600_BDTcut.root signal MCMC_86_7257_1600
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1500.txt out_pMSSM_MCMC_86_7257_Mg1500_BDTcut.root signal MCMC_86_7257_1500
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1400.txt out_pMSSM_MCMC_86_7257_Mg1400_BDTcut.root signal MCMC_86_7257_1400
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1800.txt out_pMSSM_MCMC_86_7257_Mg1800_BDTcut.root signal MCMC_86_7257_1800
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1900.txt out_pMSSM_MCMC_86_7257_Mg1900_BDTcut.root signal MCMC_86_7257_1900
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg2000.txt out_pMSSM_MCMC_86_7257_Mg2000_BDTcut.root signal MCMC_86_7257_2000
./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257.txt out_pMSSM_MCMC_86_7257_Mg1750_BDTcut.root signal MCMC_86_7257_1750

  

# ./Analyzelightbsm ./runList/filelist/runList_T5qqqqHg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_127.txt Out_T5qqqHG_2350_127_v17.root signal T5qqqqHg_2350_127
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_150.txt Out_T5qqqHG_2350_150_v17.root signal T5qqqqHg_2350_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_175.txt Out_T5qqqHG_2350_175_v17.root signal T5qqqqHg_2350_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500


# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_150.txt Out_T5qqqHG_2300_150_v17.root signal T5qqqqHg_2300_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_175.txt Out_T5qqqHG_2300_175_v17.root signal T5qqqqHg_2300_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_200.txt Out_T5qqqHG_2300_200_v17.root signal T5qqqqHg_2300_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_300.txt Out_T5qqqHG_2300_300_v17.root signal T5qqqqHg_2300_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_1500.txt Out_T5qqqHG_2300_1500_v17.root signal T5qqqqHg_2300_1500
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_127.txt Out_T5qqqHG_2350_127_v17.root signal T5qqqqHg_2350_127
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_150.txt Out_T5qqqHG_2350_150_v17.root signal T5qqqqHg_2350_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_175.txt Out_T5qqqHG_2350_175_v17.root signal T5qqqqHg_2350_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500

# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_10.txt Out_T5ttttZG_2200_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_50.txt Out_T5ttttZG_2200_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_200.txt Out_T5ttttZG_2200_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_1500.txt Out_T5ttttZG_2200_1500_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_10.txt Out_T5ttttZG_2350_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_50.txt Out_T5ttttZG_2350_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_200.txt Out_T5ttttZG_2350_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_1500.txt Out_T5ttttZG_2350_1500_v17.root signal temp

# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_10.txt Out_T6ttZg_1400_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_50.txt Out_T6ttZg_1400_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_200.txt Out_T6ttZg_1400_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_1000.txt Out_T6ttZg_1400_1000_v17.root signal temp

# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_10.txt Out_T6ttZg_1500_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_50.txt Out_T6ttZg_1500_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_200.txt Out_T6ttZg_1500_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_1000.txt Out_T6ttZg_1500_1000_v17.root signal temp





#python3 ./datapackets/quantile_transf_condorjobs.py $1 $2 $3
# python3 quantile_transf_condorjobs.py $1 $2 $3
#python3 randomSampling_fit.py
#python3 matrix_multiplication.py $1 $2 $3 
#python3 randomSampling_fit.py $1 $2 
#python3  myprog.py $1 $2 $3

#python3 myprog.py $1 $2 $3

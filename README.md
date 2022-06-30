# PhoAnalysis_Susy_lowpT
Susy physics analysis exploring phase space with low photon-pT

## To run the analyzer code
It takes 4 arguments
'./analyzeLightBSM $filelist $out_filename $year $process_tag'
$year = 2016..17..18 for background samples & 'signal' for signal processes
For example

 ./analyzeLightBSM runList/runfilelist_pMSSM_MCMC_86_7257_Mg2000.txt out_pMSSM_MCMC_86_7257_Mg2000.root signal MCMC_86_7257_2000


## To submit condor jobs 
cd condor_jobs/

run ' source calcLimit.sh $year $process'

For example 
   'source calcLimit.sh 2018 GJets'

or Run 
python driversubmitjobs.py

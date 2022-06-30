import os,sys,re,fileinput,string,shutil
from datetime import date
count=0
min_count=0
year=["Autumn18"] #,"Summer16v3"]
number=["2018"] #,"2018","2016"]
#sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
sample_tag=["GJets","QCD","ZNuNuGJets","TTGJets","TTJets_HT","WGJets","WJets","TTJets_inc"]
bkg_samples=["GJets","QCD","ZNuNuGJets","TTGJets","TTJets_HT","WGJets","WJets","TTJets_inc"] #"GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
#bkg_samples=["GJets_DR","QCD"]
count1=0;
for i in year:
    count=0
    for j in bkg_samples:
        #min_ = i
        #max_ = min_+ 100000
        # count=i
        # min_count= i+100000 
        condorSubmit = "condor_submit/submitCondor_%s_%s_Sbin"%(i,j)
        fname1 = "/uscms/home/kalpana/nobackup/public/work/Susy_lowPho_analysis/CMSSW_11_1_0_pre3/src/lowPhopT_Analysis_June22/condor_jobs/runList/runList_%s_%s.txt"%(j,i) #/home/kalpana/t3store3/public/SUSY_GMSB_Aug_2020/CMSSW_11_1_0_pre3/src/Susy_Analysis_2020/runList/filelist/bkg_samples/v1/runList_%s_%s_v18.txt"%(j,i)
        fname = "root://cmseos.fnal.gov.in//store/user/kalpana/Susy_phoMet/Out_%s_%s_lowPhotpT_v18.root"%(i,j)
        jobName = "%s_%s"%(i,j)
        #out_tag="%s_%s" %(sample_tag[count1],year[count1])
        shutil.copyfile("proto_condor_submit",condorSubmit)
        for line in fileinput.FileInput(condorSubmit, inplace=1):
            line=line.replace("outfile", fname)
            line=line.replace("filelist",fname1)
            line=line.replace("year",number[count1])
            line=line.replace("sample_tag",sample_tag[count])
            #line=line.replace("IN1",fname1)
            #    line=line.replace("IN2",fname2)
            line=line.replace("JOBNAME", jobName)
            print line.rstrip()
            
        print condorSubmit
        count+=1
    count1+=1

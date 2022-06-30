import os,sys,re,fileinput,string,shutil
from datetime import date
min_count=0
year=["Fall17","Autumn18","Summer16v3"]
number=["2017","2018","2016"]
#sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
bkg_samples=["GJets","QCD","ZNuNuGJets","TTGJets","TTJets_HT","WGJets","WJets","TTJets_inc"] #"GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJet
#bkg_samples=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
#bkg_samples=["ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets"]
#bkg_samples=["ZGJets","GJets","ZJets","QCD"]
#bkg_samples=["TTJets-HT","TTJets-inc"]
count=0;
for i in year:
    for j in bkg_samples:               
        condorSubmit = "source calcLimit.sh %s %s"%(i,j)
        print condorSubmit
        os.system(condorSubmit)
        

import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'root://cmseos.fnal.gov//store/user/vhegde/MET_HATS2017/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_PUMoriond17_80X_1C34F84E-A8BE-E611-BE97-441EA1733CCC.root'#WJetsToLNu MC
       'root://cmseos.fnal.gov//store/user/vhegde/MET_HATS2017/Run2016G_MET_MINIAOD_03Feb2017-v1_2E1DF27E-37EC-E611-A696-00259073E4E2.root'#Run 2016G MET dataset
    )
)

process.demo = cms.EDAnalyzer('METAnalyzer',
                              metSrc = cms.untracked.InputTag("slimmedMETs")
)

process.p = cms.Path(process.demo)

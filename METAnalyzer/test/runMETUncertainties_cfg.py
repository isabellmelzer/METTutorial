import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        # MC file
        '/store/mc/RunIIFall17MiniAODv2/DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/08CD9834-8142-E811-9578-AC1F6B23C88C.root'
        )
)

process.demo = cms.EDAnalyzer('METUncertainties',
                              metSrc = cms.untracked.InputTag("slimmedMETs"),
                              ifPrint = cms.untracked.bool(False)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("metUncertainties.root")
)

process.p = cms.Path(process.demo)

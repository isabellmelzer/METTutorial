import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/data/Run2018B/SingleMuon/MINIAOD/PromptReco-v1/000/317/696/00000/F8D50D8A-7B70-E811-BE51-FA163E442026.root'
    )
)

process.demo = cms.EDAnalyzer('METAnalyzer',
                              metSrc = cms.untracked.InputTag("slimmedMETs")
#                              metSrc = cms.untracked.InputTag("slimmedMETsPuppi")
)

process.p = cms.Path(process.demo)

import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        '/store/data/Run2018B/SingleMuon/MINIAOD/PromptReco-v1/000/317/696/00000/F8D50D8A-7B70-E811-BE51-FA163E442026.root'
        )
)

process.demo = cms.EDAnalyzer('METCorrections',
                              metSrc = cms.untracked.InputTag("slimmedMETs"),
                              ifPrint = cms.untracked.bool(False),
                              doPhiCorr = cms.untracked.bool(False)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("metCorrections.root")
)

process.p = cms.Path(process.demo)

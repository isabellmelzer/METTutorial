import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        # MC file
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/006F1B9A-81D0-E611-B9CE-0025905AA9CC.root'
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

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

process.demo = cms.EDAnalyzer('METFilters',
                              noiseFilterTag = cms.InputTag("TriggerResults","","RECO"),
                              metSrc = cms.untracked.InputTag("slimmedMETs"),
                              GoodVtxNoiseFilter_Selector_ = cms.string("Flag_goodVertices"),
                              GlobalTightHalo2016NoiseFilter_Selector_ = cms.string("Flag_globalTightHalo2016Filter"),
                              HBHENoiseFilter_Selector_ = cms.string("Flag_HBHENoiseFilter"),
                              HBHENoiseIsoFilter_Selector_ = cms.string("Flag_HBHENoiseIsoFilter"),
                              EcalDeadCellTriggerPrimitiveNoiseFilter_Selector_ = cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter"),
                              BadPFMuonFilter_Selector_ = cms.string("Flag_BadPFMuonFilter"),
                              BadChargedCandidateFilter_Selector_ = cms.string("Flag_BadChargedCandidateFilter"),
                              EEBadScNoiseFilter_Selector_ = cms.string("Flag_eeBadScFilter"),
                              EcalBadCalibFilter_Selector_ = cms.string("Flag_ecalBadCalibFilter")
)

process.p = cms.Path(process.demo)

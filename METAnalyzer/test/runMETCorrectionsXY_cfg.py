import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        #'root://cmseos.fnal.gov//store/user/vhegde/MET_HATS2017/Run2016G_MET_MINIAOD_03Feb2017-v1_2E1DF27E-37EC-E611-A696-00259073E4E2.root'#Run 2016G MET dataset
        # MC file
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/006F1B9A-81D0-E611-B9CE-0025905AA9CC.root'
        )
)

# Add global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '')

from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=False,
                           )

process.demo = cms.EDAnalyzer('METCorrections',
                              metSrc = cms.untracked.InputTag("slimmedMETs"),
                              ifPrint = cms.untracked.bool(False),
                              doPhiCorr = cms.untracked.bool(True)
)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("metCorrectionsXY.root")
)

process.p = cms.Path(process.fullPatMetSequence * process.demo)

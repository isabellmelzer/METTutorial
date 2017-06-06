//METTutorial/METAnalyzer/plugins/METCorrections.cc : prints out event info and MET vector

// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>
#include "TFile.h"
#include "TH1.h"
#include "TLorentzVector.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "Math/GenVector/LorentzVector.h"

// class declaration
class METCorrections : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit METCorrections(const edm::ParameterSet&);
  ~METCorrections();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  edm::InputTag metSrcTag_;
  bool ifPrint_;
  edm::EDGetTokenT<edm::View<pat::MET>>            metSrcToken_;
  int nEvent;
  TH1F *hMET_pT, *hMET_phi;
  TH1F *hMET_pT_phiCorr, *hMET_phi_phiCorr;
  TH1F *hMET_pT_raw, *hMET_phi_raw;
  TH1F *hMET_pT_smear, *hMET_phi_smear;
};

METCorrections::METCorrections(const edm::ParameterSet& iConfig)
{
  ifPrint_     = iConfig.getUntrackedParameter<bool>("ifPrint");
  metSrcTag_   = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
  metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);
  
  nEvent=0;
  edm::Service<TFileService> fs;
  hMET_pT  = fs->make<TH1F>("hMET_pT",  "hMET_pT",  100, 0.0, 200.0);
  hMET_phi = fs->make<TH1F>("hMET_phi", "hMET_phi", 100, -3.2, 3.2);
  hMET_pT_phiCorr  = fs->make<TH1F>("hMET_pT_phiCorr",  "hMET_pT_phiCorr",  100, 0.0, 200.0);
  hMET_phi_phiCorr = fs->make<TH1F>("hMET_phi_phiCorr", "hMET_phi_phiCorr", 100, -3.2, 3.2);
  usesResource("TFileService");
}

METCorrections::~METCorrections()
{
 
}

// ------------ method called for each event  ------------
void
METCorrections::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle<edm::View<pat::MET> > meth;
  iEvent.getByToken(metSrcToken_,meth);
  const pat::MET &met = meth->front();

  if(ifPrint_) {
    // Print event information
    std::cout << "run " << iEvent.id().run()
	      << " lumi " << iEvent.id().luminosityBlock()
	      << " event " << iEvent.id().event();
    // Print corr MET (default out-of-box)
    std::cout << " pt " <<  met.pt() 
	      << " phi " <<  met.phi() 
	      << std::endl;
  }    

  TLorentzVector rawMet(0,0,0,0);
  rawMet.SetPtEtaPhiE(met.uncorPt(),0,met.uncorPhi(),met.uncorPt());
  if(ifPrint_) {    
    std::cout << "raw pt " <<  rawMet.Pt() << " " << met.corPt(pat::MET::Raw) 
	      << " type1 " << met.corPt(pat::MET::Type1) 
	      << " Type1Smear " << met.corPt(pat::MET::Type1Smear)
	      << " type1JESup " << met.shiftedPt(pat::MET::JetEnUp) 
	      << " type1JESdn " << met.shiftedPt(pat::MET::JetEnDown) 
	      << " genMET "     << met.genMET()->pt() 
	      << std::endl;
  }
  
  hMET_pT ->Fill(met.pt());
  hMET_phi->Fill(met.phi());

  //hMET_pT_type1 ->Fill(met.corPt(pat::MET::Type1));
  //hMET_phi_type1->Fill(met.corPhi(pat::MET::Type1));

  hMET_pT_phiCorr ->Fill(met.corPt(pat::MET::Type1XY));
  hMET_phi_phiCorr->Fill(met.corPhi(pat::MET::Type1XY));

  nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void 
METCorrections::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METCorrections::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METCorrections::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METCorrections);

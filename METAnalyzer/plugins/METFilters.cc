// METTutorial/METFilters/plugins/METFilters.cc : example how to access MET cleaning information

// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "TLorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

// class declaration
class METFilters : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit METFilters(const edm::ParameterSet&);
  ~METFilters();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  edm::InputTag metSrcTag_;
  edm::EDGetTokenT<edm::View<pat::MET>>            metSrcToken_;
  int nEvent;

  int NpassFilterHBHE;
  int NpassFilterEEBadSC;

  edm::EDGetTokenT<edm::TriggerResults> noiseFilterTag_;

  std::string HBHENoiseFilter_Selector_;
  std::string EEBadScNoiseFilter_Selector_;




};

METFilters::METFilters(const edm::ParameterSet& iConfig):
noiseFilterTag_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("noiseFilterTag")))
{
  metSrcTag_ = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
  metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);

  HBHENoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("HBHENoiseFilter_Selector_");
  EEBadScNoiseFilter_Selector_ =  iConfig.getParameter<std::string> ("EEBadScNoiseFilter_Selector_");



  nEvent=0;

  usesResource("TFileService");
}

METFilters::~METFilters()
{
 
}

// ------------ method called for each event  ------------
void
METFilters::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle<edm::View<pat::MET> > meth;
  iEvent.getByToken(metSrcToken_,meth);
  const pat::MET &met = meth->front();

  // Print event information
  std::cout << "run " << iEvent.id().run()
	    << " lumi " << iEvent.id().luminosityBlock()
	    << " event " << iEvent.id().event();
  // Print corr MET (default out-of-box)
  std::cout << " pt " <<  met.pt() 
	    << std::endl;

  nEvent++;

  int passFilterHBHE=0;
  int passFilterEEBadSC=0;


  edm::Handle<edm::TriggerResults> noiseFilterBits_; //our trigger result object
  iEvent.getByToken(noiseFilterTag_,noiseFilterBits_);
  const edm::TriggerNames &names = iEvent.triggerNames(*noiseFilterBits_);


  for (unsigned int i = 0, n = noiseFilterBits_->size(); i < n; ++i) {
    //  std::cout << names.triggerName(i) << std::endl;
    if (names.triggerName(i) == HBHENoiseFilter_Selector_)
      passFilterHBHE=noiseFilterBits_->accept(i);
    if (names.triggerName(i) == EEBadScNoiseFilter_Selector_)
      passFilterEEBadSC=noiseFilterBits_->accept(i);
  }


  // Print filter information

  std::cout<<"passFilterHBHE ="<< passFilterHBHE <<", "
           <<"passFilterEEBadSC ="<< passFilterEEBadSC 
           << std::endl;

}


// ------------ method called once each job just before starting event loop  ------------
void 
METFilters::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METFilters::endJob() 
{

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METFilters::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METFilters);

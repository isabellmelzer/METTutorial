// -*- C++ -*-
//
// Package:    METTutorial/METAnalyzer
// Class:      METAnalyzer
// 
/**\class METAnalyzer METAnalyzer.cc METTutorial/METAnalyzer/plugins/METAnalyzer.cc
*/


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

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TLorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

//---------------for MC-----------------------
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


//
// class declaration
//
class METAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit METAnalyzer(const edm::ParameterSet&);
  ~METAnalyzer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  edm::InputTag metSrcTag_;
  edm::EDGetTokenT<edm::View<pat::MET>>            metSrcToken_;
  edm::InputTag evtinfoSrcTag_;
  edm::EDGetTokenT<GenEventInfoProduct>            eventinfoToken_;
  edm::InputTag vtxSrcTag_;
  edm::EDGetTokenT<reco::VertexCollection>         vertexCollectionToken_;
  edm::InputTag puinfoSrcTag_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo>> puInfoToken_;

  int nEvent;
};

METAnalyzer::METAnalyzer(const edm::ParameterSet& iConfig)
{

  evtinfoSrcTag_ = iConfig.getUntrackedParameter<edm::InputTag>("generator");
  eventinfoToken_ = consumes<GenEventInfoProduct>(evtinfoSrcTag_);

  //vertexCollectionToken_(consumes<reco::VertexCollection>(edm::InputTag("offlineSlimmedPrimaryVertices"))),
  //puInfoToken_(consumes<std::vector<PileupSummaryInfo>>(edm::InputTag("slimmedAddPileupInfo")))

  metSrcTag_ = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
  metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);

  nEvent=0;
  //now do what ever initialization is needed
  usesResource("TFileService");

}


METAnalyzer::~METAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called for each event  ------------
void
METAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  //edm::Handle<GenEventInfoProduct>eventinfo;
  //iEvent.getByToken(eventinfoToken_,eventinfo);
  //edm::Handle<reco::VertexCollection> vertices;
  //iEvent.getByToken(vertexCollectionToken_,vertices);
  //edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
  //iEvent.getByToken(puInfoToken_, puInfo);

  edm::Handle<edm::View<pat::MET> > meth;
  iEvent.getByToken(metSrcToken_,meth);
  const pat::MET &met = meth->front();

  TLorentzVector rawMet(0,0,0,0);
  rawMet.SetPtEtaPhiE(met.uncorPt(),0,met.uncorPhi(),met.uncorPt());
  
  // Print event information
  std::cout << "run " << iEvent.id().run()
	    << " lumi " << iEvent.id().luminosityBlock()
	    << " event " << iEvent.id().event();
  // Print raw MET
  std::cout << " rawpt " <<  rawMet.Pt() 
	    << " rawpx " <<  rawMet.Px() 
	    << " rawpy " <<  rawMet.Py() 
	    << " rawphi " << rawMet.Phi();
  // Print corr MET (default out-of-box)
  std::cout << " pt " <<  met.pt() 
	    << " px " <<  met.px() 
	    << " py " <<  met.py() 
	    << " phi " <<  met.phi() 
	    << std::endl;

  

  nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void 
METAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METAnalyzer);

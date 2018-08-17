//Mettutorial/METAnalyzer/plugins/METUncertainties.cc : prints out event info and MET vector

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
class METUncertainties : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
public:
  explicit METUncertainties(const edm::ParameterSet&);
  ~METUncertainties();
  
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
  int nEvt[3], nEvt_jecup[3], nEvt_jecdn[3];
  double metth[3] = {0.0, 40.0, 80.0} ;
  TH1F *hMET_pT, *hMET_phi;
  TH1F *hMET_pT_jecup, *hMET_phi_jecup;
  TH1F *hMET_pT_jecdn, *hMET_phi_jecdn;
};

METUncertainties::METUncertainties(const edm::ParameterSet& iConfig)
{
  ifPrint_     = iConfig.getUntrackedParameter<bool>("ifPrint");
  metSrcTag_   = iConfig.getUntrackedParameter<edm::InputTag>("metSrc");
  metSrcToken_ = consumes<edm::View<pat::MET> >(metSrcTag_);
  
  nEvent=0; 
  for(int ii=0; ii<3; ii++) {nEvt[ii]=0; nEvt_jecup[ii]=0; nEvt_jecdn[ii]=0;}
  
  edm::Service<TFileService> fs;
  hMET_pT       = fs->make<TH1F>("hMET_pT",         "hMET_pT",       100, 0.0, 200.0);
  hMET_phi      = fs->make<TH1F>("hMET_phi",        "hMET_phi",      100, 0.0, 200.0 );
  hMET_pT_jecup  = fs->make<TH1F>("hMET_pT_jecup",  "hMET_pT_jecup", 100, 0.0, 200.0);
  hMET_phi_jecup = fs->make<TH1F>("hMET_phi_jecup", "hMET_phi_jecup",100, 0.0, 200.0 );
  hMET_pT_jecdn  = fs->make<TH1F>("hMET_pT_jecdn",  "hMET_pT_jecdn", 100, 0.0, 200.0);
  hMET_phi_jecdn = fs->make<TH1F>("hMET_phi_jecdn", "hMET_phi_jecdn",100, 0.0, 200.0 );
  usesResource("TFileService");
}

METUncertainties::~METUncertainties()
{
  std::cout << "nEvents " << "      nEventsJECup "  << "   nEventsJECdn " << std::endl;
  for(int ii=0; ii<3; ii++) {
    if(ii==0)      std::cout << "met: 0-40 GeV  ";
    else if(ii==1) std::cout << "met: 40-80 GeV ";
    else if(ii==2) std::cout << "met: >80       ";
    std::cout << nEvt[ii] << " " << nEvt_jecup[ii] 
	      << " " << nEvt_jecdn[ii] << std::endl;
  }  

}

// ------------ method called for each event  ------------
void
METUncertainties::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
	      << " pt:JEC up "  << met.shiftedPt(pat::MET::JetEnUp, pat::MET::Type1)
	      << " phi:JEC up " << met.shiftedPhi(pat::MET::JetEnUp, pat::MET::Type1)
	      << " pt:JEC up "  << met.shiftedPt(pat::MET::JetEnDown, pat::MET::Type1)
	      << " phi:JEC dn " << met.shiftedPhi(pat::MET::JetEnDown, pat::MET::Type1)
	      << std::endl;
  }    

  // default met in miniAOD
  hMET_pT ->Fill(met.pt());
  hMET_phi->Fill(met.phi());

  // access met shifted for jec up 
  hMET_pT_jecup ->Fill(met.shiftedPt(pat::MET::JetEnUp, pat::MET::Type1));
  hMET_phi_jecup->Fill(met.shiftedPhi(pat::MET::JetEnUp, pat::MET::Type1));

  // access met shifted for jec down
  hMET_pT_jecdn ->Fill(met.shiftedPt(pat::MET::JetEnDown, pat::MET::Type1));
  hMET_phi_jecdn->Fill(met.shiftedPhi(pat::MET::JetEnDown, pat::MET::Type1));

  for(int ii=0; ii<2; ii++) {
    if((met.pt() > metth[ii]) && (met.pt() < metth[ii+1])) nEvt[ii]+=1;
    if((met.shiftedPt(pat::MET::JetEnUp, pat::MET::Type1) > metth[ii]) && (met.shiftedPt(pat::MET::JetEnUp, pat::MET::Type1) < metth[ii+1])) nEvt_jecup[ii]+=1;
    if((met.shiftedPt(pat::MET::JetEnDown, pat::MET::Type1) > metth[ii]) && (met.shiftedPt(pat::MET::JetEnDown, pat::MET::Type1) < metth[ii+1])) nEvt_jecdn[ii]+=1;
  }  
  if(met.pt() > metth[2]) nEvt[2]+=1;
  if(met.shiftedPt(pat::MET::JetEnUp, pat::MET::Type1) > metth[2]) nEvt_jecup[2]+=1;
  if(met.shiftedPt(pat::MET::JetEnDown, pat::MET::Type1) > metth[2])  nEvt_jecdn[2]+=1;
  
  // for additional corrections, look for METUncertainty
  // in DataFormats/PatCandidates/interface/MET.h


  nEvent++;
}


// ------------ method called once each job just before starting event loop  ------------
void 
METUncertainties::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METUncertainties::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METUncertainties::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(METUncertainties);

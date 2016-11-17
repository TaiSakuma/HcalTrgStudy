// -*- C++ -*-
//__________________________________________________________________||
#include <memory>
#include <iostream>
#include <iomanip>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "DataFormats/DetId/interface/DetId.h"

//__________________________________________________________________||
class PositionDump : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PositionDump(const edm::ParameterSet&);
      ~PositionDump();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

};

//__________________________________________________________________||
PositionDump::PositionDump(const edm::ParameterSet& iConfig)

{
   usesResource("TFileService");
}


//__________________________________________________________________||
PositionDump::~PositionDump()
{

}

//__________________________________________________________________||
void
PositionDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::ESHandle<CaloGeometry> caloGeoHandle;
  edm::ESHandle<HcalTrigTowerGeometry> hcalTrigTowGeoHandle;
  iSetup.get<CaloGeometryRecord>().get(caloGeoHandle);   
  iSetup.get<CaloGeometryRecord>().get(hcalTrigTowGeoHandle);

  // caloGeoHandle->getValidDetIds(DetId::Hcal);
  auto detids = caloGeoHandle->getValidDetIds();

  for(auto detid : detids)
    {
      if(!(detid.det() == 4)) continue; // 4 is probably HCAL
      if(!(detid.subdetId() == 4)) continue; // 4 is probably HF

      HcalDetId hcaldetid(detid);

      auto thisCell = caloGeoHandle->getGeometry(detid);
      // auto position = thisCell->getPosition();
      
      std::cout << std::setw(4) << detid.det()
		<< std::setw(4) << detid.subdetId()
		<< std::setw(4) << hcaldetid.ieta()
		<< std::setw(4) << hcaldetid.iphi()
		<< std::setw(10) << thisCell->etaPos()
		<< " " << std::setw(20) << thisCell->phiPos()
		<< std::endl;
    }
}


//__________________________________________________________________||
void 
PositionDump::beginJob()
{
}

//__________________________________________________________________||
void 
PositionDump::endJob() 
{
}

//__________________________________________________________________||
void
PositionDump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//__________________________________________________________________||
DEFINE_FWK_MODULE(PositionDump);

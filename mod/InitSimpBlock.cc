///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"

#include "GeometryService/inc/GeometryService.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "GeometryService/inc/VirtualDetector.hh"

#include "MCDataProducts/inc/SimParticle.hh"
#include "MCDataProducts/inc/SimParticleCollection.hh"
#include "MCDataProducts/inc/StepPointMCCollection.hh"
#include "MCDataProducts/inc/StrawHitMCTruthCollection.hh"
#include "MCDataProducts/inc/PtrStepPointMCVectorCollection.hh"
#include "DataProducts/inc/VirtualDetectorId.hh"

#include "RecoDataProducts/inc/StrawHitCollection.hh"

#include "Stntuple/obj/TSimpBlock.hh"

#include "Stntuple/mod/InitStntupleDataBlocks.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

//-----------------------------------------------------------------------------
int StntupleInitMu2eSimpBlock(TStnDataBlock* Block, AbsEvent* AnEvent, int mode) 
{
  // fill generator particle data block (GENP - GENerator Particles - 
  // is the name from Run I)
  const char* oname = {"StntupleInitMu2eSimpBlock"};

  static char   strh_module_label[100], strh_description[100];
  static char   g4_module_label  [100], g4_description  [100];

  std::vector<art::Handle<mu2e::SimParticleCollection>> list_of_sp;

  const mu2e::SimParticleCollection*       simp_coll(0);
  const mu2e::SimParticle*                 sim(0);
  art::Handle<mu2e::SimParticleCollection> simp_handle;
  const mu2e::StrawHitCollection*          list_of_straw_hits(0);

  double        px, py, pz, mass, energy;
  int           id, parent_id, generator_id, n_straw_hits(0), nhits;
  int           pdg_code, start_vol_id, end_vol_id, creation_code, termination_code;
  TParticlePDG* part;
  TDatabasePDG* pdg_db = TDatabasePDG::Instance();
  TSimParticle* simp;

  TSimpBlock* simp_block = (TSimpBlock*) Block;
  simp_block->Clear();

  Block->GetModuleLabel("mu2e::StrawHitCollection",strh_module_label);
  Block->GetDescription("mu2e::StrawHitCollection",strh_description );

  Block->GetModuleLabel("mu2e::StepPointMCCollection",g4_module_label);
  Block->GetDescription("mu2e::StepPointMCCollection",g4_description );

  art::Handle<mu2e::StrawHitCollection> shHandle;
  if (strh_module_label[0] != 0) {
    if (strh_description[0] == 0) AnEvent->getByLabel(strh_module_label,shHandle);
    else                          AnEvent->getByLabel(strh_module_label,strh_description,shHandle);
    if (shHandle.isValid()) {
      list_of_straw_hits = shHandle.product();
      n_straw_hits      = list_of_straw_hits->size();
    }
  }

  const mu2e::PtrStepPointMCVectorCollection* stepPointMCVectorCollection(0);
  const mu2e::StepPointMC*                    step;

  art::Handle<mu2e::PtrStepPointMCVectorCollection> mcptrHandleStraw;
  AnEvent->getByLabel(strh_module_label,mcptrHandleStraw);
  if (mcptrHandleStraw.isValid()) {
    stepPointMCVectorCollection = mcptrHandleStraw.product();
  }

  mu2e::GeomHandle<mu2e::VirtualDetector> vdg;
//-----------------------------------------------------------------------------
  //  const art::Provenance*                 prov;
//  art::Selector  selector(art::ProductInstanceNameSelector(""));
//  AnEvent->getMany(selector, list_of_sp);
//
//   for (std::vector<art::Handle<mu2e::SimParticleCollection>> ::const_iterator it = list_of_sp.begin();
//        it != list_of_sp.end(); it++) {
//     handle = it.operator -> ();

  art::Selector  selector(art::ProcessNameSelector("")             && 
			  art::ModuleLabelSelector(g4_module_label)   );
  AnEvent->get(selector,simp_handle);

  if (simp_handle.isValid()) {
    simp_coll = simp_handle.product();
    //      prov = handle->provenance();
      
    //       printf("moduleLabel = %-20s, producedClassname = %-30s, productInstanceName = %-20s\n",
    // 	     prov->moduleLabel().data(),
    // 	     prov->producedClassName().data(),
    // 	     prov->productInstanceName().data());

    for (mu2e::SimParticleCollection::const_iterator ip = simp_coll->begin(); ip != simp_coll->end(); ip++) {
      sim      = &ip->second;
      //      if (! sim->isPrimary())                               goto NEXT_PARTICLE;
      id        = sim->id().asInt();
      parent_id = -1;
      if (sim->parent()) parent_id = sim->parent()->id().asInt();

      pdg_code         = (int) sim->pdgId();
      generator_id     = sim->generatorIndex();   // ID of the MC generator
      creation_code    = sim->creationCode();
      termination_code = sim->stoppingCode();

      start_vol_id     = sim->startVolumeIndex();
      end_vol_id       = sim->endVolumeIndex();
      
      px     = sim->startMomentum().x();
      py     = sim->startMomentum().y();
      pz     = sim->startMomentum().z();

      part   = pdg_db->GetParticle(pdg_code);
      if (part) {
	mass   = part->Mass();
	energy = sqrt(px*px+py*py+pz*pz+mass*mass);
      }
      else {
	mf::LogWarning(oname) << " ERROR: PDG code " << pdg_code << "not in the database, set particle mass to 0\n";
	energy = sqrt(px*px+py*py+pz*pz);
      }

      simp   = simp_block->NewParticle(id, parent_id, pdg_code, 
				       creation_code, termination_code,
				       start_vol_id, end_vol_id,
				       generator_id);
      simp->SetStartMom(px, py, pz, energy);
      const CLHEP::Hep3Vector* sp = &sim->startPosition();
      simp->SetStartPos(sp->x(),sp->y(),sp->z(),sim->startGlobalTime());
      simp->SetEndMom  (sim->endMomentum().x(),
			sim->endMomentum().y(),
			sim->endMomentum().z(),
			sim->endMomentum().e());
      const CLHEP::Hep3Vector* ep = &sim->endPosition();
      simp->SetEndPos(ep->x(),ep->y(),ep->z(),sim->endGlobalTime());
//-----------------------------------------------------------------------------
// particle parameters at virtual detectors
//-----------------------------------------------------------------------------
    if (vdg->nDet() > 0) {
      art::Handle<mu2e::StepPointMCCollection> vdhits;
      AnEvent->getByLabel(g4_module_label,"virtualdetector",vdhits);
      if (!vdhits.isValid()) {
	printf("[%s] ERROR: StepPointMCCollection %s:virtualdetector NOT FOUND\n",
	       g4_module_label,oname);
      }
      else {
	int nvdhits = vdhits->size();
	for (int i=0; i<nvdhits; i++) {
	  const mu2e::StepPointMC* hit = &(*vdhits)[i];
	  
	  mu2e::VirtualDetectorId vdid(hit->volumeId());

	  if (vdid.id() == mu2e::VirtualDetectorId::ST_Out) {

	    const mu2e::SimParticle* sim = hit->simParticle().get();

	    if (sim == NULL) {
	      printf(">>> ERROR: %s sim == NULL\n",oname);
	    }
	    int sim_id = sim->id().asInt();
	    if (sim_id == id) {
	      simp->SetMomTargetEnd(hit->momentum().mag());
	    }
	  }
	  else if (vdid.isTrackerFront()) {

	    //	    TAnaDump::Instance()->printStepPointMC(hit,"");

	    art::Ptr<mu2e::SimParticle> const& simptr = hit->simParticle();
	    const mu2e::SimParticle* sim = simptr.get();

	    if (sim == NULL) {
	      printf("[%s] ERROR: sim == NULL. CONTINUE.\n",oname);
	    }
	    else {
	      int sim_id = sim->id().asInt();
	      if (sim_id == id) {
		simp->SetMomTrackerFront(hit->momentum().mag());
	      }
	    }
	  }
	  
	}
      }
    }
//------------------------------------------------------------------------------
// now look at the straw hits
//-----------------------------------------------------------------------------
      nhits = -1;
      if (stepPointMCVectorCollection && (stepPointMCVectorCollection->size() > 0)) {
	nhits = 0;
	for (int i=0; i<n_straw_hits; i++) {
	  mu2e::PtrStepPointMCVector const& mcptr(stepPointMCVectorCollection->at(i) );

	  step = mcptr[0].get(); // operator ->();
    
	  art::Ptr<mu2e::SimParticle> const& simptr = step->simParticle(); 
	  art::Ptr<mu2e::SimParticle> mother = simptr;
	  while(mother->hasParent())  mother = mother->parent();
	  const mu2e::SimParticle*    sim    = mother.get(); // operator ->();

	  int sim_id = sim->id().asInt();

	  if (sim_id == id) {
	    nhits += 1;
	  }
	}
      }
      simp->SetNStrawHits(nhits);

      //    NEXT_PARTICLE:;
    }
  }
  else {
    printf(" [%s] ERROR: SimParticleCollection %s:virtualdetector NOT FOUND",
	   oname,g4_module_label);
    printf(". BAIL OUT. \n");
    return -1;
  }
  //  }
//-----------------------------------------------------------------------------
// all particles are stored, now - calculate nuber of straw hits
//-----------------------------------------------------------------------------

  return 0;
}



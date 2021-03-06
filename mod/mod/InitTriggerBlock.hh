///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __InitTriggerBlock__
#define __InitTriggerBlock__

#include <string.h>

#include "canvas/Utilities/InputTag.h"

#include "Stntuple/obj/TStnInitDataBlock.hh"
#include "Stntuple/obj/TStnTriggerBlock.hh"

class StntupleInitTriggerBlock : public TStnInitDataBlock {
public:
  //  art::InputTag   fCrvRecoPulseCollTag;
  //  art::InputTag   fCrvCoincidenceClusterCollTag;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
public:

  // void   SetCrvRecoPulseCollTag         (std::string& Tag) { fCrvRecoPulseCollTag          = art::InputTag(Tag); }
  // void   SetCrvCoincidenceClusterCollTag(std::string& Tag) { fCrvCoincidenceClusterCollTag = art::InputTag(Tag); }

  
  virtual int InitDataBlock(TStnDataBlock* Block, AbsEvent* Evt, int Mode);
  virtual int ResolveLinks (TStnDataBlock* Block, AbsEvent* Evt, int Mode);

};

#endif

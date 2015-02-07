//
// Read the tracks added to the event by the track finding and fitting code.
//
// $Id: TStnTrack.hh,v 1.3 2014/09/13 01:16:28 murat Exp $
// $Author: murat $
// $Date: 2014/09/13 01:16:28 $
//
// Contact person Pavel Murat
//
#ifndef murat_inc_TStnTrack_hh
#define murat_inc_TStnTrack_hh

// Mu2e includes.

// #include "CLHEP/Geometry/HepPoint.h"
// #include "CLHEP/Vector/ThreeVector.h"
// #include "CLHEP/Matrix/SymMatrix.h"
// #include "CLHEP/Matrix/Vector.h"
// 
// #include "TrkBase/TrkHelixUtils.hh"
// #include "TrkBase/HelixParams.hh"
// #include "KalmanTrack/KalHit.hh"
// #include "KalmanTests/inc/KalRepCollection.hh"

// storable objects (data products)
// #include "RecoDataProducts/inc/StrawHitCollection.hh"
// #include "RecoDataProducts/inc/CaloCrystalHitCollection.hh"
// #include "RecoDataProducts/inc/CaloHitCollection.hh"
// #include "RecoDataProducts/inc/CaloClusterCollection.hh"
// #include "MCDataProducts/inc/GenParticleCollection.hh"


// #include "TrackCaloMatching/inc/TrackClusterLink.hh"
// #include "TrackCaloMatching/inc/TrkToCaloExtrapolCollection.hh"
// #include "TrackCaloMatching/inc/TrkToCaloExtrapol.hh"

// C++ includes.
#include <iostream>

#include "TString.h"
#include "TFolder.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "Stntuple/base/TBitset.hh"
// #include "TStnCluster.hh"

					// 'KalRep' is a BaBar class
class KalRep ;
class TStnCluster;

namespace mu2e {
  class CaloCluster;
  class TrkToCaloExtrapol;
}

// namespace murat {

class TStnTrack : public TObject {

  enum {
    kNFreeIntsV1   =  9,
    kNFreeFloatsV1 = 10,

    kNFreeIntsV2   =  9,
    kNFreeFloatsV2 =  6,

    kNFreeIntsV3   =  9,
    kNFreeFloatsV3 =  6,

    kNFreeIntsV4   =  7,		// hopefully, correct
    kNFreeFloatsV4 =  6,		// hopefully, correct

    kNFreeIntsV5   =  7,
    kNFreeFloatsV5 =  6,

    kNFreeIntsV6   =  7,
    kNFreeFloatsV6 =  6,

    kNFreeInts     =  7,
    kNFreeFloats   =  5
  };

  enum { kMaxNLayers = 88 }; // 22x2*2

					// added 3 chi*2's
public:
  enum { kNVanesV4 = 4,
	 kNVanesV5 = 4,
	 kNDisks   = 2			// there are only two disks, no vane support
  };

  struct InterData_t {
    int          fID;			// = -1 if no intersection
    int          fClusterIndex;         // cluster index in the list of clusters
    float        fTime;			// extrapolated track time, not corrected by _dtOffset
    float        fEnergy;		// closest cluster energy
    float        fXTrk;
    float        fYTrk;
    float        fZTrk;
    float        fNxTrk;		// track direction cosines in the intersection point
    float        fNyTrk;
    float        fNzTrk;
    float        fXCl;			// cluster coordinates
    float        fYCl;
    float        fZCl;
    float        fDx;			// TRK-CL
    float        fDy;			// TRK-CL
    float        fDz;
    float        fDt;			// TRK-CL , _corrected_ by _dTOffset (!)
    float        fDu;			// added in V6
    float        fDv;			// added in V6
    float        fChi2Match;		// track-cluster match chi&^2 (coord)
    float        fChi2Time;		// track-cluster match chi&^2 (time)
    float        fPath;			// track path in the disk
    float        fIntDepth;             // assumed interaction depth, added in V6;
    const mu2e::CaloCluster*       fCluster;
    const mu2e::TrkToCaloExtrapol* fExtrk;
  };
    
  TLorentzVector            fMomentum;         // this assumes DELE fit hypothesis
  
  TBitset                   fHitMask;	       // bit #i: 1 if there is a hit 
  TBitset                   fExpectedHitMask;   // bit #i: 1 if expect to have a hit at this Z

  int                       fNumber;       // track index in the list of reconstructed tracks
  int                       fNHyp;          // number of hyp's with successfull fits
  int                       fBestHyp[2];    // hypothesis with the best chi2/ndof
  int                       fIDWord;	    // now - for selection "C"
  
  int                       fNActive;	    // total number of hits
  int                       fVaneID;	    // 
  int                       fDiskID;	    // 
  int                       fPdgCode;       // PDF code of the particle produced most hits
  int                       fNGoodMcHits;   // Nhits produced by the associated MC particle
  int                       fPartID;        // MC particle ID (number in the list)
  int                       fNMcStrawHits;  // Nhits by associated particle in the straw tracker
  int                       fAlgorithmID;   // bit-packed : (alg_mask << 16 ) | best
  int                       fInt[kNFreeInts];     // provision for future expension
  
  float                     fChi2;
  float                     fChi2C;          // calculated...
  float                     fFitCons;
  float                     fT0;
  float                     fT0Err;
  float                     fFitMomErr;
  float                     fTanDip;
  float                     fP;		 // total momentum
  float                     fCharge;
  float                     fPt;	 // transverse momentum
  float                     fD0;
  float                     fZ0;

  float                     fPStOut;    // MC momentum in the VD ST_Out 
  float                     fPFront;    // MC momentum in the VD front of the tracker

  float                     fClusterE;	// energy of the associated cluster
  float                     fDt;
  float                     fEp;
  float                     fDx;	// about 0 for vanes
  float                     fDy;	// 
  float                     fDz;	// about 0 for disks
  
  float                     fEleLogLHCal;  // log likelihood of the electron hypothesis
  float                     fMuoLogLHCal;	// log lilelihood of the muon     hypothesis

  float                     fRSlope;	    // timing residual slope dres(T)/dZ
  float                     fRSlopeErr;

  float                     fLogLHRXs;      // XSlope-only-based likelihood

  float                     fEleLogLHDeDx;  // dE/dX LH calculated by Vadim based 
  float                     fMuoLogLHDeDx;  // 
  float                     fX1;	    // momentum defined at Z1
  float                     fY1;
  float                     fZ1;
  float                     fP0;            // momentum defined at Z0
  float                     fP2;            // momentum defined at Z0
  float                     fC0;	    // curvature
  float                     fFloat[kNFreeFloats]; // provision for future I/O expansion

  InterData_t               fDisk[kNDisks];  // track intersections with disks
//-----------------------------------------------------------------------------
//  transient data members, all the persistent ones should go above
//-----------------------------------------------------------------------------
  const mu2e::TrkToCaloExtrapol* fExtrk;              //! 
  const mu2e::CaloCluster*       fClosestCaloCluster; //!

  InterData_t*                   fVMinS;	      //! intersection with min S
  InterData_t*                   fVMaxEp;	      //! intersection with max E/P
  KalRep*                        fKalRep[4];          //! different fits, sequence: e-, e+, mu-, mu+

  int                            fITmp[ 5];           //!
  Float_t                        fTmp [10];           //! for temporary analysis needs
  int                            fNHPerStation[50];   //! currently - 44 x 3
  TStnCluster*                   fCluster;            //! 
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
  TStnTrack(int i = -1);
  ~TStnTrack();

  TLorentzVector* Momentum() { return &fMomentum; }

  TBitset*        HitMask        () { return &fHitMask; }
  TBitset*        ExpectedHitMask() { return &fExpectedHitMask; }
  
  int    Number   () { return fNumber; }
  int    NActive  () { return fNActive;}
  int    NClusters();
  int    NMcStrawHits() { return fNMcStrawHits; }
  int    NGoodMcHits () { return fNGoodMcHits; }

  int    AlgorithmID() { return fAlgorithmID; }
  int    BestAlg    () { return fAlgorithmID & 0xffff; }
  int    AlgMask    () { return (fAlgorithmID >> 16) & 0xffff; }

  float  T0       () { return fT0;    }
  float  T0Err    () { return fT0Err; }
  float  FitCons  () { return fFitCons; }
  float  FitMomErr() { return fFitMomErr; }
  float  TanDip   () { return fTanDip; }

  float  D0       () { return fD0; }
  float  Z0       () { return fZ0; }
  float  Dt       () { return fDt; }
  float  Ep       () { return fEp; }
  float  Dy       () { return fDy; }
  float  Dz       () { return fDz; }
  float  Chi2Dof  () { return fChi2/(fNActive-5+1.e-12) ; }
  float  ClusterE () { return fClusterE;    }

  float  EleLogLHCal() { return fEleLogLHCal; }
  float  MuoLogLHCal() { return fMuoLogLHCal; }
  float  LogLHRCal  () { return fEleLogLHCal-fMuoLogLHCal; }

  float  RSlope   () { return fRSlope;    }
  float  RSlopeErr() { return fRSlopeErr; }
  float  XSlope   () { return fRSlope/fRSlopeErr; }

					// track-only likelihood based on timing residuals

  float  EleLogLHDeDx() { return fEleLogLHDeDx; }
  float  MuoLogLHDeDx() { return fMuoLogLHDeDx; }
  float  LogLHRDeDx  () { return fEleLogLHDeDx-fMuoLogLHDeDx; }
  float  LogLHRXs    () { return fLogLHRXs; }

  float  LogLHRTrk   () { return LogLHRXs()+LogLHRDeDx(); }

  TStnCluster*   Cluster() { return fCluster; }

  float  P () { return fP; }
  float  Pt() { return fPt; }
  Int_t  GetMomentum  (TLorentzVector* Momentum);
  
  KalRep*   GetKalRep() { return fKalRep[0]; }
  
  float  Phi0 () { return fFloat[0];}
  
  void   SetNumber(int I ) { fNumber = I; }

  void   SetAlgorithmID (int ID) { fAlgorithmID = ID; }
//-----------------------------------------------------------------------------
// overloaded methods of TObject
//-----------------------------------------------------------------------------
  virtual void Clear (Option_t* Opt = "") ;
  virtual void Print (Option_t* Opt = "") const ;

//-----------------------------------------------------------------------------
// schema evolution
//-----------------------------------------------------------------------------
  void ReadV4(TBuffer& R__b);
  void ReadV5(TBuffer& R__b);
  void ReadV6(TBuffer& R__b);

  ClassDef(TStnTrack,7)

};

#endif

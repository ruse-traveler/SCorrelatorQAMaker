// ----------------------------------------------------------------------------
// 'SCheckTrackPairsConfig.h'
// Derek Anderson
// 02.21.2024
//
// SCorrelatorQAMaker plugin to iterate through
// all pairs of tracks in an event and fill
// tuples/histograms comparing them.
// ----------------------------------------------------------------------------

// c++ utilities
#include <utility>
// root utilities
#include <TF1.h>
// analysis utilities
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/SCorrelatorUtilities.h"

// make common namespaces implicit
using namespace std;



namespace SColdQcdCorrelatorAnalysis {

  // SCheckTrackPairsConfig definition ----------------------------------------

  struct SCheckTrackPairsConfig {

    bool doDcaSigCut;
    bool requireSiSeed;
    bool useOnlyPrimVtx;

    // track acceptance
    pair<TrkInfo, TrkInfo> trkAccept;

    // for pt-dependent sigma cut
    pair<float, float> nSigCut;
    pair<float, float> ptFitMax;
    pair<TF1*,  TF1*>  fSigDca;

  };  // end SCheckTrackPairsConfig

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

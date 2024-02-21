// ----------------------------------------------------------------------------
// 'SCheckTrackPairs.h'
// Derek Anderson
// 11.14.2023
//
// SCorrelatorQAMaker plugin to iterate through
// all pairs of tracks in an event and fill
// tuples/histograms comparing them.
// ----------------------------------------------------------------------------

#ifndef SCHECKTRACKPAIRS_H
#define SCHECKTRACKPAIRS_H

// c++ utilities
#include <string>
#include <vector>
// root utilities
#include <TNtuple.h>
#include <Math/Vector3D.h>
// f4a libraries
#include <fun4all/SubsysReco.h>
// phool libraries
#include <phool/PHCompositeNode.h>
// analysis utilities
#include "SBaseQAPlugin.h"
#include "SCheckTrackPairsConfig.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/SCorrelatorUtilities.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SCheckTrackPairs definition ----------------------------------------------

  class SCheckTrackPairs : public SubsysReco, public SBaseQAPlugin<SCheckTrackPairsConfig> {

    public:

      // ctor/dtor
      SCheckTrackPairs(const string& name = "CheckTrackPairs") : SubsysReco(name) {};
      ~SCheckTrackPairs() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // internal methods
      void InitTuples();
      void SaveOutput();
      void ResetVectors();
      void DoDoubleTrackLoop(PHCompositeNode* topNode);
      bool IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode);

      // vector members
      vector<float>             m_vecTrackPairLeaves;
      vector<TrkrDefs::cluskey> m_vecClustKeysA;
      vector<TrkrDefs::cluskey> m_vecClustKeysB;

      // root members
      TNtuple* m_ntTrackPairs;

  };  // end SCheckTrackPairs

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 'SCheckTrackPairs.h'
// Derek Anderson
// 11.14.2023
//
// SCorrelatorQAMaker plugin to iterate through
// all pairs of tracks in an event and fill
// tuples/histograms comparing them.
// ----------------------------------------------------------------------------

#define SCHECKTRACKPAIRS_CC

// f4a libraries
#include <fun4all/Fun4AllReturnCodes.h>
// analysis utilities
#include "SCheckTrackPairs.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;



namespace SColdQcdCorrelatorAnalysis {

  // F4A methods  -------------------------------------------------------------

  int SCheckTrackPairs::Init(PHCompositeNode* topNode) {

    /* TODO put stuff here */
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositenNode*)'



  int SCheckTrackPairs::process_event(PHCompositeNode* topNode) {

    /* TODO put stuff here */
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'process_event(PHCompositeNode* topNode)'



  int SCheckTrackPairs::End(PHCompositeNode* topNode) {

    /* TODO put stuff here */
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'End(PHCompositeNode*)'

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

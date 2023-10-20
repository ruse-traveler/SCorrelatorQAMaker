// ----------------------------------------------------------------------------
// 'SCorrelatorQAMaker.cc'
// Derek Anderson
// 10.04.2023
//
// A module to produce TNtuples and histograms for QA
// plots relevant to the sPHENIX Cold QCD Energy-Energy
// Correlator analysis.
// ----------------------------------------------------------------------------

#define SCORRELATORQAMAKER_CC

// user includes
#include "SCorrelatorQAMaker.h"
#include "SCorrelatorQAMaker.sys.h"
#include "SCorrelatorQAMaker.ana.h"

using namespace std;
using namespace findNode;



namespace SColdQcdCorrelatorAnalysis {

  // F4A methods  -------------------------------------------------------------

  int SCorrelatorQAMaker::Init(PHCompositeNode* topNode) {
    InitTuples();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositenNode*)'



  int SCorrelatorQAMaker::process_event(PHCompositeNode* topNode) {
    return Fun4AllReturnCodes::EVENT_OK;
  }  // end 'process_event(PHCompositeNode* topNode)'



  int SCorrelatorQAMaker::End(PHCompositeNode* topNode) {
    DoSigmaDcaCalc();
    return Fun4AllReturnCodes::EVENT_OK;
  }  // end 'End(PHCompositeNode*)'

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

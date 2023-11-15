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
// f4a libraries
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
// phool libraries
#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHCompositeNode.h>
// tracking libraries
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/TrackAnalysisUtils.h>
// root utilities
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TNtuple.h>
// analysis utilities
#include "SBaseQAPlugin.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;



namespace SColdQcdCorrelatorAnalysis {

  // SCheckTrackPairsConfig definition ----------------------------------------

  struct SCheckTrackPairsConfig {

    float ptMin = 0.;

  };  // end SCheckTrackPairsConfig



  // SCheckTrackPairs definition ----------------------------------------------

  class SCheckTrackPairs : public SubsysReco, SBaseQAPlugin<SCheckTrackPairsConfig> {

    public:

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // internal methods
      void InitTuples();
      void SaveOutput();

      // root members
      TNtuple* m_ntTrackPairs;

  };  // end SCheckTrackPairs



  // SCheckTrackPairs public methods ------------------------------------------

  int SCheckTrackPairs::Init(PHCompositeNode* topNode) {

    InitTuples();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositenNode*)'



  int SCheckTrackPairs::process_event(PHCompositeNode* topNode) {

    /* TODO put stuff here */
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'process_event(PHCompositeNode* topNode)'



  int SCheckTrackPairs::End(PHCompositeNode* topNode) {

    SaveOutput();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'End(PHCompositeNode*)'



  // SCheckTrackPairs internal methods ----------------------------------------

  void SCheckTrackPairs::InitTuples() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SCheckTrackPairs::InitTuples(): initializing output tuple." << endl;
    }

    // track pair leaves
    const vector<string> vecTrkPairLeaves = {
      "trkid_a",
      "trkid_b",
      "pt_a",
      "pt_b",
      "eta_a",
      "eta_b",
      "phi_a",
      "phi_b",
      "ene_a",
      "ene_b",
      "dcaxy_a",
      "dcaxy_b",
      "dcaz_a",
      "dcaz_b",
      "vtxx_a",
      "vtxx_b",
      "vtxy_a",
      "vtxy_b",
      "vtxz_a",
      "vtxz_b",
      "quality_a",
      "quality_b",
      "deltapt_a",
      "deltapt_b",
      "nmvtxlayers_a",
      "nmvtxlayers_b",
      "ninttlayers_a",
      "ninttlayers_b",
      "ntpclayers_a",
      "ntpclayers_b",
      "nmvtxclusts_a",
      "nmvtxclusts_b",
      "ninttclusts_a",
      "ninttclusts_b",
      "ntpcclusts_a",
      "ntpcclusts_b",
      "nclustkey_a",
      "nclustkey_b",
      "nsameclustkey",
      "deltartrack"
    };

    // compress leaves into a list
    string argTrkPairLeaves("");
    for (size_t iLeaf = 0; iLeaf < vecTrkPairLeaves.size(); iLeaf++) {
      argTrkPairLeaves.append(vecTrkPairLeaves[iLeaf]);
      if ((iLeaf + 1) != vecTrkPairLeaves.size()) {
        argTrkPairLeaves.append(":");
      }
    }

    // create tuple and return
    m_ntTrackPairs = new TNtuple("ntTrackPairs", "Pairs of tracks",   argTrkPairLeaves.data());
    return;

  }  // end 'InitTuples()'



  void SCheckTrackPairs::SaveOutput() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SCheckTrackPairs::SaveOutput(): saving output." << endl;
    }

    m_outDir       -> cd();
    m_ntTrackPairs -> Write();
    return;

  }  // end 'SaveOutput()'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

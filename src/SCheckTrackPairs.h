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
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.TrkTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.Constants.h"

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
      void ResetVectors();
      void DoDoubleTrackLoop();

      // vector members
      vector<float>             vecTrackPairLeaves;
      vector<TrkrDefs::cluskey> vecClustKeysA;
      vector<TrkrDefs::cluskey> vecClustKeysB;

      // root members
      TNtuple* m_ntTrackPairs;

  };  // end SCheckTrackPairs



  // SCheckTrackPairs public methods ------------------------------------------

  int SCheckTrackPairs::Init(PHCompositeNode* topNode) {

    InitTuples();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositenNode*)'



  int SCheckTrackPairs::process_event(PHCompositeNode* topNode) {

    ResetVectors();
    DoDoubleTrackLoop(topNode);
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

    // list of track pair leaves
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
    vecTrackPairLeaves.reserve(vecTrkPairLeaves.size());
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



  void SCheckTrackPairs::ResetVectors() {

    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SColdQcdCorrelatorAnalysis::SCheckTrackPairs::ResetVectors(): resetting vectors." << endl;
    }

    // set leaf values to a default
    const size_t nLeaves = vecTrackPairLeaves.size()
    for (size_t iLeaf = 0; iLeaf < nLeaves; iLeaf++) {
      vecTrackPairLaves[iLeaf] = -999.;
    }

    // clear cluster keys
    vecClustKeysA.clear();
    vecClustKeysB.clear();
    return;

  }  // end 'ResetVectors()'



  void SCheckTrackPairs::DoDoubleTrackLoop(PHCompositeNode* topNode) {

    // loop over tracks
    unsigned int  iCst    = particles.size();
    unsigned int  nTrkTot = 0;
    unsigned int  nTrkAcc = 0;
    double        eTrkSum = 0.;
    SvtxTrack*    track   = NULL;
    SvtxTrack*    trackB  = NULL;
    SvtxTrackMap* mapTrks = GetTrackMap(topNode);
    for (SvtxTrackMap::Iter itTrkA = mapTrks -> begin(); itTrkA != mapTrks -> end(); ++itTrkA) {

      // grab track A and skip if bad
      trackA = itTrkA -> second;
      if (!trackA) continue;

      //const bool isGoodTrackA = IsGoodTrack(trackA, topNode);
      //if (!isGoodTrackA) continue;

      // grab track dca and vertex
      pair<double, double> trkDcaPairA = GetTrackDcaPair(trackA, topNode);
      CLHEP::Hep3Vector    trkVtxA     = GetTrackVertex(trackA, topNode);

      // grab track A kinematic info
      const double trkPhiA = trackA -> get_phi();
      const double trkEtaA = trackA -> get_eta();
      const double trkPtA  = trackA -> get_pt();
      const double trkPxA  = trackA -> get_px();
      const double trkPyA  = trackA -> get_py();
      const double trkPzA  = trackA -> get_pz();
      const double trkE A  = sqrt((trkPxA * trkPxA) + (trkPyA * trkPyA) + (trkPzA * trkPzA) + (0.140 * 0.140));  // FIXME move pion mass to constant in utilities namespace

      // grab track A quality info
      const double trkQualityA   = trackA -> get_quality();
      const double trkDeltaPtA   = GetTrackDeltaPt(trackA);
      const double trkDcaXYA     = trkDcaPairA.first;
      const double trkDcaZA      = trkDcaPairA.second;
      const int    trkNumTpcA    = GetNumLayer(trackA, SUBSYS::TPC);
      const int    trkNumInttA   = GetNumLayer(trackA, SUBSYS::INTT);
      const int    trkNumMvtxA   = GetNumLayer(trackA, SUBSYS::MVTX);
      const int    trkClustTpcA  = GetNumClust(trackA, SUBSYS::TPC);
      const int    trkClustInttA = GetNumClust(trackA, SUBSYS::INTT);
      const int    trkClustMvtxA = GetNumClust(trackA, SUBSYS::MVTX);
      const int    trkIDA        = trackA -> get_id();

      // loop over tracks again
      for (SvtxTrackMap::Iter itTrkB = mapTrks -> begin(); itTrkB != mapTrks -> end(); ++itTrkB) {

        // grab track B and skip if bad
        trackB = itTrkB -> second;
        if (!trackB) continue;

        //const bool isGoodTrackB = IsGoodTrack(trackB, topNode);
        //if (!isGoodTrackB) continue;

        // skip if same as track A
        const int trkIDB = trackB -> get_id();
        if (trkIDA == trkIDB) continue;

        // grab track B dca and vertex
        pair<double, double> trkDcaPairB = GetTrackDcaPair(trackB, topNode);
        CLHEP::Hep3Vector    trkVtxB     = GetTrackVertex(trackB, topNode);

        // grab track B kinematic info
        const double trkPhiB = trackB -> get_phi();
        const double trkEtaB = trackB -> get_eta();
        const double trkPtB  = trackB -> get_pt();
        const double trkPxB  = trackB -> get_px();
        const double trkPyB  = trackB -> get_py();
        const double trkPzB  = trackB -> get_pz();
        const double trkEB   = sqrt((trkPxB * trkPxB) + (trkPyB * trkPyB) + (trkPzB * trkPzB) + (0.140 * 0.140));

        // grab track B quality info
        const double trkQualityB   = trackB -> get_quality();
        const double trkDeltaPtB   = GetTrackDeltaPt(trackB);
        const double trkDcaXYB     = trkDcaPairB.first;
        const double trkDcaZB      = trkDcaPairB.second;
        const int    trkNumTpcB    = GetNumLayer(trackB, SUBSYS::TPC);
        const int    trkNumInttB   = GetNumLayer(trackB, SUBSYS::INTT);
        const int    trkNumMvtxB   = GetNumLayer(trackB, SUBSYS::MVTX);
        const int    trkClustTpcB  = GetNumClust(trackB, SUBSYS::TPC);
        const int    trkClustInttB = GetNumClust(trackB, SUBSYS::INTT);
        const int    trkClustMvtxB = GetNumClust(trackB, SUBSYS::MVTX);

        // calculate delta-R
        const double dfTrkAB = trkPhiA - trkPhiB;
        const double dhTrkAB = trkEtaA - trkEtaB;
        const double drTrkAB = sqrt((dfTrkAB * dfTrkAB) + (dhTrkAB * dhTrkAB));

        // clear vectors for checking cluster keys
        clustKeysA.clear();
        clustKeysB.clear();

        // loop over clusters from track A
        auto seedTpcA = trackA -> get_tpc_seed();
        if (seedTpcA) {
          for (auto local_iterA = seedTpcA -> begin_cluster_keys(); local_iterA != seedTpcA -> end_cluster_keys(); ++local_iterA) {
            TrkrDefs::cluskey cluster_keyA = *local_iterA;
            clustKeysA.push_back(cluster_keyA);
          }
        }

        // loop over clusters from track B
        auto seedTpcB = trackB -> get_tpc_seed();
        if (seedTpcB) {
          for (auto local_iterB = seedTpcB -> begin_cluster_keys(); local_iterB != seedTpcB -> end_cluster_keys(); ++local_iterB) {
            TrkrDefs::cluskey cluster_keyB = *local_iterB;
            clustKeysB.push_back(cluster_keyB);
          }
        }

        // calculate no. of same cluster keys
        uint64_t nSameKey = 0;
        for (auto keyA : clustKeysA) {
          for (auto keyB : clustKeysB) {
            if (keyA == keyB) {
              ++nSameKey;
              break;
            }
          }  // end cluster key B loop
        }  // end cluster key A loop
/*
        cout << "CHECK: nSameKey   = " << nSameKey          << "\n"
             << "       nClustKeyA = " << clustKeysA.size() << "\n"
             << "       nClustKeyB = " << clustKeysB.size()
             << endl;
*/

        // set tuple leaves
        vecTrackPairLeaves[0]  = (float) trkIDA;
        vecTrackPairLeaves[1]  = (float) trkIDB;
        vecTrackPairLeaves[2]  = (float) trkPtA;
        vecTrackPairLeaves[3]  = (float) trkPtB;
        vecTrackPairLeaves[4]  = (float) trkEtaA;
        vecTrackPairLeaves[5]  = (float) trkEtaB;
        vecTrackPairLeaves[6]  = (float) trkPhiA;
        vecTrackPairLeaves[7]  = (float) trkPhiB;
        vecTrackPairLeaves[8]  = (float) trkEA;
        vecTrackPairLeaves[9]  = (float) trkEB;
        vecTrackPairLeaves[10] = (float) trkDcaXYA;
        vecTrackPairLeaves[11] = (float) trkDcaXYB;
        vecTrackPairLeaves[12] = (float) trkDcaZA;
        vecTrackPairLeaves[13] = (float) trkDcaZB;
        vecTrackPairLeaves[14] = (float) trkVtxA.x();
        vecTrackPairLeaves[15] = (float) trkVtxB.x();
        vecTrackPairLeaves[16] = (float) trkVtxA.y();
        vecTrackPairLeaves[17] = (float) trkVtxB.y();
        vecTrackPairLeaves[18] = (float) trkVtxA.z();
        vecTrackPairLeaves[19] = (float) trkVtxB.z();
        vecTrackPairLeaves[20] = (float) trkQualityA;
        vecTrackPairLeaves[21] = (float) trkQualityB;
        vecTrackPairLeaves[22] = (float) trkDeltaPtA;
        vecTrackPairLeaves[23] = (float) trkDeltaPtB;
        vecTrackPairLeaves[24] = (float) trkNumMvtxA;
        vecTrackPairLeaves[25] = (float) trkNumMvtxB;
        vecTrackPairLeaves[26] = (float) trkNumInttA;
        vecTrackPairLeaves[27] = (float) trkNumInttB;
        vecTrackPairLeaves[28] = (float) trkNumTpcA;
        vecTrackPairLeaves[29] = (float) trkNumTpcB;
        vecTrackPairLeaves[30] = (float) trkClustMvtxA;
        vecTrackPairLeaves[31] = (float) trkClustMvtxB;
        vecTrackPairLeaves[32] = (float) trkClustInttA;
        vecTrackPairLeaves[33] = (float) trkClustInttB;
        vecTrackPairLeaves[34] = (float) trkClustTpcA;
        vecTrackPairLeaves[35] = (float) trkClustTpcB;
        vecTrackPairLeaves[36] = (float) clustKeysA.size();
        vecTrackPairLeaves[37] = (float) clustKeysB.size();
        vecTrackPairLeaves[38] = (float) nSameKey;
        vecTrackPairLeaves[39] = (float) drTrkAB;

        // fill track pair tuple
        m_ntWeirdTracks -> Fill(vecTrackPairLeaves.data());

      }  // end 2nd track loop
    }  // end track loop
    return;

  }  // end 'DoDoubleTrackLoop(PHCompositeNode*)'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

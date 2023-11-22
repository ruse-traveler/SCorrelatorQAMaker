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
#include <TF1.h>
#include <TNtuple.h>
#include <Math/Vector3D.h>
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
// analysis utilities
#include "SBaseQAPlugin.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.TrkTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.Constants.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SCheckTrackPairsConfig definition ----------------------------------------

  struct SCheckTrackPairsConfig {

    bool    doDcaSigCut;
    bool    requireSiSeed;
    bool    useOnlyPrimVtx;
    TrkInfo minAccept;
    TrkInfo maxAccept;

    // for pt-dependent sigma cut
    pair<float, float> nSigCut;
    pair<float, float> ptFitMax;
    pair<TF1*,  TF1*>  fSigDca;

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
      void DoDoubleTrackLoop(PHCompositeNode* topNode);
      bool IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode);

      // vector members
      vector<float>             m_vecTrackPairLeaves;
      vector<TrkrDefs::cluskey> m_vecClustKeysA;
      vector<TrkrDefs::cluskey> m_vecClustKeysB;

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
    m_vecTrackPairLeaves.reserve(vecTrkPairLeaves.size());
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
    const size_t nLeaves = m_vecTrackPairLeaves.size();
    for (size_t iLeaf = 0; iLeaf < nLeaves; iLeaf++) {
      m_vecTrackPairLeaves[iLeaf] = -999.;
    }

    // clear cluster keys
    m_vecClustKeysA.clear();
    m_vecClustKeysB.clear();
    return;

  }  // end 'ResetVectors()'



  void SCheckTrackPairs::DoDoubleTrackLoop(PHCompositeNode* topNode) {

    // loop over tracks
    SvtxTrack*    trackA  = NULL;
    SvtxTrack*    trackB  = NULL;
    SvtxTrackMap* mapTrks = GetTrackMap(topNode);
    for (SvtxTrackMap::Iter itTrkA = mapTrks -> begin(); itTrkA != mapTrks -> end(); ++itTrkA) {

      // grab track A and skip if bad
      trackA = itTrkA -> second;
      if (!trackA) continue;

      const bool isGoodTrackA = IsGoodTrack(trackA, topNode);
      if (!isGoodTrackA) continue;

      // grab track info
      TrkInfo trkInfoA(trackA, topNode);

      // loop over tracks again
      for (SvtxTrackMap::Iter itTrkB = mapTrks -> begin(); itTrkB != mapTrks -> end(); ++itTrkB) {

        // grab track B and skip if bad
        trackB = itTrkB -> second;
        if (!trackB) continue;

        const bool isGoodTrackB = IsGoodTrack(trackB, topNode);
        if (!isGoodTrackB) continue;

        // skip if same as track A
        if ((trackA -> get_id()) == (trackB -> get_id())) continue;

        // grab track info
        TrkInfo trkInfoB(trackB, topNode);

        // calculate delta-R
        const double dfTrkAB = trkInfoA.phi - trkInfoB.phi;
        const double dhTrkAB = trkInfoA.eta - trkInfoB.eta;
        const double drTrkAB = sqrt((dfTrkAB * dfTrkAB) + (dhTrkAB * dhTrkAB));

        // clear vectors for checking cluster keys
        m_vecClustKeysA.clear();
        m_vecClustKeysB.clear();

        // loop over clusters from track A
        auto seedTpcA = trackA -> get_tpc_seed();
        if (seedTpcA) {
          for (auto local_iterA = seedTpcA -> begin_cluster_keys(); local_iterA != seedTpcA -> end_cluster_keys(); ++local_iterA) {
            TrkrDefs::cluskey cluster_keyA = *local_iterA;
            m_vecClustKeysA.push_back(cluster_keyA);
          }
        }

        // loop over clusters from track B
        auto seedTpcB = trackB -> get_tpc_seed();
        if (seedTpcB) {
          for (auto local_iterB = seedTpcB -> begin_cluster_keys(); local_iterB != seedTpcB -> end_cluster_keys(); ++local_iterB) {
            TrkrDefs::cluskey cluster_keyB = *local_iterB;
            m_vecClustKeysB.push_back(cluster_keyB);
          }
        }

        // calculate no. of same cluster keys
        uint64_t nSameKey = 0;
        for (auto keyA : m_vecClustKeysA) {
          for (auto keyB : m_vecClustKeysB) {
            if (keyA == keyB) {
              ++nSameKey;
              break;
            }
          }  // end cluster key B loop
        }  // end cluster key A loop
/*
        cout << "CHECK: nSameKey   = " << nSameKey          << "\n"
             << "       nClustKeyA = " << m_vecClustKeysA.size() << "\n"
             << "       nClustKeyB = " << m_vecClustKeysB.size()
             << endl;
*/

        // set tuple leaves
        m_vecTrackPairLeaves[0]  = (float) trkInfoA.id;
        m_vecTrackPairLeaves[1]  = (float) trkInfoB.id;
        m_vecTrackPairLeaves[2]  = (float) trkInfoA.pt;
        m_vecTrackPairLeaves[3]  = (float) trkInfoB.pt;
        m_vecTrackPairLeaves[4]  = (float) trkInfoA.eta;
        m_vecTrackPairLeaves[5]  = (float) trkInfoB.eta;
        m_vecTrackPairLeaves[6]  = (float) trkInfoA.phi;
        m_vecTrackPairLeaves[7]  = (float) trkInfoB.phi;
        m_vecTrackPairLeaves[8]  = (float) trkInfoA.ene;
        m_vecTrackPairLeaves[9]  = (float) trkInfoB.ene;
        m_vecTrackPairLeaves[10] = (float) trkInfoA.dcaXY;
        m_vecTrackPairLeaves[11] = (float) trkInfoB.dcaXY;
        m_vecTrackPairLeaves[12] = (float) trkInfoA.dcaZ;
        m_vecTrackPairLeaves[13] = (float) trkInfoB.dcaZ;
        m_vecTrackPairLeaves[14] = (float) trkInfoA.vtxX;
        m_vecTrackPairLeaves[15] = (float) trkInfoB.vtxX;
        m_vecTrackPairLeaves[16] = (float) trkInfoA.vtxY;
        m_vecTrackPairLeaves[17] = (float) trkInfoB.vtxY;
        m_vecTrackPairLeaves[18] = (float) trkInfoA.vtxZ;
        m_vecTrackPairLeaves[19] = (float) trkInfoB.vtxZ;
        m_vecTrackPairLeaves[20] = (float) trkInfoA.quality;
        m_vecTrackPairLeaves[21] = (float) trkInfoB.quality;
        m_vecTrackPairLeaves[22] = (float) trkInfoA.ptErr;
        m_vecTrackPairLeaves[23] = (float) trkInfoB.ptErr;
        m_vecTrackPairLeaves[24] = (float) trkInfoA.nMvtxLayer;
        m_vecTrackPairLeaves[25] = (float) trkInfoB.nMvtxLayer;
        m_vecTrackPairLeaves[26] = (float) trkInfoA.nInttLayer;
        m_vecTrackPairLeaves[27] = (float) trkInfoB.nInttLayer;
        m_vecTrackPairLeaves[28] = (float) trkInfoA.nTpcLayer;
        m_vecTrackPairLeaves[29] = (float) trkInfoB.nTpcLayer;
        m_vecTrackPairLeaves[30] = (float) trkInfoA.nMvtxClust;
        m_vecTrackPairLeaves[31] = (float) trkInfoB.nMvtxClust;
        m_vecTrackPairLeaves[32] = (float) trkInfoA.nInttClust;
        m_vecTrackPairLeaves[33] = (float) trkInfoB.nInttClust;
        m_vecTrackPairLeaves[34] = (float) trkInfoA.nTpcClust;
        m_vecTrackPairLeaves[35] = (float) trkInfoB.nTpcClust;
        m_vecTrackPairLeaves[36] = (float) m_vecClustKeysA.size();
        m_vecTrackPairLeaves[37] = (float) m_vecClustKeysB.size();
        m_vecTrackPairLeaves[38] = (float) nSameKey;
        m_vecTrackPairLeaves[39] = (float) drTrkAB;

        // fill track pair tuple
        m_ntTrackPairs -> Fill(m_vecTrackPairLeaves.data());

      }  // end 2nd track loop
    }  // end track loop
    return;

  }  // end 'DoDoubleTrackLoop(PHCompositeNode*)'



  bool SCheckTrackPairs::IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode) {

    // print debug statement
    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SCheckTrackPairs::IsGoodTrack(SvtxTrack*, PHCompositeNode*) Checking if track is good..." << endl;
    }

    // grab track info
    TrkInfo info(track, topNode);

    // if needed, check if dca is in pt-dependent range
    bool isInDcaSigma = true;
    if (m_config.doDcaSigCut) {
      isInDcaSigma = IsInSigmaDcaCut(info, m_config.nSigCut, m_config.ptFitMax, m_config.fSigDca);
    }

    // if needed, check if track is from primary vertex
    const bool isFromPrimVtx = m_config.useOnlyPrimVtx ? IsFromPrimaryVtx(track, topNode) : true;

    // check if seed is good & track is in acceptance
    const bool isSeedGood = IsGoodTrackSeed(track, m_config.requireSiSeed);
    const bool isInAccept = IsInTrackAcceptance(info, m_config.minAccept, m_config.maxAccept);

    // return overall goodness of track
    return (isFromPrimVtx && isInDcaSigma && isSeedGood && isInAccept);

  }  // end 'IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode)'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

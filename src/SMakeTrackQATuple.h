// ----------------------------------------------------------------------------
// 'SMakeTrackQATuple.h'
// Derek Anderson
// 12.29.2023
//
// SCorrelatorQAMaker plugin to produce QA tuples
// for tracks.
// ----------------------------------------------------------------------------

#ifndef SMAKETRACKQATUPLE_H
#define SMAKETRACKQATUPLE_H

// c++ utilities
#include <string>
#include <vector>
#include <utility>
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
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/GenTools.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/EvtTools.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/TrkTools.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/Constants.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/Interfaces.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SMakeTrackQATupleConfig definition ----------------------------------------

  struct SMakeTrackQATupleConfig {

    bool isEmbed;
    bool doDcaSigCut;
    bool requireSiSeed;
    bool useOnlyPrimVtx;

    // track acceptance
    pair<TrkInfo, TrkInfo> trkAccept;

    // for pt-dependent sigma cut
    pair<float, float> nSigCut;
    pair<float, float> ptFitMax;
    pair<TF1*,  TF1*>  fSigDca;

  };  // end SMakeTrackQATupleConfig



  // SMakeTrackQATuple definition ----------------------------------------------

  class SMakeTrackQATuple : public SubsysReco, public SBaseQAPlugin<SMakeTrackQATupleConfig> {

    public:

      // ctor/dtor
      SMakeTrackQATuple(const string& name = "TrackQATuple") : SubsysReco(name) {};
      ~SMakeTrackQATuple() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // internal methods
      void InitTuple();
      void SaveOutput();
      void DoTrackLoop(PHCompositeNode* topNode);
      bool IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode);

      // for tuple leaves
      vector<float> m_vecTrackLeaves;

      // root members
      TNtuple* m_ntTrackQA;

  };  // end SMakeTrackQATuple



  // SMakeTrackQATuple public methods ------------------------------------------

  int SMakeTrackQATuple::Init(PHCompositeNode* topNode) {

    InitOutput();
    InitTuple();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositeNode*)'



  int SMakeTrackQATuple::process_event(PHCompositeNode* topNode) {

    DoTrackLoop(topNode);
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'process_event(PHCompositeNode* topNode)'



  int SMakeTrackQATuple::End(PHCompositeNode* topNode) {

    SaveOutput();
    CloseOutput();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'End(PHCompositeNode*)'



  // SMakeTrackQATuple internal methods -----------------------------------------

  void SMakeTrackQATuple::InitTuple() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrackQATuple::InitTuple(): initializing output tuple." << endl;
    }

    // create leaf list for event and track info
    vector<string> vecTrkLeaves = MakeLeafVector<RecoInfo>();
    AddLeavesToVector<GenInfo>(vecTrkLeaves);
    AddLeavesToVector<TrkInfo>(vecTrkLeaves);

    // compress leaves into a color-separated list
    string argTrkLeaves = FlattenLeafList(vecTrkLeaves);

    // create tuple and return
    m_ntTrackQA = new TNtuple("ntTrackQA", "Track QA", argTrkLeaves.data());
    m_vecTrackLeaves.reserve(vecTrkLeaves.size());
    return;

  }  // end 'InitTuple()'



  void SMakeTrackQATuple::SaveOutput() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrackQATuple::SaveOutput(): saving output." << endl;
    }

    m_outDir    -> cd();
    m_ntTrackQA -> Write();
    return;

  }  // end 'SaveOutput()'



  void SMakeTrackQATuple::DoTrackLoop(PHCompositeNode* topNode) {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrackQATuple::DoTrackLoop(PHCompositeNode*): looping over tracks." << endl;
    }

    // grab event info
    //   FIXME add in subevent selection
    RecoInfo recInfo(topNode);
    GenInfo  genInfo(topNode, m_config.isEmbed, {2});

    // loop over tracks
    SvtxTrack*    track  = NULL;
    SvtxTrackMap* mapTrks = GetTrackMap(topNode);
    for (SvtxTrackMap::Iter itTrk = mapTrks -> begin(); itTrk != mapTrks -> end(); ++itTrk) {


      // grab track and skip if bad
      track = itTrk -> second;
      if (!track) continue;

      const bool isGoodTrack = IsGoodTrack(track, topNode);
      if (!isGoodTrack) continue;

      // grab track info
      TrkInfo trkInfo(track, topNode);

      // set tuple leaves
      m_vecTrackLeaves[0]  = (float) recInfo.nTrks;
      m_vecTrackLeaves[1]  = (float) recInfo.pSumTrks;
      m_vecTrackLeaves[2]  = (float) recInfo.eSumEMCal;
      m_vecTrackLeaves[3]  = (float) recInfo.eSumIHCal;
      m_vecTrackLeaves[4]  = (float) recInfo.eSumOHCal;
      m_vecTrackLeaves[5]  = (float) recInfo.vx;
      m_vecTrackLeaves[6]  = (float) recInfo.vy;
      m_vecTrackLeaves[7]  = (float) recInfo.vz;
      m_vecTrackLeaves[8]  = (float) genInfo.nChrgPar;
      m_vecTrackLeaves[9]  = (float) genInfo.nNeuPar;
      m_vecTrackLeaves[10] = (float) genInfo.isEmbed;
      m_vecTrackLeaves[11] = (float) genInfo.eSumChrg;
      m_vecTrackLeaves[12] = (float) genInfo.eSumNeu;
      m_vecTrackLeaves[13] = (float) genInfo.partons.first.pid;
      m_vecTrackLeaves[14] = (float) genInfo.partons.first.status;
      m_vecTrackLeaves[15] = (float) genInfo.partons.first.barcode;
      m_vecTrackLeaves[16] = (float) genInfo.partons.first.embedID;
      m_vecTrackLeaves[17] = (float) genInfo.partons.first.charge;
      m_vecTrackLeaves[18] = (float) genInfo.partons.first.mass;
      m_vecTrackLeaves[19] = (float) genInfo.partons.first.eta;
      m_vecTrackLeaves[20] = (float) genInfo.partons.first.phi;
      m_vecTrackLeaves[21] = (float) genInfo.partons.first.ene;
      m_vecTrackLeaves[22] = (float) genInfo.partons.first.px;
      m_vecTrackLeaves[23] = (float) genInfo.partons.first.py;
      m_vecTrackLeaves[24] = (float) genInfo.partons.first.pz;
      m_vecTrackLeaves[25] = (float) genInfo.partons.first.pt;
      m_vecTrackLeaves[26] = (float) genInfo.partons.first.vx;
      m_vecTrackLeaves[27] = (float) genInfo.partons.first.vy;
      m_vecTrackLeaves[28] = (float) genInfo.partons.first.vz;
      m_vecTrackLeaves[29] = (float) genInfo.partons.second.pid;
      m_vecTrackLeaves[30] = (float) genInfo.partons.second.status;
      m_vecTrackLeaves[31] = (float) genInfo.partons.second.barcode;
      m_vecTrackLeaves[32] = (float) genInfo.partons.second.embedID;
      m_vecTrackLeaves[33] = (float) genInfo.partons.second.charge;
      m_vecTrackLeaves[34] = (float) genInfo.partons.second.mass;
      m_vecTrackLeaves[35] = (float) genInfo.partons.second.eta;
      m_vecTrackLeaves[36] = (float) genInfo.partons.second.phi;
      m_vecTrackLeaves[37] = (float) genInfo.partons.second.ene;
      m_vecTrackLeaves[38] = (float) genInfo.partons.second.px;
      m_vecTrackLeaves[39] = (float) genInfo.partons.second.py;
      m_vecTrackLeaves[40] = (float) genInfo.partons.second.pz;
      m_vecTrackLeaves[41] = (float) genInfo.partons.second.pt;
      m_vecTrackLeaves[42] = (float) genInfo.partons.second.vx;
      m_vecTrackLeaves[43] = (float) genInfo.partons.second.vy;
      m_vecTrackLeaves[44] = (float) genInfo.partons.second.vz;
      m_vecTrackLeaves[45] = (float) trkInfo.id;
      m_vecTrackLeaves[46] = (float) trkInfo.nMvtxLayer;
      m_vecTrackLeaves[47] = (float) trkInfo.nInttLayer;
      m_vecTrackLeaves[48] = (float) trkInfo.nTpcLayer;
      m_vecTrackLeaves[49] = (float) trkInfo.nMvtxClust;
      m_vecTrackLeaves[50] = (float) trkInfo.nInttClust;
      m_vecTrackLeaves[51] = (float) trkInfo.nTpcClust;
      m_vecTrackLeaves[52] = (float) trkInfo.eta;
      m_vecTrackLeaves[53] = (float) trkInfo.phi;
      m_vecTrackLeaves[54] = (float) trkInfo.px;
      m_vecTrackLeaves[55] = (float) trkInfo.py;
      m_vecTrackLeaves[56] = (float) trkInfo.pz;
      m_vecTrackLeaves[57] = (float) trkInfo.pt;
      m_vecTrackLeaves[58] = (float) trkInfo.ene;
      m_vecTrackLeaves[59] = (float) trkInfo.dcaXY;
      m_vecTrackLeaves[60] = (float) trkInfo.dcaZ;
      m_vecTrackLeaves[61] = (float) trkInfo.ptErr;
      m_vecTrackLeaves[62] = (float) trkInfo.quality;
      m_vecTrackLeaves[63] = (float) trkInfo.vtxX;
      m_vecTrackLeaves[64] = (float) trkInfo.vtxY;
      m_vecTrackLeaves[65] = (float) trkInfo.vtxZ;

      // fill track tuple
      m_ntTrackQA -> Fill(m_vecTrackLeaves.data());

    }  // end track loop
    return;

  }  // end 'DoTrackLoop(PHCompositeNode*)'



  bool SMakeTrackQATuple::IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode) {

    // print debug statement
    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SMakeTrackQATuple::IsGoodTrack(SvtxTrack*, PHCompositeNode*) Checking if track is good..." << endl;
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
    const bool isInAccept = IsInAcceptance(info, m_config.trkAccept.first, m_config.trkAccept.second);

    // return overall goodness of track
    return (isFromPrimVtx && isInDcaSigma && isSeedGood && isInAccept);

  }  // end 'IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode)'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end -----------------------------------------------------------------------

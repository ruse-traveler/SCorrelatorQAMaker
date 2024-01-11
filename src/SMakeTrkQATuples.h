// ----------------------------------------------------------------------------
// 'SMakeTrkQATuples.h'
// Derek Anderson
// 12.29.2023
//
// SCorrelatorQAMaker plugin to produce QA tuples
// for tracks, calo clusters, and particle flow
// objects.
// ----------------------------------------------------------------------------

#ifndef SMAKETRKQATUPLES_H
#define SMAKETRKQATUPLES_H

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
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/GenTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/EvtTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/TrkTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/Constants.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/Interfaces.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SMakeTrkQATuplesConfig definition ----------------------------------------

  struct SMakeTrkQATuplesConfig {

    bool    isSimEvt;
    bool    isEmbed;
    bool    doDcaSigCut;
    bool    requireSiSeed;
    bool    useOnlyPrimVtx;
    TrkInfo minAccept;
    TrkInfo maxAccept;

    // for pt-dependent sigma cut
    pair<float, float> nSigCut;
    pair<float, float> ptFitMax;
    pair<TF1*,  TF1*>  fSigDca;

  };  // end SMakeTrkQATuplesConfig



  // SMakeTrkQATuples definition ----------------------------------------------
  class SMakeTrkQATuples : public SubsysReco, public SBaseQAPlugin<SMakeTrkQATuplesConfig> {

    public:

      // ctor/dtor
      SMakeTrkQATuples(const string& name = "TrackQATuples") : SubsysReco(name) {};
      ~SMakeTrkQATuples() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // internal methods
      void InitTuples();
      void SaveOutput();
      void DoTrackLoop(PHCompositeNode* topNode);
      bool IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode);

      // for tuple leaves
      vector<float> m_vecTrackLeaves;

      // root members
      TNtuple* m_ntTrackQA;

  };  // end SCheckTrackPairs



  // SMakeTrkQATuples public methods ------------------------------------------

  int SMakeTrkQATuples::Init(PHCompositeNode* topNode) {

    InitOutput();
    InitTuples();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositeNode*)'



  int SMakeTrkQATuples::process_event(PHCompositeNode* topNode) {

    DoTrackLoop(topNode);
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'process_event(PHCompositeNode* topNode)'



  int SMakeTrkQATuples::End(PHCompositeNode* topNode) {

    SaveOutput();
    CloseOutput();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'End(PHCompositeNode*)'



  // SMakeTrkQATuples internal methods -----------------------------------------

  void SMakeTrkQATuples::InitTuples() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrkQATuples::InitTuples(): initializing output tuple." << endl;
    }

    // create leaf list for event and track info
    vector<string> vecTrkLeaves = MakeLeafVector<RecoInfo>();
    AddLeavesToVector<GenInfo>(vecTrkLeaves);
    AddLeavesToVector<TrkInfo>(vecTrkLeaves);

    cout << "CHECK: num leaves = " << vecTrkLeaves.size() << endl;

    // compress leaves into a color-separated list
    string argTrkLeaves = FlattenLeafList(vecTrkLeaves);

    cout << "CHECK list = " << argTrkLeaves << endl;

    // create tuple and return
    m_ntTrackQA = new TNtuple("ntTrackQA", "Track QA", argTrkLeaves.data());
    m_vecTrackLeaves.reserve(vecTrkLeaves.size());
    return;

  }  // end 'InitTuples()'



  void SMakeTrkQATuples::SaveOutput() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrkQATuples::SaveOutput(): saving output." << endl;
    }

    m_outDir    -> cd();
    m_ntTrackQA -> Write();
    return;

  }  // end 'SaveOutput()'



  void SMakeTrkQATuples::DoTrackLoop(PHCompositeNode* topNode) {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeTrkQATuples::DoTrackLoop(): looping over tracks." << endl;
    }

    // grab event info
    EvtInfo evtInfo(topNode, m_config.isSimEvt, m_config.isEmbed, {1});

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
      m_vecTrackLeaves[0]  = (float) evtInfo.reco.nTrks;
      m_vecTrackLeaves[1]  = (float) evtInfo.reco.pSumTrks;
      m_vecTrackLeaves[2]  = (float) evtInfo.reco.eSumEMCal;
      m_vecTrackLeaves[3]  = (float) evtInfo.reco.eSumIHCal;
      m_vecTrackLeaves[4]  = (float) evtInfo.reco.eSumOHCal;
      m_vecTrackLeaves[5]  = (float) evtInfo.reco.vx;
      m_vecTrackLeaves[6]  = (float) evtInfo.reco.vy;
      m_vecTrackLeaves[7]  = (float) evtInfo.reco.vz;
      m_vecTrackLeaves[8]  = (float) evtInfo.gen.nChrgPar;
      m_vecTrackLeaves[9]  = (float) evtInfo.gen.nNeuPar;
      m_vecTrackLeaves[10] = (float) evtInfo.gen.isEmbed;
      m_vecTrackLeaves[11] = (float) evtInfo.gen.eSumChrg;
      m_vecTrackLeaves[12] = (float) evtInfo.gen.eSumNeu;
      m_vecTrackLeaves[13] = (float) evtInfo.gen.partons.first.pid;
      m_vecTrackLeaves[14] = (float) evtInfo.gen.partons.first.status;
      m_vecTrackLeaves[15] = (float) evtInfo.gen.partons.first.embedID;
      m_vecTrackLeaves[16] = (float) evtInfo.gen.partons.first.charge;
      m_vecTrackLeaves[17] = (float) evtInfo.gen.partons.first.mass;
      m_vecTrackLeaves[18] = (float) evtInfo.gen.partons.first.eta;
      m_vecTrackLeaves[19] = (float) evtInfo.gen.partons.first.phi;
      m_vecTrackLeaves[20] = (float) evtInfo.gen.partons.first.ene;
      m_vecTrackLeaves[21] = (float) evtInfo.gen.partons.first.px;
      m_vecTrackLeaves[22] = (float) evtInfo.gen.partons.first.py;
      m_vecTrackLeaves[23] = (float) evtInfo.gen.partons.first.pz;
      m_vecTrackLeaves[24] = (float) evtInfo.gen.partons.first.pt;
      m_vecTrackLeaves[25] = (float) evtInfo.gen.partons.first.vx;
      m_vecTrackLeaves[26] = (float) evtInfo.gen.partons.first.vy;
      m_vecTrackLeaves[27] = (float) evtInfo.gen.partons.first.vz;
      m_vecTrackLeaves[28] = (float) evtInfo.gen.partons.second.pid;
      m_vecTrackLeaves[29] = (float) evtInfo.gen.partons.second.status;
      m_vecTrackLeaves[30] = (float) evtInfo.gen.partons.second.embedID;
      m_vecTrackLeaves[31] = (float) evtInfo.gen.partons.second.charge;
      m_vecTrackLeaves[32] = (float) evtInfo.gen.partons.second.mass;
      m_vecTrackLeaves[33] = (float) evtInfo.gen.partons.second.eta;
      m_vecTrackLeaves[34] = (float) evtInfo.gen.partons.second.phi;
      m_vecTrackLeaves[35] = (float) evtInfo.gen.partons.second.ene;
      m_vecTrackLeaves[36] = (float) evtInfo.gen.partons.second.px;
      m_vecTrackLeaves[37] = (float) evtInfo.gen.partons.second.py;
      m_vecTrackLeaves[38] = (float) evtInfo.gen.partons.second.pz;
      m_vecTrackLeaves[39] = (float) evtInfo.gen.partons.second.pt;
      m_vecTrackLeaves[40] = (float) evtInfo.gen.partons.second.vx;
      m_vecTrackLeaves[41] = (float) evtInfo.gen.partons.second.vy;
      m_vecTrackLeaves[42] = (float) evtInfo.gen.partons.second.vz;
      m_vecTrackLeaves[43] = (float) trkInfo.id;
      m_vecTrackLeaves[44] = (float) trkInfo.nMvtxLayer;
      m_vecTrackLeaves[45] = (float) trkInfo.nInttLayer;
      m_vecTrackLeaves[46] = (float) trkInfo.nTpcLayer;
      m_vecTrackLeaves[47] = (float) trkInfo.nMvtxClust;
      m_vecTrackLeaves[48] = (float) trkInfo.nInttClust;
      m_vecTrackLeaves[49] = (float) trkInfo.nTpcClust;
      m_vecTrackLeaves[50] = (float) trkInfo.eta;
      m_vecTrackLeaves[51] = (float) trkInfo.phi;
      m_vecTrackLeaves[52] = (float) trkInfo.px;
      m_vecTrackLeaves[53] = (float) trkInfo.py;
      m_vecTrackLeaves[54] = (float) trkInfo.pz;
      m_vecTrackLeaves[55] = (float) trkInfo.pt;
      m_vecTrackLeaves[56] = (float) trkInfo.ene;
      m_vecTrackLeaves[57] = (float) trkInfo.dcaXY;
      m_vecTrackLeaves[58] = (float) trkInfo.dcaZ;
      m_vecTrackLeaves[59] = (float) trkInfo.ptErr;
      m_vecTrackLeaves[60] = (float) trkInfo.quality;
      m_vecTrackLeaves[61] = (float) trkInfo.vtxX;
      m_vecTrackLeaves[62] = (float) trkInfo.vtxY;
      m_vecTrackLeaves[63] = (float) trkInfo.vtxZ;

      // fill track tuple
      m_ntTrackQA -> Fill(m_vecTrackLeaves.data());

    }  // end track loop
    return;

  }  // end 'DoTrackLoop(PHCompositeNode*)'



  bool SMakeTrkQATuples::IsGoodTrack(SvtxTrack* track, PHCompositeNode* topNode) {

    // print debug statement
    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SMakeTrkQATuples::IsGoodTrack(SvtxTrack*, PHCompositeNode*) Checking if track is good..." << endl;
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

// end -----------------------------------------------------------------------

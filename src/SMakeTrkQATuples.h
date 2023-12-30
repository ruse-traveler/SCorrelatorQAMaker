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
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/EvtTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/TrkTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/Constants.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SMakeTrkQATuplesConfig definition ----------------------------------------

  struct SMakeTrkQATuplesConfig {

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

    //ResetVectors();
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

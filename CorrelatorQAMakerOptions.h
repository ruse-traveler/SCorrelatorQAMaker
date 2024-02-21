// ----------------------------------------------------------------------------
// 'CorrelatorQAMakerOptions.h'
// Derek Anderson
// 01.26.2024
//
// Options for the SCorrelatorQAMaker module(s).
// ----------------------------------------------------------------------------

#ifndef CORRELATORQAMAKEROPTIONS_H
#define CORRELATORQAMAKEROPTIONS_H

// c++ utilities
#include <string>
#include <utility>

// analysis utilities
#include "/sphenix/user/danderson/install/include/scorrelatorqamaker/SCheckTrackPairsConfig.h"
#include "/sphenix/user/danderson/install/include/scorrelatorqamaker/SMakeTrackQATuple.h"
#include "/sphenix/user/danderson/install/include/scorrelatorqamaker/SMakeClustQATree.h"
#include "/sphenix/user/danderson/install/include/scorrelatorqamaker/SReadLambdaJetTreeConfig.h"
#include "/sphenix/user/danderson/install/include/scorrelatorutilities/SCorrelatorUtilities.h"

// make common namespacs implicit
using namespace std;
using namespace SColdQcdCorrelatorAnalysis;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace CorrelatorQAMakerOptions {

  // track, cluster, etc. acceptances -----------------------------------------

  // track acceptance
  const pair<int, int>       nMvtxLayerTrkRange = {2, 100};
  const pair<int, int>       nInttLayerTrkRange = {1, 100};
  const pair<int, int>       nTpcLayerTrkRange  = {24, 100};
  const pair<double, double> ptTrkRange         = {0.2, 100.};
  const pair<double, double> etaTrkRange        = {-1.1, 1.1};
  const pair<double, double> dcaXYTrkRange      = {-5., 5.};
  const pair<double, double> dcaZTrkRange       = {-5., 5.};
  const pair<double, double> ptErrTrkRange      = {0., 0.5};
  const pair<double, double> qualityTrkRange    = {0., 10.};

  // cluster acceptance
  const pair<double, double> eneClustRange = {0.1, 100.};
  const pair<double, double> etaClustRange = {-1.1, 1.1};



  // set up acceptances -------------------------------------------------------

  // bundle track acceptance into a pair
  pair<TrkInfo, TrkInfo> GetTrackAccept() {

    pair<TrkInfo, TrkInfo> trkAccept;
    trkAccept.first.nMvtxLayer  = nMvtxLayerTrkRange.first;
    trkAccept.first.nInttLayer  = nInttLayerTrkRange.first;
    trkAccept.first.nTpcLayer   = nTpcLayerTrkRange.first;
    trkAccept.first.pt          = ptTrkRange.first;
    trkAccept.first.eta         = etaTrkRange.first;
    trkAccept.first.dcaXY       = dcaXYTrkRange.first;
    trkAccept.first.dcaZ        = dcaZTrkRange.first;
    trkAccept.first.ptErr       = ptErrTrkRange.first;
    trkAccept.first.quality     = qualityTrkRange.first;
    trkAccept.second.nMvtxLayer = nMvtxLayerTrkRange.second;
    trkAccept.second.nInttLayer = nInttLayerTrkRange.second;
    trkAccept.second.nTpcLayer  = nTpcLayerTrkRange.second;
    trkAccept.second.pt         = ptTrkRange.second;
    trkAccept.second.eta        = etaTrkRange.second;
    trkAccept.second.dcaXY      = dcaXYTrkRange.second;
    trkAccept.second.dcaZ       = dcaZTrkRange.second;
    trkAccept.second.ptErr      = ptErrTrkRange.second;
    trkAccept.second.quality    = qualityTrkRange.second;
    return trkAccept;

  }  // end 'GetTrackAcceptance()'



  // bundle cluster acceptance into a pair
  pair<ClustInfo, ClustInfo> GetClustAccept() {

    pair<ClustInfo, ClustInfo> clustAccept;
    clustAccept.first.ene  = eneClustRange.first;
    clustAccept.first.eta  = etaClustRange.first;
    clustAccept.second.ene = eneClustRange.second;
    clustAccept.second.eta = etaClustRange.second;
    return clustAccept;

  }  // end 'GetClustAccept()'



  // set up configurations ----------------------------------------------------

  // SCheckTrackPairs configuration
  SCheckTrackPairsConfig GetCheckTrackPairsConfig() {

    SCheckTrackPairsConfig config = {
      .doDcaSigCut    = false,
      .requireSiSeed  = true,
      .useOnlyPrimVtx = true,
      .trkAccept      = GetTrackAccept()
    };
    return config;

  }  // end 'GetCheckTrackPairsConfig()'



  // SMakeTrackQATuple configuration
  SMakeTrackQATupleConfig GetMakeTrackQATupleConfig() {

    SMakeTrackQATupleConfig config = {
      .isEmbed        = true,
      .doDcaSigCut    = false,
      .requireSiSeed  = true,
      .useOnlyPrimVtx = true,
      .trkAccept      = GetTrackAccept()
    };
    return config;

  }  // end 'GetMakeTrackQATupleConfig()'



  // SMakeClustQATree configuration
  SMakeClustQATreeConfig GetMakeClustQATreeConfig() {

    SMakeClustQATreeConfig config = {
      .isEmbed     = true,
      .clustAccept = GetClustAccept()
    };
    return config;

  }  // end 'GetMakeClustQATreeConfig()'



  // SReadLambdaJetTree configuration
  SReadLambdaJetTreeConfig GetReadLambdaJetTreeConfig(const string input) {

    SReadLambdaJetTreeConfig config = {
      .inFileName = input,
      .inTreeName = "LambdaJetTree",
      .ptJetMin   = 10.,
      .ptLamMin   = 0.2,
      .etaJetMax  = 0.7,
      .etaLamMax  = 1.1,
      .zLeadMin   = 0.5
    };
    return config;

  }  // end 'GetReadLambdaJetTreeConfig()'

}  // end CorrelatorQAMakerOptions namespace

#endif

// end ------------------------------------------------------------------------

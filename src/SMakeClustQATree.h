// ----------------------------------------------------------------------------
// 'SMakeClustQATree.h'
// Derek Anderson
// 01.216.2024
//
// SCorrelatorQAMaker plugin to produce the QA tree
// for calorimeter clusters.
// ----------------------------------------------------------------------------

#ifndef SMAKECLUSTQATREE_H
#define SMAKECLUSTQATREE_H

// c++ utilities
#include <string>
#include <vector>
// root utilities
#include <TF1.h>
#include <TTree.h>
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
// calo includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calotrigger/CaloTriggerInfo.h>
// analysis utilities
#include "SBaseQAPlugin.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/GenTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/EvtTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/CalTools.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/Constants.h"
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/Interfaces.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SMakeClustQATreeConfig definition ----------------------------------------

  struct SMakeClustQATreeConfig {

    bool      isEmbed;
    ClustInfo minAccept;
    ClustInfo maxAccept;

  };  // end SMakeClustQATreeConfig



  // SMakeClustQATreeOutput definition ----------------------------------------

  struct SMakeClustQATreeOutput {

    // event information
    RecoInfo recInfo;
    GenInfo  genInfo;

    // cluster information
    vector<ClustInfo> emCalInfo;
    vector<ClustInfo> ihCalInfo;
    vector<ClustInfo> ohCalInfo;

  };  // end SMakeClustQATreeOutput



  // SMakeClustQATree definition ----------------------------------------------

  class SMakeClustQATree : public SubsysReco, public SBaseQAPlugin<SMakeClustQATreeConfig> {

    public:

      // ctor/dtor
      SMakeClustQATree(const string& name = "ClustQATree") : SubsysReco(name) {};
      ~SMakeClustQATree() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // internal methods
      void InitTree();
      void SaveOutput();
      void ResetOutput();
      void DoClustLoop(PHCompositeNode* topNode, const string node);
      bool IsGoodCluster(const RawCluster* cluster);

      // output
      SMakeClustQATreeOutput m_output;

      // root members
      TTree* m_tClustQA;

  };  // end SMakeClustQATree



  // SMakeClustQATree public methods ------------------------------------------

  int SMakeClustQATree::Init(PHCompositeNode* topNode) {

    InitOutput();
    InitTree();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'Init(PHCompositeNode*)'



  int SMakeClustQATree::process_event(PHCompositeNode* topNode) {

    // grab event info
    //   FIXME add in subevent selection
    RecoInfo recInfo(topNode);
    GenInfo  genInfo(topNode, m_config.isEmbed, {2});

    // grab cluster info
    DoClustLoop(topNode, "CLUSTER_CEMC");
    DoClustLoop(topNode, "CLUSTER_HCALIN");
    DoClustLoop(topNode, "CLUSTER_HCALOUT");

    // fill output tree and reset
    m_tClustQA -> Fill();
    ResetOutput();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'process_event(PHCompositeNode* topNode)'



  int SMakeClustQATree::End(PHCompositeNode* topNode) {

    SaveOutput();
    CloseOutput();
    return Fun4AllReturnCodes::EVENT_OK;

  }  // end 'End(PHCompositeNode*)'



  // SMakeClustQATree internal methods -----------------------------------------

  void SMakeClustQATree::InitTree() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeClustQATree::InitTree(): initializing output tree." << endl;
    }

    m_tClustQA = new TTree("tClustQA", "Cluster QA");
    m_tClustQA -> SetBranchAddress("ClusterQA", &m_output);
    return;

  }  // end 'InitTuples()'



  void SMakeClustQATree::SaveOutput() {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeClustQATree::SaveOutput(): saving output." << endl;
    }

    m_outDir   -> cd();
    m_tClustQA -> Write();
    return;

  }  // end 'SaveOutput()'



  void SMakeClustQATree::DoClustLoop(PHCompositeNode* topNode, const string node) {

    if (m_isDebugOn && (m_verbosity > 2)) {
      cout << "SColdQcdCorrelatorAnalysis::SMakeClustQATree::DoClustLoop(PHCompositeNode*, string): looping over clusters." << endl;
    }

    // grab clusters
    RawClusterContainer::ConstRange clusters = GetClusters(topNode, node);

    // loop over clusters
    for (
      RawClusterContainer::ConstIterator itClust = clusters.first;
      itClust != clusters.second;
      itClust++
    ) {

      // grab cluster
      const RawCluster* cluster = itClust -> second;
      if (!cluster) continue;

      // skip if bad
      const bool isGoodClust = IsGoodCluster(cluster);
      if (!isGoodClust) continue;

      // grab cluster info
      ClustInfo clustInfo(cluster, mapNodeOntoIndex[node]);

      // add to relevant list
      switch (mapNodeOntoIndex[node]) {

        case Subsys::EMCal:
          m_output.emCalInfo.push_back(clustInfo);
          break;

        case Subsys::IHCal:
          m_output.ihCalInfo.push_back(clustInfo);
          break;

        case Subsys::OHCal:
          m_output.ohCalInfo.push_back(clustInfo);
          break;

        default:
          cerr << "SColdQcdCorrealtorAnalysis::SMakeClustQATree::DoClustLoop(PHCompositeNode*, string) WARNING: trying to add clusters from unknown node to output!" << endl;
          break;
      }
    }  // end cluster loop
    return;

  }  // end 'DoClustLoop(PHCompositeNode*, string)'



  void SMakeClustQATree::ResetOutput() {

    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SColdQcdCorrealtorAnalysis::SMakeClustQATree::ResetOutput(): resetting output container." << endl;
    }

    m_output.recInfo.Reset();
    m_output.genInfo.Reset();
    m_output.emCalInfo.clear();
    m_output.ihCalInfo.clear();
    m_output.ohCalInfo.clear();
    return;

  }  // end 'ResetOutput()'



  bool SMakeClustQATree::IsGoodCluster(const RawCluster* cluster) {

    // print debug statement
    if (m_isDebugOn && (m_verbosity > 4)) {
      cout << "SMakeClustQATree::IsGoodCluster(RawCluster*) Checking if cluster is good..." << endl;
    }

    // grab cluster info
    ClustInfo clustInfo(cluster);

    // check if cluster is in acceptance and return overall goodness
    const bool isInAccept = IsInClusterAcceptance(clustInfo, m_config.minAccept, m_config.maxAccept);
    return isInAccept;

  }  // end 'IsGoodTrack(RawCluster*)'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

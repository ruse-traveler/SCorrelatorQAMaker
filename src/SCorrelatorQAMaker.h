// ----------------------------------------------------------------------------
// 'SCorrelatorQAMaker.h'
// Derek Anderson
// 10.04.2023
//
// A module to produce TNtuples and histograms for QA
// plots relevant to the sPHENIX Cold QCD Energy-Energy
// Correlator analysis.
// ----------------------------------------------------------------------------

#ifndef SCORRELATORQAMAKER_H
#define SCORRELATORQAMAKER_H

// standard c includes
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
// f4a include
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
// phool includes
#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHCompositeNode.h>
// main geant4 includes
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
// track evaluator includes
#include <g4eval/SvtxTrackEval.h>
#include <g4eval/SvtxEvalStack.h>
// vtx includes
#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
// tracking includes
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxVertexMap.h>
#include <trackbase_historic/TrackAnalysisUtils.h>
// calo includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calotrigger/CaloTriggerInfo.h>
// particle flow includes
#include <particleflowreco/ParticleFlowElement.h>
#include <particleflowreco/ParticleFlowElementContainer.h>
// root objects
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TString.h>
// analysis utilities
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.h"

using namespace std;
using namespace findNode;



// SCorrelatorQAMaker definition --------------------------------------------------

namespace SColdQcdCorrelatorAnalysis {

  class SCorrelatorQAMaker : public SubsysReco {

    public:

      // ctor/dtor
      SCorrelatorQAMaker()  {};
      ~SCorrelatorQAMaker() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

    private:

      // system methods
      void InitTuples();

      // analysis methods
      void DoSigmaDcaCalc();

      // io members
      TFile* m_outFile;

      // QA tuples
      TNtuple* ntTrkQA;
      TNtuple* ntFlowQA;
      TNtuple* ntECalQA;
      TNtuple* ntHCalQA;

  };

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

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

// c++ utilities
#include <string>
#include <optional>
// plugin definitions
#include "SCheckTrackPairs.h"
#include "SMakeTrackQATuple.h"
#include "SMakeClustQATree.h"

using namespace std;



// SCorrelatorQAMaker definition --------------------------------------------------

namespace SColdQcdCorrelatorAnalysis {

  class SCorrelatorQAMaker {

    public:

      // ctor/dtor
      SCorrelatorQAMaker() {};
      ~SCorrelatorQAMaker();

      // global setters
      void SetGlobalDebug(const bool debug);
      void SetGlobalOutFile(const string sOutFile);
      void SetGlobalVerbosity(const int verbosity);

      // plugin initializers
      template <typename T> void InitPlugin(const T& config, optional<string> name);

      // plugin accessors
      SCheckTrackPairs*  CheckTrackPairs()  {return m_checkTrackPairs;}
      SMakeTrackQATuple* MakeTrackQATuple() {return m_makeTrackQATuple;}
      SMakeClustQATree*  MakeClustQATree()  {return m_makeClustQATree;}

    private:

      // plugins
      SCheckTrackPairs*  m_checkTrackPairs;
      SMakeTrackQATuple* m_makeTrackQATuple;
      SMakeClustQATree*  m_makeClustQATree;

  };

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

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
// class declarations
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TString.h>
// analysis utilities
#include "/sphenix/user/danderson/eec/SCorrelatorUtilities/SCorrelatorUtilities.h"

using namespace std;



// SCorrelatorQAMaker definition --------------------------------------------------

namespace SColdQcdCorrelatorAnalysis {

  class SCorrelatorQAMaker {

    public:

      // ctor/dtor
      SCorrelatorQAMaker();
      ~SCorrelatorQAMaker();

      /* TODO plotting methods go here */

    private:

      /* TODO members go here */

      /* TODO helper methods go here */

  };

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

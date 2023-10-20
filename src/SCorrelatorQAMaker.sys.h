// ----------------------------------------------------------------------------
// 'SCorrelatorQAMaker.sys.h'
// Derek Anderson
// 10.20.2023
//
// A module to produce TNtuples and histograms for QA
// plots relevant to the sPHENIX Cold QCD Energy-Energy
// Correlator analysis.
// ----------------------------------------------------------------------------

#pragma once

using namespace std;



// system methods -----------------------------------------------------------------

namespace SColdQcdCorrelatorAnalysis {

  void SCorrelatorQAMaker::InitTuples() {

    /* TODO add debugging message */
    /* TODO add other tuples */

    // track QA leaves
    const vector<string> vecTrkQALeaves = {
      "pt",
      "eta",
      "phi",
      "energy",
      "dcaxy",
      "dcaz",
      "deltapt",
      "quality",
      "nmvtxlayer",
      "ninttlayer",
      "ntpclayer",
      "vtxx",
      "vtxy",
      "vtxz"
    };

    // flatten leaf list
    string argTrkQALeaves("");
    for (size_t iLeaf = 0; iLeaf < vecTrkQALeaves.size(); iLeaf++) {
      argTrkQALeaves.append(vecTrkQALeaves[iLeaf]);
      if ((iLeaf + 1) != vecTrkQALeaves.size()) {
        argTrkQALeaves.append(":");
      }
    }

    m_ntTrkQA = new TNtuple("ntTrkQA", "Track QA", argTrkQALeaves.data());
    return;

  }  // end 'InitTuples()'

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------


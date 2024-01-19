// ----------------------------------------------------------------------------
// 'SCheckCstPairs.h'
// Derek Anderson
// 01.19.2024
//
// SCorrelatorQAMaker plugin to iterate through
// all pairs of constituents in an event and fill
// tuples/histograms comparing them.
//
// This is similar to the `SCheckTrackPairs` plugin,
// which specifically looks at pairs of tracks off
// the node tree. This plugin compares constituents
// of any type off the correlator jet tree.
// ----------------------------------------------------------------------------

#ifndef SCHECKCSTPAIRS_H
#define SCHECKCSTPAIRS_H

// c++ utilities
#include <string>
#include <vector>
// analysis utilities
#include "SBaseQAPlugin.h"

// make common namespaces implicit
using namespace std;
using namespace findNode;
using namespace SColdQcdCorrelatorAnalysis::SCorrelatorUtilities;



namespace SColdQcdCorrelatorAnalysis {

  // SCheckCstPairsConfig definition ------------------------------------------

  struct SCheckCstPairsConfig {

    /* TODO config goes here */

  };  // end SCheckCstPairsConfig


  // SCheckCstPairs definition ------------------------------------------------

  class SCheckCstPairs : public SBaseQAPlugin<SCheckCstPairsConfig> {

    public:

      // ctor/dtor
      SCheckCstPairs() {};
      ~SCheckCstPairs() {};

      // analysis methods
      void Init();
      void Analyze();
      void End();

    private:

      // internal methods
      void SaveOutput();
      /* TODO more will go here */

      // data members 
      /* TODO will go here */

  };  // end SCheckCstPairs



  // SCheckCstPairs public methods --------------------------------------------

  void SCheckCstPairs::Init() {

    InitOutput();
    /* TODO more will go here */
    return;

  }  // end 'Init()'



  void SCheckCstPairs::Analyze() {

    /* TODO body will go here */
    return;

  }  // end 'Analyze()'



  void SCheckCstPairs::End() {

    SaveOutput();
    CloseOutput();
    return;

  }  // end 'End()'



  // SCheckCstPairs internal methods ------------------------------------------

  void SCheckCstPairs::SaveOutput() {

    /* TODO body will go here */
    return;

  }  // end 'SaveOutput()'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

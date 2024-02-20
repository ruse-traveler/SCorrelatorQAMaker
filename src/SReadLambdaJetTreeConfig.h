// ----------------------------------------------------------------------------
// 'ReadLambdaJetTreeConfig.h'
// Derek Anderson
// 02.19.2024
//
// SCorrelatorQAMaker plugin to read lambda-tagged jet
// trees and draw plots and calculate the total no.
// of lambdas.
// ----------------------------------------------------------------------------

#ifndef SREADLAMBDAJETTREECONFIG_H
#define SREADLAMBDAJETTREECONFIG_H

// c++ utilities
#include <string>

// make common namespaces implicit
using namespace std;



namespace SColdQcdCorrelatorAnalysis {

  // SReadLambdaJetTreeConfig definition --------------------------------------

  struct SReadLambdaJetTreeConfig {

    // i/o options
    string inFileName;
    string inTreeName;

    // calculation options
    double ptJetMin;
    double ptLamMin;
    double etaJetMax;
    double etaLamMax;
    double zLeadMin;

  };  // end SReadLambdaJetTreeConfig

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

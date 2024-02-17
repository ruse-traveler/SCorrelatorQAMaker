// ----------------------------------------------------------------------------
// 'SCheckCstPairs.h'
// Derek Anderson
// 01.19.2024
//
// SCorrelatorQAMaker plugin to iterate through all pairs of constituents in
// an event and fill tuples/histograms comparing them.
//
// This is similar to the `SCheckTrackPairs` plugin, which specifically looks
// at pairs of tracks off the node tree. This plugin compares constituents
// of any type off the correlator jet tree.
// ----------------------------------------------------------------------------

#ifndef SCHECKCSTPAIRS_H
#define SCHECKCSTPAIRS_H

// c++ utilities
#include <string>
#include <vector>
// root libraries
#include <TH1.h>
#include <TH2.h>
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
      void InitInput();
      void InitHists();
      void SaveOutput();
      void DoDoubleCstLoop();
      /* TODO more will go here */

      // data members 
      /* TODO will go here */

      // root members
      TH2D* hCstPtOneVsDr;
      TH2D* hCstPtTwoVsDr;
      TH2D* hCstPtFracVsDr;
      TH2D* hCstPhiOneVsDr;
      TH2D* hCstPhiTwoVsDr;
      TH2D* hCstEtaOneVsDr;
      TH2D* hCstEtaTwoVsDr;
      TH2D* hDeltaPhiOneVsDr;
      TH2D* hDeltaPhiTwoVsDr;
      TH2D* hDeltaEtaOneVsDr;
      TH2D* hDeltaEtaTwoVsDr;
      TH2D* hJetPtFracOneVsDr;
      TH2D* hJetPtFracTwoVsDr;
      TH2D* hCstPairWeightVsDr;

  };  // end SCheckCstPairs



  // SCheckCstPairs public methods --------------------------------------------

  void SCheckCstPairs::Init() {

    InitOutput();
    InitHists();
    return;

  }  // end 'Init()'



  void SCheckCstPairs::Analyze() {

    DoDoubleCstLoop();
    return;

  }  // end 'Analyze()'



  void SCheckCstPairs::End() {

    SaveOutput();
    CloseOutput();
    return;

  }  // end 'End()'



  // SCheckCstPairs internal methods ------------------------------------------

  void SCheckCstPairs::InitInput() {

    /* TODO body will go here */
    return;

  }  // end 'InitInput()'



  void SCheckCstPairs::InitHists() {

    // FIXME remove dependence on EECLongestSide
    vector<double> drBinEdges  = m_eecLongSide[0] -> bin_edges();
    size_t         nDrBinEdges = drBinEdges.size();
 
    double drBinEdgeArray[nDrBinEdges];
    for (size_t iDrEdge = 0; iDrEdge < nDrBinEdges; iDrEdge++) {
      drBinEdgeArray[iDrEdge] = drBinEdges.at(iDrEdge);
    }
    hCstPtOneVsDr      = new TH2D("hCstPtOneVsDr",      "", m_nBinsDr, drBinEdgeArray, 200,  0.,   100.);
    hCstPtTwoVsDr      = new TH2D("hCstPtTwoVsDr",      "", m_nBinsDr, drBinEdgeArray, 200,  0.,   100.);
    hCstPtFracVsDr     = new TH2D("hCstPtFracVsDr",     "", m_nBinsDr, drBinEdgeArray, 500,  0.,   5.);
    hCstPhiOneVsDr     = new TH2D("hCstPhiOneVsDr",     "", m_nBinsDr, drBinEdgeArray, 360, -3.15, 3.15);;
    hCstPhiTwoVsDr     = new TH2D("hCstPhiTwoVsDr",     "", m_nBinsDr, drBinEdgeArray, 360, -3.15, 3.15);
    hCstEtaOneVsDr     = new TH2D("hCstEtaOneVsDr",     "", m_nBinsDr, drBinEdgeArray, 400, -2.,   2.);
    hCstEtaTwoVsDr     = new TH2D("hCstEtaTwoVsDr",     "", m_nBinsDr, drBinEdgeArray, 400, -2.,   2.);
    hDeltaPhiOneVsDr   = new TH2D("hDeltaPhiOneVsDr",   "", m_nBinsDr, drBinEdgeArray, 720, -6.30, 6.30);
    hDeltaPhiTwoVsDr   = new TH2D("hDeltaPhiTwoVsDr",   "", m_nBinsDr, drBinEdgeArray, 720, -6.30, 6.30);
    hDeltaEtaOneVsDr   = new TH2D("hDeltaEtaOneVsDr",   "", m_nBinsDr, drBinEdgeArray, 800, -4.,   4.);
    hDeltaEtaTwoVsDr   = new TH2D("hDeltaEtaTwoVsDr",   "", m_nBinsDr, drBinEdgeArray, 800, -4.,   4.);
    hJetPtFracOneVsDr  = new TH2D("hJetPtFracOneVsDr",  "", m_nBinsDr, drBinEdgeArray, 500, 0.,    5.);
    hJetPtFracTwoVsDr  = new TH2D("hJetPtFracTwoVsDr",  "", m_nBinsDr, drBinEdgeArray, 500, 0.,    5.);
    hCstPairWeightVsDr = new TH2D("hCstPairWeightVsDr", "", m_nBinsDr, drBinEdgeArray, 100,  0.,   1.);
    return;

  }  // end 'InitHists()'



  void SCheckCstPairs::SaveOutput() {

    m_outDir           -> cd();
    hCstPtOneVsDr      -> Write();
    hCstPtTwoVsDr      -> Write();
    hCstPtFracVsDr     -> Write();
    hCstPhiOneVsDr     -> Write();
    hCstPhiTwoVsDr     -> Write();
    hCstEtaOneVsDr     -> Write();
    hCstEtaTwoVsDr     -> Write();
    hDeltaPhiOneVsDr   -> Write();
    hDeltaPhiTwoVsDr   -> Write();
    hDeltaEtaOneVsDr   -> Write();
    hDeltaEtaTwoVsDr   -> Write();
    hJetPtFracOneVsDr  -> Write();
    hJetPtFracTwoVsDr  -> Write();
    hCstPairWeightVsDr -> Write();
    return;

  }  // end 'SaveOutput()'



  void SCheckCstPairs::DoDoubleCstLoop() {

    // announce start of event loop
    // FIXME remove all pieces related to SEnergyCorrelator
    const uint64_t nEvts = m_inChain -> GetEntriesFast();

    // event loop
    uint64_t nBytes = 0;
    for (uint64_t iEvt = 0; iEvt < nEvts; iEvt++) {

      const uint64_t entry = LoadTree(iEvt);
      if (entry < 0) break;

      const uint64_t bytes = GetEntry(iEvt);
      if (bytes < 0) {
        break;
      } else {
        nBytes += bytes;
        PrintMessage(8, nEvts, iEvt);
      }

      // jet loop
      uint64_t nJets = (int) m_evtNumJets;
      for (uint64_t iJet = 0; iJet < nJets; iJet++) {

        // clear vector for correlator
        m_jetCstVector.clear();

        // get jet info
        const uint64_t nCsts  = m_jetNumCst -> at(iJet);
        const double   ptJet  = m_jetPt     -> at(iJet);
        const double   etaJet = m_jetEta    -> at(iJet);

        // select jet pt bin & apply jet cuts
        const uint32_t iPtJetBin = GetJetPtBin(ptJet);
        const bool     isGoodJet = ApplyJetCuts(ptJet, etaJet);
        if (!isGoodJet) continue;

        // constituent loop
        for (uint64_t iCst = 0; iCst < nCsts; iCst++) {

          // get cst info
          const double drCst  = (m_cstDr  -> at(iJet)).at(iCst);
          const double etaCst = (m_cstEta -> at(iJet)).at(iCst);
          const double phiCst = (m_cstPhi -> at(iJet)).at(iCst);
          const double ptCst  = (m_cstPt  -> at(iJet)).at(iCst);

          // for weird cst check
          for (uint64_t jCst = 0; jCst < nCsts; jCst++) {

            // skip over the same cst
            if (jCst == iCst) continue;

            // get cst info
            const double etaCstB = (m_cstEta -> at(iJet)).at(jCst);
            const double phiCstB = (m_cstPhi -> at(iJet)).at(jCst);
            const double ptCstB  = (m_cstPt  -> at(iJet)).at(jCst);

            // calculate separation and pt-weight
            const double dhCstAB  = (etaCst - etaCstB);
            const double dfCstAB  = (phiCst - phiCstB);
            const double drCstAB  = sqrt((dhCstAB * dhCstAB) + (dfCstAB * dfCstAB));
            const double ptFrac   = ptCst / ptCstB;
            const double ztJetA   = ptCst / ptJet;
            const double ztJetB   = ptCstB / ptJet;
            const double ptWeight = (ptCst * ptCstB) / (ptJet * ptJet);
            hCstPtOneVsDr      -> Fill(drCstAB, ptCst);
            hCstPtTwoVsDr      -> Fill(drCstAB, ptCstB);
            hCstPtFracVsDr     -> Fill(drCstAB, ptFrac);
            hCstPhiOneVsDr     -> Fill(drCstAB, phiCst);
            hCstPhiTwoVsDr     -> Fill(drCstAB, phiCstB);
            hCstEtaOneVsDr     -> Fill(drCstAB, etaCst);
            hCstEtaTwoVsDr     -> Fill(drCstAB, etaCstB);
            hDeltaPhiOneVsDr   -> Fill(drCstAB, dfCstAB);
            hDeltaPhiTwoVsDr   -> Fill(drCstAB, dfCstAB);
            hDeltaEtaOneVsDr   -> Fill(drCstAB, dhCstAB);
            hDeltaEtaTwoVsDr   -> Fill(drCstAB, dhCstAB);
            hJetPtFracOneVsDr  -> Fill(drCstAB, ztJetA);
            hJetPtFracTwoVsDr  -> Fill(drCstAB, ztJetB);
            hCstPairWeightVsDr -> Fill(drCstAB, ptWeight);
          }  // end 2nd cst loop
        }  // end 1st cst loop
      }  // end jet loop
    }  // end event loop
    return;

  }  // end 'DoDoubleCstLoop()'

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

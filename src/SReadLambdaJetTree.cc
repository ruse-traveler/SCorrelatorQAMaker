// ----------------------------------------------------------------------------
// 'ReadLambdaJetTree.cc'
// Derek Anderson
// 02.19.2024
//
// SCorrelatorQAMaker plugin to read lambda-tagged jet
// trees and draw plots and calculate the total no.
// of lambdas.
// ----------------------------------------------------------------------------

// c++ utilities
#include <cassert>
#include <iostream>
// root libraries
#include <TFile.h>
#include <TTree.h>
// analysis utilities
#include "SReadLambdaJetTree.h"

// make common namespaces implicit
using namespace std;



namespace SColdQcdCorrelatorAnalysis {

  // SReadLambdaJetTree public methods ----------------------------------------

  void SReadLambdaJetTree::Init() {

    // announce start
    cout << "\n  Starting lambda jet tree reader!\n" << endl;

    // initialize output
    InitOutput();

    // run internal routines
    InitInput();
    InitTree();
    InitHists();
    return;

  }  // end 'Init()'



  void SReadLambdaJetTree::Analyze() {

    DoAnalysis();
    return;

  }  // end 'Analyze()'



  void SReadLambdaJetTree::End() {

    // run internal routines
    SaveOutput();
    CloseInput();

    // close output
    CloseOutput();

    // announce end and exit
    cout << "  Finished lambda jet tree reader!\n" << endl;
    return;

  }  // end 'End()'



  // SReadLambdaJetTree internal methods --------------------------------------

  void SReadLambdaJetTree::InitInput() {

    // open input files
    m_fInput = new TFile(m_config.inFileName.data(), "read");
    if (!m_fInput) {
      cerr << "PANIC: couldn't open input file:\n"
           << "       \"" << m_config.inFileName << "\"\n"
           << endl;
      assert(m_fInput);
    }
    cout << "    Opened input file:\n"
         << "      \"" << m_config.inFileName << "\""
         << endl;

    // grab input tree
    m_tInput = (TTree*) m_fInput -> Get(m_config.inTreeName.data());
    if (!m_tInput) {
      cerr << "PANIC: couldn't grab input tree \"" << m_config.inTreeName << "\"!\n" << endl;
      assert(m_tInput);
    }
    cout << "    Grabbed input tree \"" << m_config.inTreeName << "\"." << endl;

    // exit routine
    return;

  }  // end 'InitInput()'



  void SReadLambdaJetTree::InitTree() {

    // set branch addresses
    m_tInput -> SetMakeClass(1);
    m_tInput -> SetBranchAddress("EvtNJets",       &m_evtNJets,       &m_brEvtNJets);
    m_tInput -> SetBranchAddress("EvtNLambdas",    &m_evtNLambdas,    &m_brEvtNLambdas);
    m_tInput -> SetBranchAddress("EvtNTaggedJets", &m_evtNTaggedJets, &m_brEvtNTaggedJets);
    m_tInput -> SetBranchAddress("EvtNChrgPars",   &m_evtNChrgPars,   &m_brEvtNChrgPars);
    m_tInput -> SetBranchAddress("EvtNNeuPars",    &m_evtNNeuPars,    &m_brEvtNNeuPars);
    m_tInput -> SetBranchAddress("EvtSumEPar",     &m_evtSumEPar,     &m_brEvtSumEPar);
    m_tInput -> SetBranchAddress("EvtVtxX",        &m_evtVtxX,        &m_brEvtVtxX);
    m_tInput -> SetBranchAddress("EvtVtxY",        &m_evtVtxY,        &m_brEvtVtxY);
    m_tInput -> SetBranchAddress("EvtVtxZ",        &m_evtVtxZ,        &m_brEvtVtxZ);
    m_tInput -> SetBranchAddress("PartonA_ID",     &m_partonA_ID,     &m_brPartonA_ID);
    m_tInput -> SetBranchAddress("PartonB_ID",     &m_partonB_ID,     &m_brPartonB_ID);
    m_tInput -> SetBranchAddress("PartonA_Px",     &m_partonA_Px,     &m_brPartonA_Px);
    m_tInput -> SetBranchAddress("PartonA_Py",     &m_partonA_Py,     &m_brPartonA_Py);
    m_tInput -> SetBranchAddress("PartonA_Pz",     &m_partonA_Pz,     &m_brPartonA_Pz);
    m_tInput -> SetBranchAddress("PartonA_E",      &m_partonA_E,      &m_brPartonA_E);
    m_tInput -> SetBranchAddress("PartonB_Px",     &m_partonB_Px,     &m_brPartonB_Px);
    m_tInput -> SetBranchAddress("PartonB_Py",     &m_partonB_Py,     &m_brPartonB_Py);
    m_tInput -> SetBranchAddress("PartonB_Pz",     &m_partonB_Pz,     &m_brPartonB_Pz);
    m_tInput -> SetBranchAddress("PartonB_E",      &m_partonB_E,      &m_brPartonB_E);
    m_tInput -> SetBranchAddress("LambdaID",       &m_lambdaID,       &m_brLambdaID);
    m_tInput -> SetBranchAddress("LambdaPID",      &m_lambdaPID,      &m_brLambdaPID);
    m_tInput -> SetBranchAddress("LambdaJetID",    &m_lambdaJetID,    &m_brLambdaJetID);
    m_tInput -> SetBranchAddress("LambdaEmbedID",  &m_lambdaEmbedID,  &m_brLambdaEmbedID);
    m_tInput -> SetBranchAddress("LambdaZ",        &m_lambdaZ,        &m_brLambdaZ);
    m_tInput -> SetBranchAddress("LambdaDr",       &m_lambdaDr,       &m_brLambdaDr);
    m_tInput -> SetBranchAddress("LambdaEnergy",   &m_lambdaEnergy,   &m_brLambdaEnergy);
    m_tInput -> SetBranchAddress("LambdaPt",       &m_lambdaPt,       &m_brLambdaPt);
    m_tInput -> SetBranchAddress("LambdaEta",      &m_lambdaEta,      &m_brLambdaEta);
    m_tInput -> SetBranchAddress("LambdaPhi",      &m_lambdaPhi,      &m_brLambdaPhi);
    m_tInput -> SetBranchAddress("JetHasLambda",   &m_jetHasLambda,   &m_brJetHasLambda);
    m_tInput -> SetBranchAddress("JetNCst",        &m_jetNCst,        &m_brJetNCst);
    m_tInput -> SetBranchAddress("JetID",          &m_jetID,          &m_brJetID);
    m_tInput -> SetBranchAddress("JetE",           &m_jetE,           &m_brJetE);
    m_tInput -> SetBranchAddress("JetPt",          &m_jetPt,          &m_brJetPt);
    m_tInput -> SetBranchAddress("JetEta",         &m_jetEta,         &m_brJetEta);
    m_tInput -> SetBranchAddress("JetPhi",         &m_jetPhi,         &m_brJetPhi);
    m_tInput -> SetBranchAddress("CstID",          &m_cstID,          &m_brCstID);
    m_tInput -> SetBranchAddress("CstPID",         &m_cstPID,         &m_brCstPID);
    m_tInput -> SetBranchAddress("CstJetID",       &m_cstJetID,       &m_brCstJetID);
    m_tInput -> SetBranchAddress("CstEmbedID",     &m_cstEmbedID,     &m_brCstEmbedID);
    m_tInput -> SetBranchAddress("CstZ",           &m_cstZ,           &m_brCstZ);
    m_tInput -> SetBranchAddress("CstDr",          &m_cstDr,          &m_brCstDr);
    m_tInput -> SetBranchAddress("CstEnergy",      &m_cstEnergy,      &m_brCstEnergy);
    m_tInput -> SetBranchAddress("CstPt",          &m_cstPt,          &m_brCstPt);
    m_tInput -> SetBranchAddress("CstEta",         &m_cstEta,         &m_brCstEta);
    m_tInput -> SetBranchAddress("CstPhi",         &m_cstPhi,         &m_brCstPhi);
    cout << "    Set input branches." << endl;

    // exit routine
    return;

  }  // end 'InitTree()'



  void SReadLambdaJetTree::InitHists() {

    /* TODO will go here */
    return;

  }  // end 'InitHists()'



  void SReadLambdaJetTree::DoAnalysis() {

    const int64_t nEvents = m_tInput -> GetEntries();
    cout << "    Beginning event loop: " << nEvents << " to process" << endl;

    int64_t nBytes = 0;
    for (int64_t iEvt = 0; iEvt < nEvents; iEvt++) {

      // grab event
      const int64_t bytes = m_tInput -> GetEntry(iEvt);
      if (bytes < 0) {
        cerr << "WARNING: issue with event " << iEvt << "! Aborting event loop!" << endl;
        break;
      } else {
        nBytes += bytes;
      }

      // announce progress
      const int64_t iProg = iEvt + 1;
      if (iProg == nEvents) {
        cout << "      Processing entry " << iEvt << "/" << nEvents << "..." << endl;
      } else {
        cout << "      Processing entry " << iEvt << "/" << nEvents << "...\r" << flush;
      }

      /* fill histograms here */

    }  // end event loop
    return;

  }  // end 'DoAnalysis()'



  void SReadLambdaJetTree::SaveOutput() {

    m_outDir -> cd();
    /* TODO will go here */
    return;

  }  // end 'SaveOutput()'



  void SReadLambdaJetTree::CloseInput() {

    m_fInput  -> cd();
    m_fInput  -> Close();
    return;

  }  // end 'CloseInput()' 

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

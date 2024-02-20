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
#include <utility>
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
    cout << "\n  Starting lambda jet tree reader!" << endl;

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
    cout << "    Initialized input tree." << endl;

    // exit routine
    return;

  }  // end 'InitTree()'



  void SReadLambdaJetTree::InitHists() {

    // no. of histogram binning
    const size_t nNumBins  = 100;
    const size_t nFracBins = 500;
    const size_t nEneBins  = 100;
    const size_t nEtaBins  = 80;
    const size_t nDfBins   = 180;
    const size_t nDhBins   = 160;
    const size_t nDrBins   = 500;

    // histogram ranges
    const pair<float, float> rNumBins  = {0.0,   100.};
    const pair<float, float> rFracBins = {0.0,   5.0};
    const pair<float, float> rEneBins  = {0.0,   100.};
    const pair<float, float> rEtaBins  = {-2.0,  2.0};
    const pair<float, float> rDfBins   = {-3.15, 3.15};
    const pair<float, float> rDhBins   = {-4.,   4.};
    const pair<float, float> rDrBins   = {0.0,   5.0};

    // axis definitions
    const vector<tuple<string, size_t, pair<float, float>>> vecAxisDef = {
      make_tuple("#eta",            nEtaBins,  rEtaBins),
      make_tuple("E [GeV/c]",       nEneBins,  rEneBins),
      make_tuple("p_{T} [GeV/c]",   nEneBins,  rEneBins),
      make_tuple("#Delta#varphi",   nDfBins,   rDfBins),
      make_tuple("#Delta#eta",      nDhBins,   rDhBins),
      make_tuple("#Deltar",         nDrBins,   rDrBins),
      make_tuple("z = p / p^{jet}", nFracBins, rFracBins)
    };
    const vector<tuple<string, size_t, pair<float, float>>> vecVsDef = {
      make_tuple("#eta",            nEtaBins,  rEtaBins),
      make_tuple("E [GeV/c]",       nEneBins,  rEneBins),
      make_tuple("p_{T} [GeV/c]",   nEneBins,  rEneBins),
      make_tuple("#Delta#varphi",   nDfBins,   rDfBins),
      make_tuple("#Delta#eta",      nDhBins,   rDhBins)
    };

    // histogram base names
    const vector<string> vecBaseNames = {
      "hEta",
      "hEne",
      "hPt",
      "hDeltaPhi",
      "hDeltaEta",
      "hDeltaR",
      "hFrac"
    };
    const vector<string> vecVsMods = {
      "VsJetEta",
      "VsJetEne",
      "VsJetPt",
      "VsJetDeltaPhi",
      "VsJetDeltaEta"
    };
    const vector<string> vecTypeNames = {
      "Lam",
      "LeadLam",
      "Jet",
      "LamJet",
      "LeadLamJet",
      "HighestPtJet"
    };

    // create event histograms
    vecHistEvt.push_back( new TH1D("hNumJet",        ";N_{jet};counts", nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumTagJet",     ";N_{jet};counts", nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumLeadLamJet", ";N_{jet};counts", nNumBins, rNumBins.first, rNumBins.second) );

    // create jet/lambda histograms
    vecHist1D.resize(m_const.nHistType);
    vecHist2D.resize(m_const.nHistType);
    for (size_t iType = 0; iType < m_const.nHistType; iType++) {

      // loop over base variables
      vecHist1D[iType].resize(m_const.nHistVar);
      vecHist2D[iType].resize(m_const.nHistVar);
      for (size_t iVar = 0; iVar < m_const.nHistVar; iVar++) {
 
        // make 1d name and title
        const string sName1D  = vecBaseNames[iVar] + "_" + vecTypeNames[iType];
        const string sTitle1D = ";" + get<0>(vecAxisDef[iVar]) + ";counts";

        // create 1d histogram
        vecHist1D[iType][iVar] = new TH1D(
          sName1D.data(),
          sTitle1D.data(),
          get<1>(vecAxisDef[iVar]),
          get<2>(vecAxisDef[iVar]).first,
          get<2>(vecAxisDef[iVar]).second
        );

        // loop over vs variables
        vecHist2D[iType][iVar].resize(m_const.nHistVs);
        for (size_t iVs = 0; iVs < m_const.nHistVs; iVs++) {
 
          // make 2d name and title
          const string sName2D  = vecBaseNames[iVar] + vecVsMods[iVs] + "_" + vecTypeNames[iType];
          const string sTitle2D = ";" + get<0>(vecVsDef[iVs]) + ";" + get<0>(vecAxisDef[iVar]) + ";counts";

          // create 2d histogram
          vecHist2D[iType][iVar][iVs] = new TH2D(
            sName2D.data(),
            sTitle2D.data(),
            get<1>(vecVsDef[iVar]),
            get<2>(vecVsDef[iVar]).first,
            get<2>(vecVsDef[iVar]).second,
            get<1>(vecAxisDef[iVar]),
            get<2>(vecAxisDef[iVar]).first,
            get<2>(vecAxisDef[iVar]).second
          );
        }  // end vs loop
      }  // end variable loop
    }  // end type loop
    cout << "    Initialized histograms." << endl;

    // exit routine
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


      // identify highest pt jet
      double ptTop  = -1.;
      size_t iTopPt = -1;
      for (size_t iJet = 0; iJet < m_jetPt -> size(); iJet++) {
        if (m_jetPt -> at(iJet) > ptTop) {
          ptTop  = m_jetPt -> at(iJet);
          iTopPt = iJet;
        }
      }  // end 1st jet loop

      // fill highest pt histograms
      Hist hTopPtJet = {
        .eta = m_jetEta -> at(iTopPt),
        .ene = m_jetE   -> at(iTopPt),
        .pt  = m_jetPt  -> at(iTopPt),
        .df  = 0.,
        .dh  = 0.,
        .dr  = 0.,
        .z   = 1.
      };
      VsVar vsTopPtJet = {
        .eta = m_jetEta -> at(iTopPt),
        .ene = m_jetE   -> at(iTopPt),
        .pt  = m_jetPt  -> at(iTopPt),
        .df  = 0.,
        .dh  = 0.
      };
      FillHist1D(Type::HJet, hTopPtJet);
      FillHist2D(Type::HJet, hTopPtJet, vsTopPtJet);

    }  // end event loop
    return;

  }  // end 'DoAnalysis()'



  void SReadLambdaJetTree::SaveOutput() {

    m_outDir -> cd();
    for (auto hEvt : vecHistEvt) {
      hEvt -> Write();
    }
    for (auto type : vecHist1D) {
      for (auto h1D : type) {
        h1D -> Write();
      }
    }
    for (auto type : vecHist2D) {
      for (auto var : type) {
        for (auto h2D : var) {
          h2D -> Write();
        }
      }
    }

    // announce and return
    cout << "    Saved histograms." << endl;
    return;

  }  // end 'SaveOutput()'



  void SReadLambdaJetTree::CloseInput() {

    m_fInput -> cd();
    m_fInput -> Close();
    return;

  }  // end 'CloseInput()' 



  void SReadLambdaJetTree::FillHist1D(const int type, Hist hist) {

    vecHist1D.at(type)[Var::Eta]  -> Fill(hist.eta);
    vecHist1D.at(type)[Var::Ene]  -> Fill(hist.ene);
    vecHist1D.at(type)[Var::Pt]   -> Fill(hist.pt);
    vecHist1D.at(type)[Var::DPhi] -> Fill(hist.df);
    vecHist1D.at(type)[Var::DEta] -> Fill(hist.dh);
    vecHist1D.at(type)[Var::Dr]   -> Fill(hist.dr);
    vecHist1D.at(type)[Var::Z]    -> Fill(hist.z);
    return;

  }  // end 'FillHist1D(int, Hist)'



  void SReadLambdaJetTree::FillHist2D(const int type, Hist hist, VsVar vs) {

    // fill vs. eta histograms
    vecHist2D.at(type)[Var::Eta][Mod::VsEta]  -> Fill(vs.eta, hist.eta);
    vecHist2D.at(type)[Var::Ene][Mod::VsEta]  -> Fill(vs.eta, hist.ene);
    vecHist2D.at(type)[Var::Pt][Mod::VsEta]   -> Fill(vs.eta, hist.pt);
    vecHist2D.at(type)[Var::DPhi][Mod::VsEta] -> Fill(vs.eta, hist.df);
    vecHist2D.at(type)[Var::DEta][Mod::VsEta] -> Fill(vs.eta, hist.dh);
    vecHist2D.at(type)[Var::Dr][Mod::VsEta]   -> Fill(vs.eta, hist.dr);
    vecHist2D.at(type)[Var::Z][Mod::VsEta]    -> Fill(vs.eta, hist.z);

    // fill vs. energy histograms
    vecHist2D.at(type)[Var::Eta][Mod::VsEne]  -> Fill(vs.ene, hist.eta);
    vecHist2D.at(type)[Var::Ene][Mod::VsEne]  -> Fill(vs.ene, hist.ene);
    vecHist2D.at(type)[Var::Pt][Mod::VsEne]   -> Fill(vs.ene, hist.pt);
    vecHist2D.at(type)[Var::DPhi][Mod::VsEne] -> Fill(vs.ene, hist.df);
    vecHist2D.at(type)[Var::DEta][Mod::VsEne] -> Fill(vs.ene, hist.dh);
    vecHist2D.at(type)[Var::Dr][Mod::VsEne]   -> Fill(vs.ene, hist.dr);
    vecHist2D.at(type)[Var::Z][Mod::VsEne]    -> Fill(vs.ene, hist.z);

    // fill vs. pt histograms
    vecHist2D.at(type)[Var::Eta][Mod::VsPt]  -> Fill(vs.pt, hist.eta);
    vecHist2D.at(type)[Var::Ene][Mod::VsPt]  -> Fill(vs.pt, hist.ene);
    vecHist2D.at(type)[Var::Pt][Mod::VsPt]   -> Fill(vs.pt, hist.pt);
    vecHist2D.at(type)[Var::DPhi][Mod::VsPt] -> Fill(vs.pt, hist.df);
    vecHist2D.at(type)[Var::DEta][Mod::VsPt] -> Fill(vs.pt, hist.dh);
    vecHist2D.at(type)[Var::Dr][Mod::VsPt]   -> Fill(vs.pt, hist.dr);
    vecHist2D.at(type)[Var::Z][Mod::VsPt]    -> Fill(vs.pt, hist.z);

    // fill vs. delta-phi histograms
    vecHist2D.at(type)[Var::Eta][Mod::VsDPhi]  -> Fill(vs.df, hist.eta);
    vecHist2D.at(type)[Var::Ene][Mod::VsDPhi]  -> Fill(vs.df, hist.ene);
    vecHist2D.at(type)[Var::Pt][Mod::VsDPhi]   -> Fill(vs.df, hist.pt);
    vecHist2D.at(type)[Var::DPhi][Mod::VsDPhi] -> Fill(vs.df, hist.df);
    vecHist2D.at(type)[Var::DEta][Mod::VsDPhi] -> Fill(vs.df, hist.dh);
    vecHist2D.at(type)[Var::Dr][Mod::VsDPhi]   -> Fill(vs.df, hist.dr);
    vecHist2D.at(type)[Var::Z][Mod::VsDPhi]    -> Fill(vs.df, hist.z);

    // fill vs. delta-eta histograms
    vecHist2D.at(type)[Var::Eta][Mod::VsDEta]  -> Fill(vs.dh, hist.eta);
    vecHist2D.at(type)[Var::Ene][Mod::VsDEta]  -> Fill(vs.dh, hist.ene);
    vecHist2D.at(type)[Var::Pt][Mod::VsDEta]   -> Fill(vs.dh, hist.pt);
    vecHist2D.at(type)[Var::DPhi][Mod::VsDEta] -> Fill(vs.dh, hist.df);
    vecHist2D.at(type)[Var::DEta][Mod::VsDEta] -> Fill(vs.dh, hist.dh);
    vecHist2D.at(type)[Var::Dr][Mod::VsDEta]   -> Fill(vs.dh, hist.dr);
    vecHist2D.at(type)[Var::Z][Mod::VsDEta]    -> Fill(vs.dh, hist.z);
    return;

  }  // end 'FillHist2D(int, Hist, VsVar)'

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

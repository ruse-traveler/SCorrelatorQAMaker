// ----------------------------------------------------------------------------
// 'ReadLambdaJetTree.cc'
// Derek Anderson
// 02.19.2024
//
// SCorrelatorQAMaker plugin to read lambda-tagged jet
// trees and draw plots and calculate the total no.
// of lambdas.
// ----------------------------------------------------------------------------

#define SCORRELATORQAMAKER_SREADLAMBDAJETTREE_CC

// plugin definition
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
    SetHistogramStyles();
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
      "VsEta",
      "VsEne",
      "VsPt",
      "VsDeltaPhi",
      "VsDeltaEta"
    };
    const vector<string> vecTypeNames = {
      "Lam",
      "LeadLam",
      "Jet",
      "LamJet",
      "LeadLamJet",
      "HighestPtJet"
    };

    // make sure sumw2 is on
    TH1::SetDefaultSumw2(true);
    TH2::SetDefaultSumw2(true);

    // create event histograms
    vecHistEvt.push_back( new TH1D("hNumJet",         ";N_{jet};counts",     nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumTagJet",      ";N_{jet};counts",     nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumLeadLamJet",  ";N_{jet};counts",     nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumLambda",      ";N_{#Lambda};counts", nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumLambdaInJet", ";N_{#Lambda};counts", nNumBins, rNumBins.first, rNumBins.second) );
    vecHistEvt.push_back( new TH1D("hNumLeadLambda",  ";N_{#Lambda};counts", nNumBins, rNumBins.first, rNumBins.second) );

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
            get<1>(vecVsDef[iVs]),
            get<2>(vecVsDef[iVs]).first,
            get<2>(vecVsDef[iVs]).second,
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

    int64_t  nBytes      = 0;
    uint64_t nJetTot     = 0;
    uint64_t nTagJetTot  = 0;
    uint64_t nLeadJetTot = 0;
    uint64_t nLamTot     = 0;
    uint64_t nLeadLamTot = 0;
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

      // get total no. of jets, lambdas in vectors
      const size_t nVecJets = m_jetPt    -> size();
      const size_t nVecLams = m_lambdaPt -> size();

      // identify highest pt jet
      bool     foundTopPt = false;
      double   ptTop      = 0.;
      uint64_t iTopPt     = 0;
      for (size_t iJet = 0; iJet < nVecJets; iJet++) {

        // make sure jet satisfies cuts
        const bool isGoodJet = IsGoodJet(m_jetPt -> at(iJet), m_jetEta -> at(iJet));
        if (!isGoodJet) continue; 

        if (m_jetPt -> at(iJet) > ptTop) {
          ptTop      = m_jetPt -> at(iJet);
          iTopPt     = iJet;
          foundTopPt = true;
        }
      }  // end 1st jet loop

      // fill highest pt histograms
      if (foundTopPt) {
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
      }

      // loop over lambdas
      uint64_t nLamEvt     = 0;
      uint64_t nLeadLamEvt = 0;
      for (size_t iLam = 0; iLam < nVecLams; iLam++) {

        // make sure lambda satisfies cuts
        const bool isGoodLam = IsGoodLambda(m_lambdaPt -> at(iLam), m_lambdaEta -> at(iLam));
        if (!isGoodLam) continue;

        // do calculations
        const double dfLam = GetDeltaPhi(m_lambdaPhi -> at(iLam), m_jetPhi -> at(iTopPt));
        const double dhLam = GetDeltaEta(m_lambdaEta -> at(iLam), m_jetEta -> at(iTopPt)); 

        // fill general lambda histograms
        Hist hLambda = {
          .eta = m_lambdaEta    -> at(iLam),
          .ene = m_lambdaEnergy -> at(iLam),
          .pt  = m_lambdaPt     -> at(iLam),
          .df  = dfLam,
          .dh  = dhLam,
          .dr  = m_lambdaDr -> at(iLam),
          .z   = m_lambdaZ  -> at(iLam)
        };
        VsVar vsLambda = {
          .eta = m_lambdaEta    -> at(iLam),
          .ene = m_lambdaEnergy -> at(iLam),
          .pt  = m_lambdaPt     -> at(iLam),
          .df  = dfLam,
          .dh  = dhLam
        };
        FillHist1D(Type::Lam, hLambda);
        FillHist2D(Type::Lam, hLambda, vsLambda);
        ++nLamEvt;
        ++nLamTot;

        const bool isLeadLam = IsLeadingLambda(m_lambdaZ -> at(iLam));
        if (isLeadLam) {
          FillHist1D(Type::LLam, hLambda);
          FillHist2D(Type::LLam, hLambda, vsLambda);
          ++nLeadLamEvt;
          ++nLeadLamTot;
        }
      }  // end lambda loop

      // loop over jets
      uint64_t nJetEvt     = 0;
      uint64_t nTagJetEvt  = 0;
      uint64_t nLeadJetEvt = 0;
      for (size_t iJet = 0; iJet < nVecJets; iJet++) {

        // make sure jet satisfies cuts
        const bool isGoodJet = IsGoodJet(m_jetPt -> at(iJet), m_jetEta -> at(iJet));
        if (!isGoodJet) continue;

        // do calculations
        const double dfJet = GetDeltaPhi(m_jetPhi -> at(iJet), m_jetPhi -> at(iTopPt));
        const double dhJet = GetDeltaEta(m_jetEta -> at(iJet), m_jetEta -> at(iTopPt));

        // fill general jet histograms
        Hist hJet = {
          .eta = m_jetEta -> at(iJet),
          .ene = m_jetE   -> at(iJet),
          .pt  = m_jetPt  -> at(iJet),
          .df  = dfJet,
          .dh  = dhJet,
          .dr  = 0.,
          .z   = 1.
        };
        VsVar vsJet = {
          .eta = m_jetEta -> at(iJet),
          .ene = m_jetE   -> at(iJet),
          .pt  = m_jetPt  -> at(iJet),
          .df  = dfJet,
          .dh  = dhJet
        };
        FillHist1D(Type::Jet, hJet);
        FillHist2D(Type::Jet, hJet, vsJet);
        ++nJetEvt;
        ++nJetTot;

        // if no associated lambda, continue
        const bool hasLambda = m_jetHasLambda -> at(iJet);
        if (!hasLambda) continue;

        // find associated lambda(s)
        bool     hasLeadLam = false;
        uint64_t nLamJet    = 0;
        for (size_t iLam = 0; iLam < nVecLams; iLam++) {

          // check if lambda is associated
          const bool isAssocLam = IsAssociatedLambda(m_lambdaJetID -> at(iLam), m_jetID -> at(iJet));
          if (!isAssocLam) {
            continue;
          }
          ++nLamJet;

          // check if lambda is leading
          const bool isLeadLam = IsLeadingLambda(m_lambdaZ -> at(iLam));
          if (isLeadLam) hasLeadLam = true;

        }  // end lambda loop
        vecHistEvt.at(Evt::NLamJet) -> Fill(nLamJet);

        // fill tagged jet histograms
        FillHist1D(Type::LJet, hJet);
        FillHist2D(Type::LJet, hJet, vsJet);
        ++nTagJetEvt;
        ++nTagJetTot;

        // fill jet w/ leading lambda histograms
        if (hasLeadLam) {
          FillHist1D(Type::LLJet, hJet);
          FillHist2D(Type::LLJet, hJet, vsJet);
          ++nLeadJetEvt;
          ++nLeadJetTot;
        }
      }  // end 2nd jet loop

      // fill event histograms
      vecHistEvt.at(Evt::NJet)     -> Fill(nJetEvt);
      vecHistEvt.at(Evt::NTagJet)  -> Fill(nTagJetEvt);
      vecHistEvt.at(Evt::NLeadJet) -> Fill(nLeadJetEvt); 
      vecHistEvt.at(Evt::NLam)     -> Fill(nLamEvt);
      vecHistEvt.at(Evt::NLeadLam) -> Fill(nLeadLamEvt);

    }  // end event loop
    cout << "    Event loop finished!\n"
         << "      nLambda      = " << nLamTot     << "\n"
         << "      nLeadLambda  = " << nLeadLamTot << "\n"
         << "      nJet         = " << nJetTot     << "\n"
         << "      nTaggedJets  = " << nTagJetTot  << "\n"
         << "      nLeadTagJets = " << nLeadJetTot
         << endl;

    // exit routine
    return;

  }  // end 'DoAnalysis()'



  void SReadLambdaJetTree::SetHistogramStyles() {

    for (auto hEvt : vecHistEvt) {
      hEvt -> SetLineStyle(m_config.defLineStyle);
      hEvt -> SetLineColor(m_config.defHistColor);
      hEvt -> SetFillStyle(m_config.defFillStyle);
      hEvt -> SetFillColor(m_config.defHistColor);
      hEvt -> SetMarkerStyle(m_config.defMarkStyle);
      hEvt -> SetMarkerColor(m_config.defHistColor);
      hEvt -> GetXaxis() -> CenterTitle(m_config.centerTitle);
      hEvt -> GetXaxis() -> SetTitleFont(m_config.defHistFont);
      hEvt -> GetXaxis() -> SetTitleSize(m_config.defTitleX);
      hEvt -> GetXaxis() -> SetTitleOffset(m_config.defOffX);
      hEvt -> GetXaxis() -> SetLabelSize(m_config.defLabelX);
      hEvt -> GetYaxis() -> CenterTitle(m_config.centerTitle);
      hEvt -> GetYaxis() -> SetTitleFont(m_config.defHistFont);
      hEvt -> GetYaxis() -> SetTitleSize(m_config.defTitleY);
      hEvt -> GetYaxis() -> SetTitleOffset(m_config.defOffY);
      hEvt -> GetYaxis() -> SetLabelSize(m_config.defLabelY);
    }
    for (auto type : vecHist1D) {
      for (auto h1D : type) {
        h1D -> SetLineStyle(m_config.defLineStyle);
        h1D -> SetLineColor(m_config.defHistColor);
        h1D -> SetFillStyle(m_config.defFillStyle);
        h1D -> SetFillColor(m_config.defHistColor);
        h1D -> SetMarkerStyle(m_config.defMarkStyle);
        h1D -> SetMarkerColor(m_config.defHistColor);
        h1D -> GetXaxis() -> CenterTitle(m_config.centerTitle);
        h1D -> GetXaxis() -> SetTitleFont(m_config.defHistFont);
        h1D -> GetXaxis() -> SetTitleSize(m_config.defTitleX);
        h1D -> GetXaxis() -> SetTitleOffset(m_config.defOffX);
        h1D -> GetXaxis() -> SetLabelSize(m_config.defLabelX);
        h1D -> GetYaxis() -> CenterTitle(m_config.centerTitle);
        h1D -> GetYaxis() -> SetTitleFont(m_config.defHistFont);
        h1D -> GetYaxis() -> SetTitleSize(m_config.defTitleY);
        h1D -> GetYaxis() -> SetTitleOffset(m_config.defOffY);
        h1D -> GetYaxis() -> SetLabelSize(m_config.defLabelY);
      }
    }
    for (auto type : vecHist2D) {
      for (auto var : type) {
        for (auto h2D : var) {
          h2D -> SetLineStyle(m_config.defLineStyle);
          h2D -> SetLineColor(m_config.defHistColor);
          h2D -> SetFillStyle(m_config.defFillStyle);
          h2D -> SetFillColor(m_config.defHistColor);
          h2D -> SetMarkerStyle(m_config.defMarkStyle);
          h2D -> SetMarkerColor(m_config.defHistColor);
          h2D -> GetXaxis() -> CenterTitle(m_config.centerTitle);
          h2D -> GetXaxis() -> SetTitleFont(m_config.defHistFont);
          h2D -> GetXaxis() -> SetTitleSize(m_config.defTitleX);
          h2D -> GetXaxis() -> SetTitleOffset(m_config.defOffX);
          h2D -> GetXaxis() -> SetLabelSize(m_config.defLabelX);
          h2D -> GetYaxis() -> CenterTitle(m_config.centerTitle);
          h2D -> GetYaxis() -> SetTitleFont(m_config.defHistFont);
          h2D -> GetYaxis() -> SetTitleSize(m_config.defTitleY);
          h2D -> GetYaxis() -> SetTitleOffset(m_config.defOffY);
          h2D -> GetYaxis() -> SetLabelSize(m_config.defLabelY);
          h2D -> GetZaxis() -> CenterTitle(m_config.centerTitle);
          h2D -> GetZaxis() -> SetTitleFont(m_config.defHistFont);
          h2D -> GetZaxis() -> SetTitleSize(m_config.defTitleZ);
          h2D -> GetZaxis() -> SetTitleOffset(m_config.defOffZ);
          h2D -> GetZaxis() -> SetLabelSize(m_config.defLabelZ);
        }
      }
    }
    cout << "    Set histogram styles." << endl;

    // exit internal routine
    return;

  }  // end 'SetHistogramStyles()'



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
    cout << "    Saved histograms." << endl;

    // exit internal routine
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



  bool SReadLambdaJetTree::IsGoodJet(const double pt, const double eta) {

    const bool isGoodPt  = (pt > m_config.ptJetMin);
    const bool isGoodEta = (abs(eta) < m_config.etaJetMax);
    const bool isGoodJet = (isGoodPt && isGoodEta);
    return isGoodJet;

  }  // end 'IsGoodJet(double, double)'



  bool SReadLambdaJetTree::IsGoodLambda(const double pt, const double eta) {

    const bool isGoodPt  = (pt > m_config.ptLamMin);
    const bool isGoodEta = (abs(eta) < m_config.etaLamMax);
    const bool isGoodLam = (isGoodPt && isGoodEta);
    return isGoodLam;

  }  // end 'IsGoodLambda(double, double)'



  bool SReadLambdaJetTree::IsLeadingLambda(const double z) {

    const bool isLeadLam = (z > m_config.zLeadMin);
    return isLeadLam;

  }  // end 'IsLeadingLambda(double)'



  bool SReadLambdaJetTree::IsAssociatedLambda(const int idLam, const int idJet) {

    const bool isAssoc = (idLam == idJet);
    return isAssoc;

  }  // end 'IsAssociatedLambda(int, int)'



  double SReadLambdaJetTree::GetDeltaPhi(const double phiA, const double phiB) {

    double dPhi = phiA - phiB;
    if (dPhi < m_const.minDPhi) dPhi += TMath::TwoPi();
    if (dPhi > m_const.maxDPhi) dPhi -= TMath::TwoPi();
    return dPhi;

  }  // end 'GetDeltaPhi(double, double)'



  double SReadLambdaJetTree::GetDeltaEta(const double etaA, const double etaB) {

    const double dEta = etaA - etaB;
    return dEta;

  }  // end 'GetDeltaEta(double, double)'

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

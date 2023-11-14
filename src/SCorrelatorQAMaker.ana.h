// ----------------------------------------------------------------------------
// 'SCorrelatorQAMaker.ana.h'
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

/*
  void SCorrelatorQAMaker::DoSigmaDcaCalc() {

    // TODO clean up and remove unncessary bits

    // global constants
    const size_t NDca(2);
    const size_t NPar(4);

    // options ------------------------------------------------------------------

    // histogram parameters
    const array<string, NDca> sDcaVsPtAll = {"hDcaXYvsPtAll",       "hDcaZvsPtAll"};
    const array<string, NDca> sDcaVsPtSel = {"hDcaXYvsPtSel",       "hDcaZvsPtSel"};
    const array<string, NDca> sWidthName  = {"hWidthDcaXY",         "hWidthDcaZ"};
    const string              sDcaXYvsZAll("hDcaXYvsZAll");
    const string              sDcaXYvsZSel("hDcaXYvsZSel");

    // fit parameters
    const uint16_t            cut(0);
    const string              sDcaFit("gaus(0)");
    const string              sWidthFit("[0]+[1]/x+[2]/(x*x)+[3]/(x*x*x)");
    const string              sDcaOpt("QNR");
    const string              sWidthOpt("R");
    const string              sParam("_2");
    const pair<float, float>  ptFitRange    = {1., 15.};
    const array<string, NDca> sDcaFitName   = {"fFitDcaXY",   "fFitDcaZ"};
    const array<string, NDca> sWidthFitName = {"fFitWidthXY", "fFitWidthZ"};
    const array<float,  NPar> fWidthGuess   = {1., 1., 1., 1.};

    // cut parameters
    const bool   doOtherCuts(true);
    const float  nMvtxMin(2.);
    const float  nInttMin(1.);
    const float  nTpcMin(24.);
    const float  qualMax(10.);
    const float  etaMax(1.1);
    const float  ptMin(0.1);
    const float  nCut(3.);
    const string sAll("all tracks");
    const string sSel("selected tracks");

    // text parameters
    const vector<string> sText = {
      "#bf{#it{sPHENIX}} simulation [Run 8]",
      "PYTHIA-8, JS 10 GeV sample",
      "#bf{reco. tracks}"
    };

    // set tuple branches -----------------------------------------------------

    // declare leaves
    float pt;
    float eta;
    float phi;
    float energy;
    float dcaxy;
    float dcaz;
    float deltapt;
    float quality;
    float nmvtxlayer;
    float ninttlayer;
    float ntpclayer;

    // set branch addresses
    m_ntTrkQA -> SetBranchAddress("pt",        &pt);
    m_ntTrkQA -> SetBranchAddress("eta",       &eta);
    m_ntTrkQA -> SetBranchAddress("phi",       &phi);
    m_ntTrkQA -> SetBranchAddress("energy",    &energy);
    m_ntTrkQA -> SetBranchAddress("dcaxy",     &dcaxy);
    m_ntTrkQA -> SetBranchAddress("dcaz",      &dcaz);
    m_ntTrkQA -> SetBranchAddress("deltapt",   &deltapt);
    m_ntTrkQA -> SetBranchAddress("quality",   &quality);
    m_ntTrkQA -> SetBranchAddress("nmvtxlayer",&nmvtxlayer);
    m_ntTrkQA -> SetBranchAddress("ninttlayer",&ninttlayer);
    m_ntTrkQA -> SetBranchAddress("ntpclayer", &ntpclayer);
    cout << "    Set input tuple branches." << endl;

    // initialize histograms ----------------------------------------------------

    // histogram binning
    const tuple<size_t, pair<float, float>> dcaBins = {2000, make_pair(-5., 5.)};
    const tuple<size_t, pair<float, float>> ptBins  = {100,  make_pair(0.,  100.)};

    // declare histograms
    array<TH1D*, NDca> arrDcaWidth;
    array<TH2D*, NDca> arrDcaVsPtAll;
    array<TH2D*, NDca> arrDcaVsPtSel;
    for (size_t iDca = 0; iDca < NDca; iDca++) {
      arrDcaWidth[iDca] = new TH1D(
        sWidthName[iDca].data(),
        "",
        get<0>(ptBins),
        get<1>(ptBins).first,
        get<1>(ptBins).second
      );
      arrDcaVsPtAll[iDca] = new TH2D(
        sDcaVsPtAll[iDca].data(),
        sAll.data(),
        get<0>(ptBins),
        get<1>(ptBins).first,
        get<1>(ptBins).second,
        get<0>(dcaBins),
        get<1>(dcaBins).first,
        get<1>(dcaBins).second
      );
      arrDcaVsPtSel[iDca] = new TH2D(
        sDcaVsPtSel[iDca].data(),
        sSel.data(),
        get<0>(ptBins),
        get<1>(ptBins).first,
        get<1>(ptBins).second,
        get<0>(dcaBins),
        get<1>(dcaBins).first,
        get<1>(dcaBins).second
      );
    }

    TH2D* hDcaXYvsZAll = new TH2D(
      sDcaXYvsZAll.data(),
      sAll.data(),
      get<0>(dcaBins),
      get<1>(dcaBins).first,
      get<1>(dcaBins).second,
      get<0>(dcaBins),
      get<1>(dcaBins).first,
      get<1>(dcaBins).second
    );
    TH2D* hDcaXYvsZSel = new TH2D(
      sDcaXYvsZSel.data(),
      sSel.data(),
      get<0>(dcaBins),
      get<1>(dcaBins).first,
      get<1>(dcaBins).second,
      get<0>(dcaBins),
      get<1>(dcaBins).first,
      get<1>(dcaBins).second
    );
    cout << "    Initialized output histograms." << endl;

    // 1st entry loop -------------------------------------------------------------

    // TODO replace with RDataFrames

    // start entry loop
    uint64_t nEntries = m_ntTrkQA -> GetEntries();
    cout << "    Starting 1st entry loop: " << nEntries << " to process" << endl;

    uint64_t nBytes = 0;
    for (uint64_t iEntry = 0; iEntry < nEntries; iEntry++) {

      // grab entry
      const uint64_t bytes = m_ntTrkQA -> GetEntry(iEntry);
      if (bytes < 0) {
        cerr << "WARNING: issue with entry " << iEntry << "! Aborting loop!" << endl;
        break;
      } else {
        nBytes += bytes;
      }

      // announce progress
      const uint64_t iProg = iEntry + 1;
      if (iProg == nEntries) {
        cout << "      Processing entry " << iProg << "/" << nEntries << "..." << endl;
      } else {
        cout << "      Processing entry " << iProg << "/" << nEntries << "...\r" << flush;
      }

      // apply cuts other than dca if needed
      if (doOtherCuts) {
        const bool isNumMvtxGood = (nmvtxlayer > nMvtxMin);
        const bool isNumInttGood = (ninttlayer > nInttMin);
        const bool isNumTpcGood  = (ntpclayer  > nTpcMin);
        const bool isQualityGood = (quality    < qualMax);
        const bool isEtaGood     = (abs(eta)   < etaMax);
        const bool isPtGood      = (pt         > ptMin);
        const bool isTrackGood   = (isNumMvtxGood && isNumInttGood && isNumTpcGood && isQualityGood && isEtaGood && isPtGood);
        if (!isTrackGood) continue;
      }

      // fill histograms
      arrDcaVsPtAll[0] -> Fill(pt,   dcaxy);
      arrDcaVsPtAll[1] -> Fill(pt,   dcaz);
      hDcaXYvsZAll     -> Fill(dcaz, dcaxy);

    }  // end entry loop 1
    cout << "    Finished 1st entry loop!" << endl;

    // get & fit widths ---------------------------------------------------------

    array<TF1*, NDca> arrWidthFits;
    for (size_t iDca = 0; iDca < NDca; iDca++) {

      // create result name
      TString sResult(sDcaVsPtAll[iDca].data());
      sResult.Append(sParam.data());

      // determine dca range
      const uint64_t iStart = arrDcaVsPtAll[iDca] -> FindFirstBinAbove(0., 1);
      const uint64_t iStop  = arrDcaVsPtAll[iDca] -> FindLastBinAbove(0.,  1);
      const float    start  = arrDcaVsPtAll[iDca] -> GetXaxis() -> GetBinLowEdge(iStart);
      const float    stop   = arrDcaVsPtAll[iDca] -> GetXaxis() -> GetBinLowEdge(iStop + 1);

      // declare fit and parameter pointers
      TF1*       fDcaFit   = new TF1(sDcaFitName[iDca].data(), sDcaFit.data(), start, stop);
      TObjArray* arrParams = NULL;

      // fit slices of dca vs pt
      arrDcaVsPtAll[iDca]  -> FitSlicesY(fDcaFit, iStart, iStop, cut, sDcaOpt.data(), arrParams);
      arrDcaWidth[iDca] =  (TH1D*) gDirectory -> Get(sResult.Data()) -> Clone();

      // declare width fit function
      arrWidthFits[iDca] = new TF1(sWidthFitName[iDca].data(), sWidthFit.data(), start, stop);
      for (size_t iPar = 0; iPar < NPar; iPar++) {
        arrWidthFits[iDca] -> SetParameters(iPar, fWidthGuess[iPar]);
      }

      // fit width of dca distributions 
      arrDcaWidth[iDca] -> Fit(arrWidthFits[iDca], sWidthOpt.data(), "", ptFitRange.first, ptFitRange.second);
      arrDcaWidth[iDca] -> SetName(sWidthName[iDca].data());
    }  // end dca variable loop

    // 2nd entry loop -------------------------------------------------------------

    // TODO replace with RDataFrame

    // start entry loop
    cout << "    Starting 2nd entry loop: applying a cut of " << nCut << " times the DCA width" << endl;

    nBytes = 0;
    for (uint64_t iEntry = 0; iEntry < nEntries; iEntry++) {

      // grab entry
      const uint64_t bytes = m_ntTrkQA -> GetEntry(iEntry);
      if (bytes < 0) {
        cerr << "WARNING: issue with entry " << iEntry << "! Aborting loop!" << endl;
        break;
      } else {
        nBytes += bytes;
      }

      // announce progress
      const uint64_t iProg = iEntry + 1;
      if (iProg == nEntries) {
        cout << "      Processing entry " << iProg << "/" << nEntries << "..." << endl;
      } else {
        cout << "      Processing entry " << iProg << "/" << nEntries << "...\r" << flush;
      }

      // apply cuts other than dca if needed
      if (doOtherCuts) {
        const bool isNumMvtxGood = (nmvtxlayer > nMvtxMin);
        const bool isNumInttGood = (ninttlayer > nInttMin);
        const bool isNumTpcGood  = (ntpclayer  > nTpcMin);
        const bool isQualityGood = (quality    < qualMax);
        const bool isEtaGood     = (abs(eta)   < etaMax);
        const bool isPtGood      = (pt         > ptMin);
        const bool isTrackGood   = (isNumMvtxGood && isNumInttGood && isNumTpcGood && isQualityGood && isEtaGood && isPtGood);
        if (!isTrackGood) continue;
      }

      // calculate max dca
      const double dcaWidthXY = arrWidthFits[0] -> Eval(pt);
      const double dcaWidthZ  = arrWidthFits[1] -> Eval(pt);
      const double dcaMaxXY   = nCut * dcaWidthXY;
      const double dcaMaxZ    = nCut * dcaWidthZ;

      // apply cuts
      const bool isTrkInDcaCutXY = (abs(dcaxy) < dcaMaxXY);
      const bool isTrkInDcaCutZ  = (abs(dcaz)  < dcaMaxZ);
      const bool isTrkInDcaCut   = (isTrkInDcaCutXY && isTrkInDcaCutZ);
      if (!isTrkInDcaCut) continue;

      // fill histograms
      arrDcaVsPtSel[0] -> Fill(pt, dcaxy);
      arrDcaVsPtSel[1] -> Fill(pt, dcaz);
      hDcaXYvsZSel     -> Fill(dcaz, dcaxy);

    }  // end entry loop 1
    cout << "    Finished 2nd entry loop!" << endl;

    // make cut lines -----------------------------------------------------------

    // line options
    const uint16_t fColLin(2);
    const uint16_t fStyLin(1);
    const uint16_t fWidLin(2);

    array<TF1*, NDca>  arrFitsNeg;
    array<TF1*, NDca>  arrFitsPos;
    array<float, NPar> arrNegParams;
    array<float, NPar> arrPosParams;
    for (size_t iDca = 0; iDca < NDca; iDca++) {

      // make name
      const TString sFitBase = arrWidthFits[iDca] -> GetName();
      TString       sFitNeg  = sFitBase;
      TString       sFitPos  = sFitBase;
      sFitNeg.Append("_Neg");
      sFitPos.Append("_Pos");

      // set parameters
      for (size_t iPar = 0; iPar < NPar; iPar++) {
        arrNegParams[iPar]  = arrWidthFits[iDca] -> GetParameter(iPar);
        arrPosParams[iPar]  = arrWidthFits[iDca] -> GetParameter(iPar);
        arrNegParams[iPar] *= (-1. * nCut);
        arrPosParams[iPar] *= nCut;
      }

      // make functions
      arrFitsNeg[iDca] = (TF1*) arrWidthFits[iDca] -> Clone();
      arrFitsPos[iDca] = (TF1*) arrWidthFits[iDca] -> Clone();
      arrFitsNeg[iDca] -> SetName(sFitNeg.Data());
      arrFitsPos[iDca] -> SetName(sFitPos.Data());
      arrFitsNeg[iDca] -> SetLineColor(fColLin);
      arrFitsPos[iDca] -> SetLineColor(fColLin);
      arrFitsNeg[iDca] -> SetLineStyle(fStyLin);
      arrFitsPos[iDca] -> SetLineStyle(fStyLin);
      arrFitsNeg[iDca] -> SetLineWidth(fWidLin);
      arrFitsPos[iDca] -> SetLineWidth(fWidLin);
      for (size_t iPar = 0; iPar < NPar; iPar++) {
        arrFitsNeg[iDca] -> SetParameter(iPar, arrNegParams[iPar]);
        arrFitsPos[iDca] -> SetParameter(iPar, arrPosParams[iPar]);
      }
    }  // end variable loops
    cout << "    Made cut lines." << endl;

    return;
  }  // end 'DoSigmaDcaCalc()'
*/

}  // end SColdQcdCorrelatorAnalysis namespace

// end ------------------------------------------------------------------------

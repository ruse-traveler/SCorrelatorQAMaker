// ----------------------------------------------------------------------------
// 'CalculateSigmaDca.cxx'
// Derek Anderson
// 10.11.2023
//
// Quick script to fit sigma-dca/dca distributions
// to figure out pt-dependent cut from SCorrelatorJetTree
// track QA tuples.

// c utilities
#include <string>
#include <vector>
#include <utility>
#include <iostream>
// root classes
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TMath.h>
#include <TError.h>
#include <TNtuple.h>
#include <TString.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TPaveText.h>
#include <TGraphErrors.h>

using namespace std;

// global constants
static const size_t NDca(2);
static const size_t NPar(3);
static const size_t NObj(2);
static const size_t NVtx(4);



void CalculateSigmaDca() {

  // lower verbosity
  gErrorIgnoreLevel = kError;
  cout << "\n  Starting sigma-dca/dca check calculation..." << endl;

  // options ------------------------------------------------------------------

  // io parameters
  const string sInput("../../SCorrelatorJetTree/output/condor/final_merge/correlatorJetTree.pp200py8jet10run8_roughCutsWithTrkTupleQA.d11m10y2023.root");
  const string sTuple("QA/Tracks/ntTrkQA"); 
  const string sOutput("test.root");

  // histogram parameters
  const array<string, NDca> sDcaVsPtAll = {"hDcaXYvsPtAll",       "hDcaZvsPtAll"};
  const array<string, NDca> sDcaVsPtSel = {"hDcaXYvsPtSel",       "hDcaZvsPtSel"};
  const array<string, NDca> sDcaName    = {"hDcaXY",              "hDcaZ"};
  const array<string, NDca> sWidthName  = {"hWidthDcaXY",         "hWidthDcaZ"};
  const array<string, NDca> sTitleWidth = {"#DeltaDCA_{xy} [cm]", "#DeltaDCA_{z} [cm]"};
  const array<string, NDca> sTitleDca   = {"DCA_{xy} [cm]",       "DCA_{z} [cm]"};
  const string              sTitlePt("p_{T}^{trk} [GeV/c]");
  const string              sDcaXyVsZAll("hDcaXyVsZAll");
  const string              sDcaXyVsZSel("hDcaXyVsZSel");

  // fit parameters
  const uint16_t            cut(0);
  const string              sDcaFit("gaus(0)");
  const string              sWidthFit("[0]+[1]/x+[2]/(x*x)");
  const string              sDcaOpt("QNR");
  const string              sWidthOpt("R");
  const string              sParam("_2");
  const array<string, NDca> sDcaFitName   = {"fFitDcaXY",   "fFitDcaZ"};
  const array<string, NDca> sWidthFitName = {"fFitWidthXY", "fFitWidthZ"};
  const array<float,  NPar> fWidthGuess   = {0.002, -0.005, 0.005};

  // cut parameters
  const float  nCut(3.);
  const string sAll("all tracks");
  const string sSel("selected tracks");

  // plot options
  const pair<float, float>  ptPlotRange    = {0.,    25.};
  const pair<float, float>  dcaPlotRange   = {-0.5,  0.5};
  const pair<float, float>  widthPlotRange = {0.001, 0.05};
  const array<string, NDca> sDcaWidthPlot  = {"cWidthDcaXY", "cWidthDcaZ"};
  const array<string, NDca> sDcaVsPtPlot   = {"cDcaXYvsPt",  "cDcaZvsPt"};
  const string              sDcaXyVsZPlot("cDcaXyVsZ");

  // text parameters
  const vector<string> sText = {
    "#bf{#it{sPHENIX}} simulation [Run 8]",
    "PYTHIA-8, JS 10 GeV sample",
    "#bf{reco. tracks}"
  };

  // open files & initialize tuple --------------------------------------------

  // open file
  TFile* fOutput = new TFile(sOutput.data(), "recreate");
  TFile* fInput  = new TFile(sInput.data(),  "read");
  if (!fOutput || !fInput) {
    cerr << "PANIC: couldn't open a file!\n"
         << "       fOutput = " << fOutput << ", " << fInput << "\n"
         << endl;
    return;
  }
  cout << "    Opened files." << endl;

  // grab input tuple
  TNtuple* ntInput = (TNtuple*) fInput -> Get(sTuple.data());
  if (!ntInput) {
    cerr << "PANIC: couldn't grab input tuple!\n" << endl;
    return;
  }
  cout << "    Grabbed input tuple." << endl;

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
  ntInput -> SetBranchAddress("pt",        &pt);
  ntInput -> SetBranchAddress("eta",       &eta);
  ntInput -> SetBranchAddress("phi",       &phi);
  ntInput -> SetBranchAddress("energy",    &energy);
  ntInput -> SetBranchAddress("dcaxy",     &dcaxy);
  ntInput -> SetBranchAddress("dcaz",      &dcaz);
  ntInput -> SetBranchAddress("deltapt",   &deltapt);
  ntInput -> SetBranchAddress("quality",   &quality);
  ntInput -> SetBranchAddress("nmvtxlayer",&nmvtxlayer);
  ntInput -> SetBranchAddress("ninttlayer",&ninttlayer);
  ntInput -> SetBranchAddress("ntpclayer", &ntpclayer);
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

  TH2D* hDcaXyVsZAll = new TH2D(
    sDcaXyVsZAll.data(),
    sAll.data(),
    get<0>(dcaBins),
    get<1>(dcaBins).first,
    get<1>(dcaBins).second,
    get<0>(dcaBins),
    get<1>(dcaBins).first,
    get<1>(dcaBins).second
  );
  TH2D* hDcaXyVsZSel = new TH2D(
    sDcaXyVsZSel.data(),
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

  // start entry loop
  uint64_t nEntries = ntInput -> GetEntries();
  cout << "    Starting 1st entry loop: " << nEntries << " to process" << endl;

  uint64_t nBytes = 0;
  for (uint64_t iEntry = 0; iEntry < nEntries; iEntry++) {

    // grab entry
    const uint64_t bytes = ntInput -> GetEntry(iEntry);
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

    // fill histograms
    arrDcaVsPtAll[0] -> Fill(pt,   dcaxy);
    arrDcaVsPtAll[1] -> Fill(pt,   dcaz);
    hDcaXyVsZAll     -> Fill(dcaz, dcaxy);

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
    arrDcaWidth[iDca]  -> Fit(arrWidthFits[iDca], sWidthOpt.data());
  }  // end dca variable loop

  // 2nd entry loop -------------------------------------------------------------

  // start entry loop
  cout << "    Starting 2nd entry loop: applying a cut of " << nCut << " times the DCA width" << endl;

  nBytes = 0;
  for (uint64_t iEntry = 0; iEntry < nEntries; iEntry++) {

    // grab entry
    const uint64_t bytes = ntInput -> GetEntry(iEntry);
    if (bytes < 0) {
      cerr << "WARNING: issue with entry " << iEntry << "! Aborting loop!" << endl;
      break;
    } else {
      nBytes += bytes;
    }

     announce progress
    const uint64_t iProg = iEntry + 1;
    if (iProg == nEntries) {
      cout << "      Processing entry " << iProg << "/" << nEntries << "..." << endl;
    } else {
      cout << "      Processing entry " << iProg << "/" << nEntries << "...\r" << flush;
    }

    // apply cuts
    const bool isTrkInDcaCutXY = (abs(dcaxy) < dcaWidthXY);
    const bool isTrkInDcaCutZ  = (abs(dcaz)  < dcaWidthZ);
    const bool isTrkInDcaCut   = (isTrkInDcaCutXY && isTrkInDcaCutZ);
    if (!isTrkInDcaCut) continue;

    // fill histograms
    arrDcaVsPtSel[0] -> Fill(pt, dcaxy);
    arrDcaVsPtSel[1] -> Fill(pt, dcaz);
    hDcaXyVsZSel     -> Fill(dcaz, dcaxy);

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

  // make plots ---------------------------------------------------------------

  // hist options
  const uint16_t fCol(1);
  const uint16_t fMar(20);
  const uint16_t fLin(1);
  const uint16_t fWid(1);
  const uint16_t fFil(0);
  const uint16_t fTxt(42);
  const uint16_t fCnt(1);
  const float    fOffX(1.1);
  const float    fOffY(1.3);
  const float    fTtl(0.04);
  const float    fLbl(0.03);

  // set dca-specific hist options
  for (size_t iDca = 0; iDca < NDca; iDca++) {

    // figure out dca vs. pt z-axis ranges
    const float minVsPtAll = arrDcaVsPtAll[iDca] -> GetMinimum(0.);
    const float minVsPtSel = arrDcaVsPtSel[iDca] -> GetMinimum(0.);
    const float maxVsPtAll = arrDcaVsPtAll[iDca] -> GetMaximum();
    const float maxVsPtSel = arrDcaVsPtSel[iDca] -> GetMaximum();
    const float minVsPtZ   = TMath::Min(minVsPtAll, minVsPtSel);
    const float maxVsPtZ   = TMath::Max(maxVsPtAll, maxVsPtSel);

    // set 1d hist options
    arrDcaWidth[iDca] -> SetMarkerColor(fCol);
    arrDcaWidth[iDca] -> SetMarkerStyle(fMar);
    arrDcaWidth[iDca] -> SetLineColor(fCol);
    arrDcaWidth[iDca] -> SetLineStyle(fLin);
    arrDcaWidth[iDca] -> SetLineWidth(fWid);
    arrDcaWidth[iDca] -> SetFillColor(fCol);
    arrDcaWidth[iDca] -> SetFillStyle(fFil);
    arrDcaWidth[iDca] -> SetTitleFont(fTxt);
    arrDcaWidth[iDca] -> SetTitle("");
    arrDcaWidth[iDca] -> GetXaxis() -> SetRangeUser(ptPlotRange.first, ptPlotRange.second);
    arrDcaWidth[iDca] -> GetXaxis() -> SetLabelSize(fLbl);
    arrDcaWidth[iDca] -> GetXaxis() -> SetTitle(sTitlePt.data());
    arrDcaWidth[iDca] -> GetXaxis() -> SetTitleFont(fTxt);
    arrDcaWidth[iDca] -> GetXaxis() -> SetTitleSize(fTtl);
    arrDcaWidth[iDca] -> GetXaxis() -> SetTitleOffset(fOffX);
    arrDcaWidth[iDca] -> GetXaxis() -> CenterTitle(fCnt);
    arrDcaWidth[iDca] -> GetYaxis() -> SetRangeUser(widthPlotRange.first, widthPlotRange.second);
    arrDcaWidth[iDca] -> GetYaxis() -> SetLabelSize(fLbl);
    arrDcaWidth[iDca] -> GetYaxis() -> SetTitle(sTitleWidth[iDca].data());
    arrDcaWidth[iDca] -> GetYaxis() -> SetTitleFont(fTxt);
    arrDcaWidth[iDca] -> GetYaxis() -> SetTitleSize(fTtl);
    arrDcaWidth[iDca] -> GetYaxis() -> SetTitleOffset(fOffY);
    arrDcaWidth[iDca] -> GetYaxis() -> CenterTitle(fCnt);

    // set dca vs. pt hist options
    arrDcaVsPtAll[iDca] -> SetMarkerColor(fCol);
    arrDcaVsPtAll[iDca] -> SetMarkerStyle(fMar);
    arrDcaVsPtAll[iDca] -> SetLineColor(fCol);
    arrDcaVsPtAll[iDca] -> SetLineStyle(fLin);
    arrDcaVsPtAll[iDca] -> SetLineWidth(fWid);
    arrDcaVsPtAll[iDca] -> SetFillColor(fCol);
    arrDcaVsPtAll[iDca] -> SetFillStyle(fFil);
    arrDcaVsPtAll[iDca] -> SetTitleFont(fTxt);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetRangeUser(ptPlotRange.first, ptPlotRange.second);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetLabelSize(fLbl);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetTitle(sTitlePt.data());
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetTitleFont(fTxt);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetTitleSize(fTtl);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> SetTitleOffset(fOffX);
    arrDcaVsPtAll[iDca] -> GetXaxis() -> CenterTitle(fCnt);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetLabelSize(fLbl);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetTitle(sTitleDca[iDca].data());
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetTitleFont(fTxt);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetTitleSize(fTtl);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> SetTitleOffset(fOffY);
    arrDcaVsPtAll[iDca] -> GetYaxis() -> CenterTitle(fCnt);
    arrDcaVsPtAll[iDca] -> GetZaxis() -> SetRangeUser(minVsPtZ, maxVsPtZ);
    arrDcaVsPtAll[iDca] -> GetZaxis() -> SetLabelSize(fLbl);
    arrDcaVsPtSel[iDca] -> SetMarkerColor(fCol);
    arrDcaVsPtSel[iDca] -> SetMarkerStyle(fMar);
    arrDcaVsPtSel[iDca] -> SetLineColor(fCol);
    arrDcaVsPtSel[iDca] -> SetLineStyle(fLin);
    arrDcaVsPtSel[iDca] -> SetLineWidth(fWid);
    arrDcaVsPtSel[iDca] -> SetFillColor(fCol);
    arrDcaVsPtSel[iDca] -> SetFillStyle(fFil);
    arrDcaVsPtSel[iDca] -> SetTitleFont(fTxt);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetRangeUser(ptPlotRange.first, ptPlotRange.second);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetLabelSize(fLbl);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetTitle(sTitlePt.data());
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetTitleFont(fTxt);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetTitleSize(fTtl);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> SetTitleOffset(fOffX);
    arrDcaVsPtSel[iDca] -> GetXaxis() -> CenterTitle(fCnt);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetLabelSize(fLbl);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetTitle(sTitleDca[iDca].data());
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetTitleFont(fTxt);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetTitleSize(fTtl);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> SetTitleOffset(fOffY);
    arrDcaVsPtSel[iDca] -> GetYaxis() -> CenterTitle(fCnt);
    arrDcaVsPtSel[iDca] -> GetZaxis() -> SetRangeUser(minVsPtZ, maxVsPtZ);
    arrDcaVsPtSel[iDca] -> GetZaxis() -> SetLabelSize(fLbl);
  }  // end variable loop

  // figure out dca xy vs. z z-axis ranges
  const float minVsDcaAll = hDcaXyVsZAll -> GetMinimum(0.);
  const float minVsDcaSel = hDcaXyVsZSel -> GetMinimum(0.);
  const float maxVsDcaAll = hDcaXyVsZAll -> GetMaximum();
  const float maxVsDcaSel = hDcaXyVsZSel -> GetMaximum();
  const float minVsDcaZ   = TMath::Min(minVsDcaAll, minVsDcaSel);
  const float maxVsDcaZ   = TMath::Max(maxVsDcaAll, maxVsDcaSel);

  // set dca xy vs. z hist options
  hDcaXyVsZAll -> SetMarkerColor(fCol);
  hDcaXyVsZAll -> SetMarkerStyle(fMar);
  hDcaXyVsZAll -> SetLineColor(fCol);
  hDcaXyVsZAll -> SetLineStyle(fLin);
  hDcaXyVsZAll -> SetLineWidth(fWid);
  hDcaXyVsZAll -> SetFillColor(fCol);
  hDcaXyVsZAll -> SetFillStyle(fFil);
  hDcaXyVsZAll -> SetTitleFont(fTxt);
  hDcaXyVsZAll -> GetXaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
  hDcaXyVsZAll -> GetXaxis() -> SetLabelSize(fLbl);
  hDcaXyVsZAll -> GetXaxis() -> SetTitle(sTitleDca[1].data());
  hDcaXyVsZAll -> GetXaxis() -> SetTitleFont(fTxt);
  hDcaXyVsZAll -> GetXaxis() -> SetTitleSize(fTtl);
  hDcaXyVsZAll -> GetXaxis() -> SetTitleOffset(fOffX);
  hDcaXyVsZAll -> GetXaxis() -> CenterTitle(fCnt);
  hDcaXyVsZAll -> GetYaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
  hDcaXyVsZAll -> GetYaxis() -> SetLabelSize(fLbl);
  hDcaXyVsZAll -> GetYaxis() -> SetTitle(sTitleDca[0].data());
  hDcaXyVsZAll -> GetYaxis() -> SetTitleFont(fTxt);
  hDcaXyVsZAll -> GetYaxis() -> SetTitleSize(fTtl);
  hDcaXyVsZAll -> GetYaxis() -> SetTitleOffset(fOffY);
  hDcaXyVsZAll -> GetYaxis() -> CenterTitle(fCnt);
  hDcaXyVsZAll -> GetZaxis() -> SetRangeUser(minVsDcaZ, maxVsDcaZ);
  hDcaXyVsZAll -> GetZaxis() -> SetLabelSize(fLbl);
  hDcaXyVsZSel -> SetMarkerColor(fCol);
  hDcaXyVsZSel -> SetMarkerStyle(fMar);
  hDcaXyVsZSel -> SetLineColor(fCol);
  hDcaXyVsZSel -> SetLineStyle(fLin);
  hDcaXyVsZSel -> SetLineWidth(fWid);
  hDcaXyVsZSel -> SetFillColor(fCol);
  hDcaXyVsZSel -> SetFillStyle(fFil);
  hDcaXyVsZSel -> SetTitleFont(fTxt);
  hDcaXyVsZSel -> GetXaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
  hDcaXyVsZSel -> GetXaxis() -> SetLabelSize(fLbl);
  hDcaXyVsZSel -> GetXaxis() -> SetTitle(sTitleDca[1].data());
  hDcaXyVsZSel -> GetXaxis() -> SetTitleFont(fTxt);
  hDcaXyVsZSel -> GetXaxis() -> SetTitleSize(fTtl);
  hDcaXyVsZSel -> GetXaxis() -> SetTitleOffset(fOffX);
  hDcaXyVsZSel -> GetXaxis() -> CenterTitle(fCnt);
  hDcaXyVsZSel -> GetYaxis() -> SetRangeUser(dcaPlotRange.first, dcaPlotRange.second);
  hDcaXyVsZSel -> GetYaxis() -> SetLabelSize(fLbl);
  hDcaXyVsZSel -> GetYaxis() -> SetTitle(sTitleDca[0].data());
  hDcaXyVsZSel -> GetYaxis() -> SetTitleFont(fTxt);
  hDcaXyVsZSel -> GetYaxis() -> SetTitleSize(fTtl);
  hDcaXyVsZSel -> GetYaxis() -> SetTitleOffset(fOffY);
  hDcaXyVsZSel -> GetYaxis() -> CenterTitle(fCnt);
  hDcaXyVsZSel -> GetZaxis() -> SetRangeUser(minVsDcaZ, maxVsDcaZ);
  hDcaXyVsZSel -> GetZaxis() -> SetLabelSize(fLbl);
  cout << "    Set histogram options." << endl;

  // text box options
  const uint16_t fAln      = 12;
  const uint16_t fColTxt   = 0;
  const uint16_t fFilTxt   = 0;
  const uint16_t fLinTxt   = 0;
  const size_t   nTxt      = sText.size();
  const float    yTxtStart = 0.1;
  const float    yTxtStop  = yTxtStart + (nTxt * 0.05);

  // text box dimensions
  const array<float, NVtx> xyTxt = {0.1, yTxtStart, 0.3, yTxtStop};

  // create text box
  TPaveText *ptInfo = new TPaveText(xyTxt[0], xyTxt[1], xyTxt[2], xyTxt[3], "NDC NB");
  ptInfo -> SetFillColor(fColTxt);
  ptInfo -> SetFillStyle(fFilTxt);
  ptInfo -> SetLineColor(fColTxt);
  ptInfo -> SetLineStyle(fLinTxt);
  ptInfo -> SetTextFont(fTxt);
  ptInfo -> SetTextAlign(fAln);
  for (const string text : sText) {
    ptInfo -> AddText(text.data());
  }
  cout << "    Made text box." << endl;

  // plot options
  const uint16_t fMode(0);
  const uint16_t fBord(2);
  const uint16_t fGrid(0);
  const uint16_t fTick(1);
  const uint16_t fLogZ(1);
  const uint16_t fFrame(0);
  const string   sPadAll("pAll");
  const string   sPadSel("pSelect");

  // plot margins
  const array<float, NVtx> xyMarginWidth = {0.02, 0.02, 0.15, 0.15};
  const array<float, NVtx> xyMarginPad   = {0.15, 0.15, 0.15, 0.25};

  // canvas & pad dimensions
  const pair<uint32_t, uint32_t> widthDim    = {750,  750};
  const pair<uint32_t, uint32_t> allVsSelDim = {1500, 750};
  const array<float, NVtx>       xyAllPad    = {0.0, 0.0, 0.5, 1.0};
  const array<float, NVtx>       xySelPad    = {0.5, 0.0, 1.0, 1.0};

  // make dca-specific plots
  array<TCanvas*, NDca> arrWidthPlots;
  array<TCanvas*, NDca> arrDcaVsPtPlots;
  array<TPad*,    NDca> arrDcaVsPtAllPad;
  array<TPad*,    NDca> arrDcaVsPtSelPad;
  for (size_t iDca = 0; iDca < NDca; iDca++) {

    // make width plots
    arrWidthPlots[iDca] = new TCanvas(sDcaWidthPlot[iDca].data(), "", widthDim.first, widthDim.second);
    arrWidthPlots[iDca] -> SetGrid(fGrid, fGrid);
    arrWidthPlots[iDca] -> SetTicks(fTick, fTick);
    arrWidthPlots[iDca] -> SetBorderMode(fMode);
    arrWidthPlots[iDca] -> SetBorderSize(fBord);
    arrWidthPlots[iDca] -> SetTopMargin(xyMarginWidth[0]);
    arrWidthPlots[iDca] -> SetRightMargin(xyMarginWidth[1]);
    arrWidthPlots[iDca] -> SetBottomMargin(xyMarginWidth[2]);
    arrWidthPlots[iDca] -> SetLeftMargin(xyMarginWidth[3]);
    arrWidthPlots[iDca] -> cd();
    arrDcaWidth[iDca]   -> Draw();
    ptInfo              -> Draw();
    fOutput             -> cd();
    arrWidthPlots[iDca] -> Write();
    arrWidthPlots[iDca] -> Close();

    // make dca vs. pt plots
    arrDcaVsPtPlots[iDca]  = new TCanvas(sDcaVsPtPlot[iDca].data(), "", allVsSelDim.first, allVsSelDim.second);
    arrDcaVsPtAllPad[iDca] = new TPad(sPadAll.data(), "", xyAllPad[0], xyAllPad[1], xyAllPad[2], xyAllPad[3]);
    arrDcaVsPtSelPad[iDca] = new TPad(sPadSel.data(), "", xySelPad[0], xySelPad[1], xySelPad[2], xySelPad[3]);
    arrDcaVsPtPlots[iDca]  -> SetGrid(fGrid, fGrid);
    arrDcaVsPtPlots[iDca]  -> SetTicks(fTick, fTick);
    arrDcaVsPtPlots[iDca]  -> SetBorderMode(fMode);
    arrDcaVsPtPlots[iDca]  -> SetBorderSize(fBord);
    arrDcaVsPtAllPad[iDca] -> SetGrid(fGrid, fGrid);
    arrDcaVsPtAllPad[iDca] -> SetTicks(fTick, fTick);
    arrDcaVsPtAllPad[iDca] -> SetBorderMode(fMode);
    arrDcaVsPtAllPad[iDca] -> SetBorderSize(fBord);
    arrDcaVsPtAllPad[iDca] -> SetLogz(fLogZ);
    arrDcaVsPtAllPad[iDca] -> SetTopMargin(xyMarginPad[0]);
    arrDcaVsPtAllPad[iDca] -> SetRightMargin(xyMarginPad[1]);
    arrDcaVsPtAllPad[iDca] -> SetBottomMargin(xyMarginPad[2]);
    arrDcaVsPtAllPad[iDca] -> SetLeftMargin(xyMarginPad[3]);
    arrDcaVsPtSelPad[iDca] -> SetGrid(fGrid, fGrid);
    arrDcaVsPtSelPad[iDca] -> SetTicks(fTick, fTick);
    arrDcaVsPtSelPad[iDca] -> SetBorderMode(fMode);
    arrDcaVsPtSelPad[iDca] -> SetBorderSize(fBord);
    arrDcaVsPtSelPad[iDca] -> SetLogz(fLogZ);
    arrDcaVsPtSelPad[iDca] -> SetTopMargin(xyMarginPad[0]);
    arrDcaVsPtSelPad[iDca] -> SetRightMargin(xyMarginPad[1]);
    arrDcaVsPtSelPad[iDca] -> SetBottomMargin(xyMarginPad[2]);
    arrDcaVsPtSelPad[iDca] -> SetLeftMargin(xyMarginPad[3]);
    arrDcaVsPtPlots[iDca]  -> cd();
    arrDcaVsPtAllPad[iDca] -> Draw();
    arrDcaVsPtSelPad[iDca] -> Draw();
    arrDcaVsPtAllPad[iDca] -> cd();
    arrDcaVsPtAll[iDca]    -> Draw("colz");
    arrFitsNeg[iDca]       -> Draw("same");
    arrFitsPos[iDca]       -> Draw("same");
    arrDcaVsPtSelPad[iDca] -> cd();
    arrDcaVsPtSel[iDca]    -> Draw("colz");
    ptInfo                 -> Draw();
    fOutput                -> cd();
    arrDcaVsPtPlots[iDca]  -> Write();
    arrDcaVsPtPlots[iDca]  -> Close();
  }  // end variable loop

  // make dca xy vs. z plot
  TCanvas* cDcaXyVsZ    = new TCanvas(sDcaXyVsZPlot.data(), "", allVsSelDim.first, allVsSelDim.second);
  TPad*    pDcaXyVsZAll = new TPad(sPadAll.data(), "", xyAllPad[0], xyAllPad[1], xyAllPad[2], xyAllPad[3]);
  TPad*    pDcaXyVsZSel = new TPad(sPadSel.data(), "", xySelPad[0], xySelPad[1], xySelPad[2], xySelPad[3]);
  cDcaXyVsZ    -> SetGrid(fGrid, fGrid);
  cDcaXyVsZ    -> SetTicks(fTick, fTick);
  cDcaXyVsZ    -> SetBorderMode(fMode);
  cDcaXyVsZ    -> SetBorderSize(fBord);
  pDcaXyVsZAll -> SetGrid(fGrid, fGrid);
  pDcaXyVsZAll -> SetTicks(fTick, fTick);
  pDcaXyVsZAll -> SetBorderMode(fMode);
  pDcaXyVsZAll -> SetBorderSize(fBord);
  pDcaXyVsZAll -> SetLogz(fLogZ);
  pDcaXyVsZAll -> SetTopMargin(xyMarginPad[0]);
  pDcaXyVsZAll -> SetRightMargin(xyMarginPad[1]);
  pDcaXyVsZAll -> SetBottomMargin(xyMarginPad[2]);
  pDcaXyVsZAll -> SetLeftMargin(xyMarginPad[3]);
  pDcaXyVsZSel -> SetGrid(fGrid, fGrid);
  pDcaXyVsZSel -> SetTicks(fTick, fTick);
  pDcaXyVsZSel -> SetBorderMode(fMode);
  pDcaXyVsZSel -> SetBorderSize(fBord);
  pDcaXyVsZSel -> SetLogz(fLogZ);
  pDcaXyVsZSel -> SetTopMargin(xyMarginPad[0]);
  pDcaXyVsZSel -> SetRightMargin(xyMarginPad[1]);
  pDcaXyVsZSel -> SetBottomMargin(xyMarginPad[2]);
  pDcaXyVsZSel -> SetLeftMargin(xyMarginPad[3]);
  cDcaXyVsZ    -> cd();
  pDcaXyVsZAll -> Draw();
  pDcaXyVsZSel -> Draw();
  pDcaXyVsZAll -> cd();
  hDcaXyVsZAll -> Draw("colz");
  pDcaXyVsZSel -> cd();
  hDcaXyVsZSel -> Draw("colz");
  ptInfo       -> Draw();
  fOutput      -> cd();
  cDcaXyVsZ    -> Write();
  cDcaXyVsZ    -> Close();
  cout << "    Made plots." << endl;

  // save output & close ------------------------------------------------------

  // save histograms
  fOutput -> cd();
  for (size_t iDca = 0; iDca < NDca; iDca++) {
    arrDcaWidth[iDca]   -> Write();
    arrWidthFits[iDca]  -> Write();
    arrDcaVsPtAll[iDca] -> Write();
    arrDcaVsPtSel[iDca] -> Write();
    arrFitsNeg[iDca]    -> Write();
    arrFitsPos[iDca]    -> Write();
  }
  hDcaXyVsZAll -> Write();
  hDcaXyVsZSel -> Write();

  // close flies
  fOutput -> cd();
  fOutput -> Close();
  fInput  -> cd();
  fInput  -> Close();
  cout << "  Finished with calculation!\n" << endl;

}

// end ------------------------------------------------------------------------

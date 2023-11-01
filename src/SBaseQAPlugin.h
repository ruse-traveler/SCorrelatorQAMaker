// ----------------------------------------------------------------------------
// 'SBaseQAPlugin.h'
// Derek Anderson
// 11.01.2023
//
// Base class for subroutines ("plugins") to be run by the SCorrelatorQAMaker
// module.
// ----------------------------------------------------------------------------

#ifndef SBASEQAPLUGIN_H
#define SBASEQAPLUGIN_H

// c++ utilities
#include <string>
#include <cassert>
// root libraries
#include <TFile.h>
#include <TSystem.h>
#include <TDirectory.h>

namespace SColdQcdCorrelatorAnalysis {

  // SBaseQAPlugin definition -------------------------------------------------

  class SBaseQAPlugin : public SubsysReco{

    public:

      // ctor/dtor
      SBaseQAPlugin()  {};
      ~SBaseQAPlugin() {};

      // F4A methods
      int Init(PHCompositeNode*)          override;
      int process_event(PHCompositeNode*) override;
      int End(PHCompositeNode*)           override;

      // setters
      void SetOutFile(const string out) {m_outFileName = out;}
      void SetOutDir(const string out)  {m_outDirName = out;}

      // getters
      TFile*      GetOutFile() {return m_outFile;}
      TDirectory* GetOutDir()  {return m_outDir;}

    private:

      void InitOutput() {

        // check output file and create if needed
        const bool doesFileExist = gSystem -> AccessPathName(m_outFileName.data());
        if (!m_outFile) {
          m_outFile = new TFile(m_outFileName.data(), "create");
        }

        // create output directory if needed
        const bool doesDirExist = m_outFile -> cd(m_outDirName.data());
        if (!doesDirExist) {
          m_outDir = (TDirectory*) m_outFile -> mkdir(m_outDirName.data());
        } else {
          m_outDir = (TDirecoty*) m_outFile -> GetDirectory(m_outDirName.data());
        }
        return;
      };  // end 'InitOutput()'

      // io members
      TFile*      m_outFile = NULL;
      TDirectory* m_outDir  = NULL;

      // atomic members
      string m_outFileName = "";
      string m_outDirName  = "";

  };  // end SBaseQAPlugin Definition

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

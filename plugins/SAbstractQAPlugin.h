// ----------------------------------------------------------------------------
// 'SAbstractQAPlugin.h'
// Derek Anderson
// 11.01.2023
//
// Base class for subroutines ("plugins") to be run by the SCorrelatorQAMaker
// module.
// ----------------------------------------------------------------------------

#ifndef SABSTRACTQAPLUGIN_H
#define SABSTRACTQAPLUGIN_H

// c++ utilities
#include <string>
// root libraries
#include <TFile.h>
#include <TSystem.h>
#include <TDirectory.h>

namespace SColdQcdCorrelatorAnalysis {

  // SBaseQAPluginConfig definition ---------------------------------------

  struct SAbstractQAPluginConfig {

    virtual ~SAbstractQAPluginConfig {};

  };  // end SBaseQAPluginConfig



  // SAbstractQAPlugin definition ---------------------------------------------

  class SAbstractQAPlugin

    public:

      // ctor/dtor
      SAbstractQAPlugin()  {};
      ~SAbstractQAPlugin() {};

      // output setters
      void SetOutFile(const string out) {m_outFileName = out;}
      void SetOutDir(const string out)  {m_outDirName = out;}

      // output getters
      TFile*      GetOutFile() {return m_outFile;}
      TDirectory* GetOutDir()  {return m_outDir;}

      // pure virtual set configuration
      virtual SetConfig() = 0;

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
          m_outDir = (TDirectory*) m_outFile -> GetDirectory(m_outDirName.data());
        }
        return;
      };  // end 'InitOutput()'

      // io members
      TFile*      m_outFile = NULL;
      TDirectory* m_outDir  = NULL;

      // atomic members
      string m_outFileName = "";
      string m_outDirName  = "";

  };  // end SAbstractQAPlugin Definition

}  // end SColdQcdCorrelatorAnalysis namespace

#endif

// end ------------------------------------------------------------------------

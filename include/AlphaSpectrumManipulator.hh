#ifndef _ALPHA_SPECTRUM_MANIPULATOR_HH_
#define _ALPHA_SPECTRUM_MANIPULATOR_HH_

// Abstract base class for manipulating alpha spectra
// Essentially contains the file input and output functions, which is reused by multiple classes
// Also contains a progress bar should it be needed

#include "ProgressBar.hh"

#include <TFile.h>
#include <TString.h>

#include <iostream>

class AlphaSpectrumManipulator{
	protected:
		TFile *fInputFile;
		TFile *fOutputFile;
		ProgressBar *fProgressBar;
		void SetFile( TString file, TFile*& f, TString file_open_state );	// Base function for SetInput and SetOutput file functions
		void CloseFile( TFile*& f );	// Base function for CloseInput and CloseOutput functions
		
		AlphaSpectrumManipulator();
		virtual ~AlphaSpectrumManipulator();
		
	public:
		virtual void SetInputFile( TString file );
		virtual void SetOutputFile( TString file );
		virtual void CloseInput();
		virtual void CloseOutput();

};
#endif

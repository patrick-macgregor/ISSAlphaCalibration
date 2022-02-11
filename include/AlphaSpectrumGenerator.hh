#ifndef _ALPHA_SPECTRUM_GENERATOR_HH_
#define _ALPHA_SPECTRUM_GENERATOR_HH_

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrum.hh"
#include "AlphaSpectrumManipulator.hh"

#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TString.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include <iostream>

// Generates a number of alpha spectra which are stored as their own special class in a TFile

class AlphaSpectrumGenerator : public AlphaSpectrumManipulator{
	private:
		std::vector < std::vector < std::vector < AlphaSpectrum* > > > fAlphaSpectrumVector;
		
		void Initialise();
		template <typename T>	// Template needed as different types of tree might be needed here
		bool CheckTTreeReaderArray( TTreeReaderArray<T>& arr, T lb, T ub );	// Bounds used to check limits placed on quantities it reads in

	public:
		AlphaSpectrumGenerator();
		virtual ~AlphaSpectrumGenerator();
		
		void PopulateSpectra();
		void LabelEmptySpectra();	// Works through populated spectra and labels ones with no data
		void WriteSpectraToFile();
		
	ClassDef( AlphaSpectrumGenerator, 1 );
};





#endif

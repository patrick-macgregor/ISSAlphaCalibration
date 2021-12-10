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


class AlphaSpectrumGenerator : public AlphaSpectrumManipulator{
	private:
		std::vector < std::vector < std::vector < AlphaSpectrum* > > > fAlphaSpectrumVector;
		
		void Initialise();
		template <typename T>
		bool CheckTTreeReaderArray( TTreeReaderArray<T>& arr, T lb, T ub );

	public:
		AlphaSpectrumGenerator();
		virtual ~AlphaSpectrumGenerator();
		
		void PopulateSpectra();
		void LabelEmptySpectra();
		void WriteSpectraToFile();
		
	ClassDef( AlphaSpectrumGenerator, 1 );
};





#endif

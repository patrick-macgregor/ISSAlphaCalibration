#ifndef _ALPHA_SPECTRUM_FITTER_HH_
#define _ALPHA_SPECTRUM_FITTER_HH_

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrum.hh"
#include "AlphaSpectrumManipulator.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TList.h>

#include <iostream>

class AlphaSpectrumFitter : public AlphaSpectrumManipulator{
	private:
		std::vector <AlphaSpectrum*> fAlphaSpectrumVector;
		
	public:
		AlphaSpectrumFitter();
		~AlphaSpectrumFitter();
		
		void GetSpectra();
		void FindPeaks();
		void SortPeaks();
		void DiscardBadPeaks();
		void GenerateFits();
		void FitPeaks();
		void WriteFitsToFile();
		void WriteFitsToImage();

	ClassDef( AlphaSpectrumFitter, 1 );
};

#endif

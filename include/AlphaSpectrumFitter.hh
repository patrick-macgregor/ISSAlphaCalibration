#ifndef _ALPHA_SPECTRUM_FITTER_HH_
#define _ALPHA_SPECTRUM_FITTER_HH_

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrum.hh"
#include "AlphaSpectrumManipulator.hh"
#include "CrystalBall.hh"

#include <TCanvas.h>
#include <TColor.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TList.h>
#include <TSystem.h>

#include <iostream>

class AlphaSpectrumFitter : public AlphaSpectrumManipulator{
	private:
		std::vector <AlphaSpectrum*> fAlphaSpectrumVector;
		
		void FitGaussian(AlphaSpectrum* a);			// Called during fitting process
		void FitCrystalBall( AlphaSpectrum* a );	// Called during fitting process
		
	public:
		AlphaSpectrumFitter();
		~AlphaSpectrumFitter();
		
		void GetSpectra();
		void FindPeaks();
		void FitPeaks();
		void WriteFitsToFile();
		void WriteFitsToImage();

	ClassDef( AlphaSpectrumFitter, 1 );
};

#endif

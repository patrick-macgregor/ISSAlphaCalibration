#ifndef _CALIBRATION_CREATOR_H_
#define _CALIBRATION_CREATOR_H_

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrum.hh"
#include "AlphaSpectrumManipulator.hh"

#include <TMath.h>
#include <TString.h>

#include <fstream>
#include <iostream>
#include <vector>

class CalibrationCreator : public AlphaSpectrumManipulator{
	private:
		std::vector <AlphaSpectrum*> fAlphaSpectrumVector;
		std::ofstream fOutputFileDat;
		
		void GetOffsetAndGainFromSpectrum( AlphaSpectrum* a, double &quadgain, double &gain, double &offset );	// Called during WriteCalibration
		TString GetCalibrationParameters( AlphaSpectrum* a, double &quadgain, double &gain, double &offset );	// Called during WriteCalibration
	
	public:
		CalibrationCreator();
		~CalibrationCreator();
		void GetSpectra();			// Collects the spectra from the file and stores them in fAlphaSpectrumVector
		void WriteCalibration();	// Writes the calibration to the dat file
		void SetOutputFile( TString file ) override;	// Overrides the base class function as writing to dat
		void CloseOutput() override;	// Overrides the base class function as writing to dat
		
	ClassDef( CalibrationCreator, 1 );
};

#endif

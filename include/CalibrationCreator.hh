#ifndef _CALIBRATION_CREATOR_H_
#define _CALIBRATION_CREATOR_H_

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrum.hh"
#include "AlphaSpectrumManipulator.hh"

#include <TMath.h>
#include <TString.h>

#include <iostream>
#include <vector>

class CalibrationCreator : public AlphaSpectrumManipulator{
	private:
		std::vector <AlphaSpectrum*> fAlphaSpectrumVector;
		std::ofstream fOutputFileDat;	//TODO
		
		void GetOffsetAndGainFromPeaks( AlphaSpectrum* a, double &quadgain, double &gain, double &offset );
		TString GetCalibrationLine( AlphaSpectrum* a, double &quadgain, double &gain, double &offset );
		
	
	public:
		CalibrationCreator();
		~CalibrationCreator();
		void GetSpectra();
		void WriteCalibration();
		void SetOutputFile( TString file );
		
	ClassDef( CalibrationCreator, 1 );
};

#endif

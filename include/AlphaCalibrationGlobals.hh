#ifndef _ALPHA_CALIBRATION_GLOBALS_HH_
#define _ALPHA_CALIBRATION_GLOBALS_HH_

#include "ProgressBar.hh"

#include <TError.h>
#include <TROOT.h>
#include <TString.h>

#include <vector>

// Put global variables in here to use
// ISS Array Info
const int NumberOfArrayModules = 3;
const int NumberOfArrayASICs = 6;
const int NumberOfPArrayChannels = 128;

// Alpha source particulars
const int gNumberOfAlphaPeaks = 4;
const int gAlphaPeakWidthEstimate = 30; 	// Rough estimate of peak width in channels (so you don't select local maxima) - best if even!
const int gAlphaPeakChannelCheckLimit = 4;	// How far to look either side of a given peak to check it's legit (in channels)

// HISTOGRAM THINGS
const int gAlphaHistNumBins = 275;
const int gAlphaHistLB = 150;
const int gAlphaHistUB = 700;

// FITTING PARAMETERS
const double gPeakHeightThresholdFraction = 0.15;	// The smallest fraction that is allowed as a peak
const double gPeakHeightDipFraction = 0.6;			// Ensure that the peak has dipped to this fraction of its height before being ready to record another peak --> this might be too much if you have multiplets...
const int gPeakChannelLowerThreshold = 350;			// Set to 0 if you want to sample the whole range of the histograms

// FUNCTIONS
TString SpectrumNameGenerator( unsigned char mod, unsigned char asic, unsigned char ch );
TString SpectrumTitleGenerator( unsigned char mod, unsigned char asic, unsigned char ch );
void InitialiseGlobalSettings();

// CALIBRATION
// Nuclides used here are 148Gd, 239Pu, 241Am, 244Cm. Energies listed in order of energy (keV).
// Energies taken from NNDC in 2022 --> just the strongest peaks at the moment
const double gAlphaParticleEnergy[gNumberOfAlphaPeaks] = {
	3182.690,	// 100    % 148Gd
	5156.59,	//  70.77 % 239Pu
	5485.56,	//  84.8  % 241Am
	5804.77 	//  76.90 % 244Cm
};

const bool gQuadraticAlphaCalibration = 0;	// 1 if quadratic, 0 if linear


#endif

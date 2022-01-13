#ifndef _ALPHA_CALIBRATION_GLOBALS_HH_
#define _ALPHA_CALIBRATION_GLOBALS_HH_

#include <TString.h>
#include <vector>

// Put global variables in here to use
// ISS Array Info
const int NumberOfArrayModules = 3;
const int NumberOfArrayASICs = 6;
//const bool ASICIsPSide[NumberOfArrayASICs] = { 1, 0, 1, 1, 0, 1 };
const int NumberOfPArrayChannels = 128;
//const int NumberOfNArrayChannels = 11;
//const int NumberOfNPerASIC = 4;
//const int NSideChannelStart[NumberOfNPerASIC] = { 11, 28, 89, 106 };

// Alpha source particulars
const int gNumberOfAlphaPeaks = 4;
const int gAlphaPeakWidthEstimate = 30; // Rough estimate of peak width in channels (so you don't select local maxima) - best if even!


// HISTOGRAM FUNCTIONS
TString SpectrumNameGenerator( unsigned char mod, unsigned char asic, unsigned char ch );
TString SpectrumTitleGenerator( unsigned char mod, unsigned char asic, unsigned char ch );

const int gAlphaHistNumBins = 250;
const int gAlphaHistLB = 150;
const int gAlphaHistUB = 650;

#endif

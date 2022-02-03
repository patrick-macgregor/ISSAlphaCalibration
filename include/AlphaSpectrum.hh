#ifndef _ALPHA_SPECTRUM_HH_
#define _ALPHA_SPECTRUM_HH_

#include "AlphaCalibrationGlobals.hh"

#include <TH1F.h>
#include <TNamed.h>
#include <TPolyLine.h>

#include <iostream>

class AlphaSpectrum : public TNamed{
	private:
		// SPECTRUM INFO
		TH1F* fHist;				// Contains the histogram showing the original alpha data
		unsigned int fChannel;		// Easy access integers for the channel, ...
		unsigned int fModule;		// ... module, ...
		unsigned int fAsic;			// ... and ASIC.
		bool fIsEmpty;				// Boolean that alerts the generator if a given spectrum is empty or not
		
		// FITTING STUFF
		std::vector<int> fPeakLocations;
		
		// Initialisation
		void Initialise();
		
	public:
		AlphaSpectrum();
		AlphaSpectrum( int mod, int asic, int ch );
		virtual ~AlphaSpectrum();
	
		inline void SetChannel( int a ){ fChannel = a; }
		inline void SetModule( int a ){ fModule = a; }
		inline void SetAsic( int a ){ fAsic = a; }
		void AddPeakLocation( int peak_location );
		
		inline unsigned int GetChannel(){ return fChannel; }
		inline unsigned int GetModule(){ return fModule; }
		inline unsigned int GetAsic(){ return fAsic; }
		inline TH1F* GetHist(){ return fHist; }
		inline unsigned int GetNumberOfPeaks(){ return fPeakLocations.size(); };
		
		inline int GetPeakChannel( int i ){ return fPeakLocations[i]; }
		
		void CreateHistogram();
		void CheckIsEmpty();
		inline void FillHistogram( double a ){ fHist->Fill(a); }
		//inline double GetEntries(){ return fHist->GetEntries(); }
		inline bool IsEmpty(){ return fIsEmpty; }
		
		void Draw();
		
	ClassDef( AlphaSpectrum, 1 );
};

#endif

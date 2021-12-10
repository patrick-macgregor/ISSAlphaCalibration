#ifndef _ALPHA_SPECTRUM_HH_
#define _ALPHA_SPECTRUM_HH_

#include "AlphaCalibrationGlobals.hh"

#include <TH1F.h>
#include <TNamed.h>

#include <iostream>

class AlphaSpectrum : public TNamed{
	private:
		TH1F* fHist;
		unsigned int fChannel;
		unsigned int fModule;
		unsigned int fAsic;
		bool fIsEmpty;
		
	public:
		AlphaSpectrum();
		AlphaSpectrum( int mod, int asic, int ch );
		virtual ~AlphaSpectrum();
	
		inline void SetChannel( int a ){ fChannel = a; }
		inline void SetModule( int a ){ fModule = a; }
		inline void SetAsic( int a ){ fAsic = a; }
		
		inline unsigned int GetChannel(){ return fChannel; }
		inline unsigned int GetModule(){ return fModule; }
		inline unsigned int GetAsic(){ return fAsic; }
		inline TH1F* GetHist(){ return fHist; }
		
		void CreateHistogram();
		void CheckIsEmpty();
		inline void FillHistogram( double a ){ fHist->Fill(a); }
		//inline double GetEntries(){ return fHist->GetEntries(); }
		inline bool IsEmpty(){ return fIsEmpty; }
		
	ClassDef( AlphaSpectrum, 1 );
};

#endif

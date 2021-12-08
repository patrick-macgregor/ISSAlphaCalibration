#ifndef _ALPHA_SPECTRUM_HH_
#define _ALPHA_SPECTRUM_HH_

#include "AlphaCalibrationGlobals.hh"

#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TString.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include <iostream>

class AlphaSpectrum{
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
		
		void CreateHistogram();
		void CheckIsEmpty();
		inline void Fill( double a ){ fHist->Fill(a); }
		inline double GetEntries(){ return fHist->GetEntries(); }
		inline bool IsEmpty(){ return fIsEmpty; }
		inline void Write(){ fHist->Write(); }
		
	ClassDef( AlphaSpectrum, 1 );
};
// ============================================================================================= //
class AlphaSpectrumGenerator{
	private:
		std::vector < std::vector < std::vector < AlphaSpectrum* > > > fAlphaSpectrumVector;
		TFile *fInputFile;
		TFile *fOutputFile;

		void SetFile( TString file, TFile*& f, TString file_open_state );
		template <typename T>
		bool CheckTTreeReaderArray( TTreeReaderArray<T>& arr, T lb, T ub );

	public:
		AlphaSpectrumGenerator();
		virtual ~AlphaSpectrumGenerator();
		
		void Initialise();
		void SetInputFile( TString file );
		void SetOutputFile( TString file );
		void PopulateSpectra();
		void LabelEmptySpectra();
		void WriteSpectraToFile();
		void CloseOutput();
		
	ClassDef( AlphaSpectrumGenerator, 1 );
};





#endif

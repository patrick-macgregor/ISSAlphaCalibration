// Combines a series of files to make a TChain and writes this to a file for ease of access

#ifndef _CHAINMAKER_HH_
#define _CHAINMAKER_HH_

#include <vector>
#include <iostream>

#include <TChain.h>
#include <TFile.h>
#include <TString.h>



class ChainMaker{
	private:
		std::vector <TString> fInputFileNameVector;
		TFile *fInputFile;
		TFile *fOutputFile;
		TChain *fChain;
		
		void Initialise();

	public:
		ChainMaker();
		ChainMaker( int argc, char* argv[] );
		virtual ~ChainMaker();
		
		void AddFile( TString file );
		void SetOutputFile( TString file );
		void MakeChain();
		void WriteChain();
		inline void CloseOutput(){ fOutputFile->Close(); }
		
	ClassDef( ChainMaker, 1 );
};

#endif

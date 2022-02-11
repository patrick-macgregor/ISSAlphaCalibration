#ifndef _CHAINMAKER_HH_
#define _CHAINMAKER_HH_

#include <vector>
#include <iostream>

#include <TChain.h>
#include <TFile.h>
#include <TString.h>

// Combines a series of files to make a TChain and writes this to a file for ease of access

class ChainMaker{
	private:
		std::vector <TString> fInputFileNameVector;	// Contains all the root file names that contain the data
		TFile *fInputFile;		// Input file
		TFile *fOutputFile;		// Output file
		TChain *fChain;			// Contains the TChain
		
		void Initialise();

	public:
		ChainMaker();
		ChainMaker( int argc, char* argv[] );
		virtual ~ChainMaker();
		
		void AddFile( TString file );
		void SetOutputFile( TString file );
		void MakeChain();	// Creates a chain based on the input files
		void WriteChain();	// Writes the chain to an output file
		inline void CloseOutput(){ fOutputFile->Close(); }
		
	ClassDef( ChainMaker, 1 );
};

#endif

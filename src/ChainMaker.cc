#include "ChainMaker.hh"

// --------------------------------------------------------------------------------------------- //
void ChainMaker::Initialise(){
	fInputFile = NULL;
	fOutputFile = NULL;
	fChain = NULL;
	return;
}
// --------------------------------------------------------------------------------------------- //
ChainMaker::ChainMaker(){ 
	Initialise();
}
// --------------------------------------------------------------------------------------------- //
ChainMaker::ChainMaker( int argc, char* argv[] ){
	Initialise();
	for ( int i = 1; i < argc; i++ ){
		AddFile( argv[i] );
	}
}
// --------------------------------------------------------------------------------------------- //
ChainMaker::~ChainMaker(){};
// --------------------------------------------------------------------------------------------- //
void ChainMaker::AddFile( TString file ){
	fInputFile = new TFile( file.Data(), "READ" );
	if ( fInputFile->IsOpen() ){
		fInputFileNameVector.push_back( file );
		fInputFile->Close();
	}
	else{
		std::cerr << "File not found: " << file << std::endl;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void ChainMaker::MakeChain(){
	// Make new chain if unassigned
	if ( fChain == NULL ){
		fChain = new TChain("iss");
	}
	// Delete old chain if already assigned
	else if ( fChain->IsOnHeap() ){
		fChain->Delete();
		fChain = new TChain("iss");
	}

	// Now add all the files to the chain
	for ( unsigned int i = 0; i < fInputFileNameVector.size(); i++ ){
		fChain->Add( fInputFileNameVector[i] );
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void ChainMaker::SetOutputFile( TString file ){
	fOutputFile = new TFile( file, "RECREATE" );
	if ( !fOutputFile->IsOpen() ){
		std::cerr << "TChain output file failed to open." << std::endl;
	}
}
// --------------------------------------------------------------------------------------------- //
void ChainMaker::WriteChain(){
	if ( fOutputFile!= NULL && fOutputFile->IsOpen() ){
		fOutputFile->cd();
		fChain->Write();
	}
	return;
}

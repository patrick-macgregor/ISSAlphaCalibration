#include "AlphaSpectrumManipulator.hh"

AlphaSpectrumManipulator::AlphaSpectrumManipulator(){
	fInputFile = NULL;
	fOutputFile = NULL;
	fProgressBar = NULL;
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumManipulator::~AlphaSpectrumManipulator(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::SetFile( TString file, TFile*& f, TString file_open_state ){
	f = new TFile( file, file_open_state );
	if ( f->IsOpen() == 0 ){
		std::cerr << "Alpha spectrum file " << file << " failed to open." << std::endl;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::SetInputFile( TString file ){
	SetFile( file, fInputFile, "READ" );
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::SetOutputFile(  TString file  ){
	SetFile( file, fOutputFile, "RECREATE" );
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::CloseFile( TFile*& f ){
	if ( f->IsOpen() ){
		f->Close();
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::CloseInput(){
	CloseFile( fInputFile );
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumManipulator::CloseOutput(){
	CloseFile( fOutputFile );
	return;
}

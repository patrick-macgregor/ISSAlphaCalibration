#include "AlphaSpectrumFitter.hh"

AlphaSpectrumFitter::AlphaSpectrumFitter(){}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumFitter::~AlphaSpectrumFitter(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::GetSpectra(){
	if ( fInputFile == NULL ){
		std::cerr << "Histogram input file not assigned!" << std::endl;
	}
	
	// Get the list of keys
	TList *l = fInputFile->GetListOfKeys();
	TString obj_name;
	
	// Loop and acquire alpha spectra
	for ( int i = 0; i < l->GetEntries(); i++ ){
		// Assume that the alpha spectrum is named with "alpha_spectrum" in its name
		obj_name =  l->At(i)->GetName();
		if ( obj_name.Contains( "alpha_spectrum" ) ){
			fAlphaSpectrumVector.push_back( (AlphaSpectrum*)fInputFile->Get( obj_name.Data() ) );
		}
	}
	
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::FitSpectra(){
	std::cout << "This is where the magic happens!" << std::endl;
	return;
}
// --------------------------------------------------------------------------------------------- //
void WriteFitsToFile(){}

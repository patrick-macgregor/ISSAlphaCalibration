#include "AlphaCalibrationGlobals.hh"

// FUNCTIONS
TString SpectrumNameGenerator( unsigned char mod, unsigned char asic, unsigned char ch ){
	return Form( "alpha_spectrum_%i_%i_%i", mod, asic, ch );
}
TString SpectrumTitleGenerator( unsigned char mod, unsigned char asic, unsigned char ch ){
	return Form( "Mod. %i | Asic %i | Ch. %i", mod, asic, ch );
}

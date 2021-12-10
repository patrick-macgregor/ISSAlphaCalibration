#include "AlphaSpectrum.hh"

AlphaSpectrum::AlphaSpectrum(){
	fChannel = 0;
	fModule = 0;
	fAsic = 0;
	fIsEmpty = 0;
	fHist = NULL;
	this->SetName("");
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::AlphaSpectrum( int mod, int asic, int ch ){
	fChannel = ch;
	fModule = mod;
	fAsic = asic;
	fIsEmpty = 0;
	fHist = NULL;
	CreateHistogram();
	this->SetName( SpectrumNameGenerator( mod, asic, ch ) );
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::~AlphaSpectrum(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrum::CreateHistogram(){
	bool new_hist_needed = 0;
	if ( fHist == NULL ){
		new_hist_needed = 1;
	}
	else if ( fHist->IsOnHeap() ){
		fHist->Delete();
		std::cout << "Deleted old histogram" << std::endl;
		new_hist_needed = 1;
	}

	if ( new_hist_needed ){
		fHist = new TH1F(
			"h_" + SpectrumNameGenerator( fModule, fAsic, fChannel ),
			SpectrumTitleGenerator( fModule, fAsic, fChannel ),
			gAlphaHistNumBins,
			gAlphaHistLB,
			gAlphaHistUB
		);
		
		// TODO histogram formatting here
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrum::CheckIsEmpty(){
	if ( fHist->GetEntries() == 0 ){
		fIsEmpty = 1;
	}
	else{
		fIsEmpty = 0;
	}
	return;
}

#include "AlphaSpectrum.hh"

AlphaSpectrum::AlphaSpectrum(){
	fChannel = 0;
	fModule = 0;
	fAsic = 0;
	Initialise();
	this->SetName("");
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::AlphaSpectrum( int mod, int asic, int ch ){
	fChannel = ch;
	fModule = mod;
	fAsic = asic;
	Initialise();
	CreateHistogram();
	this->SetName( SpectrumNameGenerator( mod, asic, ch ) );
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::~AlphaSpectrum(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrum::Initialise(){
	fIsEmpty = 0;
	fHist = NULL;
	/*fPeakLocations.resize( gNumberOfAlphaPeaks );
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		fPeakLocations[i] = -1;
	}*/
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrum::AddPeakLocation( int peak_location ){
	fPeakLocations.push_back( peak_location );
	return;
}
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
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrum::Draw(){
	// Draw the histograms
	fHist->Draw();
	
	// Add peak locations as triangles marking their locations
	if ( fPeakLocations.size() > 0 ){
		// Work out quantities specific to each histogram
		double triangle_height = fHist->GetMaximum()*0.04;
		double triangle_width = ( ( gAlphaHistUB - gAlphaHistLB )/gAlphaHistNumBins )*2;
		
		
		double x[3] = {0,0,0};
		double y[3] = {0,0,0};
		
		TPolyLine *p[fPeakLocations.size()];

		for ( unsigned int i = 0; i < fPeakLocations.size(); i++ ){
			int y_base = fHist->GetBinContent( fHist->FindBin( fPeakLocations[i] ) ) + 0.25*triangle_height;
			for ( int j = 0; j < 3; j++ ){
				x[j] = fPeakLocations[i] + ( j-1 )*triangle_width;
				y[j] = y_base + TMath::Abs( (j-1) )*triangle_height;
			}
			p[i] = new TPolyLine(3,x,y);
			p[i]->SetFillColor( kRed );
			p[i]->Draw("F SAME");
		}
	}
	return;
}






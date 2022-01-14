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
void AlphaSpectrumFitter::FindPeaks(){
	TH1F *h;
	std::vector<std::vector<int>> peak_info;	// First index is the height, second index is the channel
	std::vector<int> individual_peak;
	individual_peak.resize(2);
	
	// Loop over vector
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		// Get the histogram
		h = fAlphaSpectrumVector[i]->GetHist();
		
		// Reset peak estimates
		individual_peak[0] = 0;
		individual_peak[1] = 0;
		
		peak_info.resize(0);
		
		// Get info about the histogram
		int max = h->GetMaximum();
		int threshold = max*0.2;
		
		// Loop over each bin
		for ( int j = 0; j < h->GetNbinsX(); j++ ){
			// Check to see if the current bin is over the threshold
			if ( h->GetBinContent(j) > threshold ){
				// Now check if bins either side <= current bin content 
				// (shouldn't have peak on edge of spectrum, but may need to code that in later...)
				if ( h->GetBinContent(j-1) <= h->GetBinContent(j) && h->GetBinContent(j+1) <= h->GetBinContent(j) ){
					// Store those that pass this process
					individual_peak[0] = h->GetBinContent(j);
					individual_peak[1] = h->GetBinCenter(j);
					peak_info.push_back( individual_peak );
				}
			}
		}
		
		// Now deal with all the competitors -> set bad ones to zero
		for ( unsigned int j = 1; j < peak_info.size(); j++ ){
			for ( unsigned int k = 0; k < j; k++ ){
				// First, test that both of the test peaks have positive heights
				if ( peak_info[j][0] > 0 && peak_info[k][0] > 0 ){
					// Second, test that the peaks are within range of each other
					if ( TMath::Abs( peak_info[j][1] - peak_info[k][1] ) < 0.5*gAlphaPeakWidthEstimate ){
						// Third, set the smaller one to have negative height (of its measured height for debugging)
						if ( peak_info[j][0] > peak_info[k][0] ){
							peak_info[k][0] = -peak_info[k][0];
						}
						else{
							peak_info[j][0] = -peak_info[j][0];
						}
					}
				}
			}
		}
		
		// Delete negative entries
		for ( unsigned int j = 0; j < peak_info.size(); j++ ){
			if ( peak_info[j][0] < 0 ){
				peak_info.erase( peak_info.begin() + j );
				j--;
			}
		}
		
		// Store the peak info into the alpha spectrum
		for ( unsigned int j = 0; j < peak_info.size(); j++ ){
			fAlphaSpectrumVector[i]->AddPeakLocation( peak_info[j][1] );
		}
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::SortPeaks(){
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::DiscardBadPeaks(){
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::GenerateFits(){
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::FitPeaks(){
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::WriteFitsToFile(){
	fOutputFile->cd();
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		fOutputFile->WriteTObject( fAlphaSpectrumVector[i] );
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::WriteFitsToImage(){
	TCanvas *c = new TCanvas( "CANVAS", "CANVAS", 800, 600 );
	c->cd();
	int num = 0;
	fProgressBar = new ProgressBar( fAlphaSpectrumVector.size() );
	
	std::cout << "Printing images..." << std::endl;
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		fAlphaSpectrumVector[i]->Draw();
		num = fAlphaSpectrumVector[i]->GetNumberOfPeaks();
		TString file_name = "images/" + ( num > 0 && num < 10 ? "0" + (TString)Form( "%i", num ) + "/" : "" )+ (TString)(fAlphaSpectrumVector[i]->GetName()) + "__" + num + ".png";
		c->Print( file_name );
		
		// Check Progress
		fProgressBar->UpdateProgress(i);
	}

	return;
}







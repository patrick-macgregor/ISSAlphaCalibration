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
	std::vector<std::vector<int>> peak_info;	// Index corresponds to individual peaks
	std::vector<int> individual_peak;			// First entry is the height, second entry is the channel
	individual_peak.resize(2);
	
	// Loop over vector
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		// Get the histogram
		h = fAlphaSpectrumVector[i]->GetHist();
		
		// Reset peak estimates
		individual_peak[0] = 0;
		individual_peak[1] = 0;
		
		peak_info.resize(0);
		
		// Get info about the histogram -> find the maximum value in the desired region
		int max = 0;
		for ( int j = 0; j < h->GetNbinsX(); j++ ){
			if ( h->GetBinLowEdge(j) > gPeakChannelLowerThreshold ){
				if ( h->GetBinContent(j) > max ){
					max = h->GetBinContent(j);
				}
			}
		}
		
		int threshold = max*gPeakHeightThresholdFraction;	// Count threshold to stop accepting small peaks
		bool b_record_peaks = 1;		// Stores whether a peak has dipped enough to count as a peak
		int current_peak_height = 0;	// Stores the current height of a peak
		
		// Loop over each bin
		for ( int j = 0; j < h->GetNbinsX(); j++ ){
			// Check to see if the histogram has "dipped" enough to define a peak ( or is now much larger...)
			if ( b_record_peaks == 0 ){
				if ( h->GetBinContent(j) < gPeakHeightDipFraction*current_peak_height || h->GetBinContent(j) > current_peak_height ){
					b_record_peaks = 1;
				}
			}
		
			// Check to see if the current bin is over the required thresholds
			if ( b_record_peaks == 1 && h->GetBinContent(j) > threshold && h->GetBinLowEdge(j) > gPeakChannelLowerThreshold ){
				// Now check if bins either side <= current bin content 
				// (shouldn't have peak on edge of spectrum, but may need to code that in later...)
				if ( h->GetBinContent(j-1) <= h->GetBinContent(j) && h->GetBinContent(j+1) <= h->GetBinContent(j) ){
					// Store those that pass this process
					individual_peak[0] = h->GetBinContent(j);
					individual_peak[1] = h->GetBinCenter(j);
					peak_info.push_back( individual_peak );
					
					// Store most recent peak information
					b_record_peaks = 0;
					current_peak_height = h->GetBinContent(j);
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
		
		// CUSTOM TWEAKING - EDIT AS NECESSARY
		// If 5 peaks detected, if the first peak is smaller than the second, then delete the first
		if ( peak_info.size() == 5 ){
			if ( peak_info[0][0] < peak_info[1][0] ){
				peak_info.erase( peak_info.begin() );
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
void AlphaSpectrumFitter::FitPeaks(){
	// N.B. these methods were used on a particularly strong spectrum to see if the simple peak
	// finding needed something more added to it. I have left it in just in case anybody else
	// needs to do more complicated fits. Feel free to tweak!
	
	
	// Different fitting methods to try if peak finding not as successful
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		// Select a strong spectrum
		if ( fAlphaSpectrumVector[i]->GetModule() == 2 && fAlphaSpectrumVector[i]->GetAsic() == 0 && fAlphaSpectrumVector[i]->GetChannel() == 9 ){
			// First print out the peak positions determined by simple method
			std::cout << "SIMPLE PEAK FINDING" << std::endl;
			for ( int j = 0; j < gNumberOfAlphaPeaks; j++ ){
				std::cout << "Peak " << j << "  Mean = " << fAlphaSpectrumVector[i]->GetPeakChannel(j) << std::endl;
			}
			std::cout << std::endl << std::endl;
			
			// Gaussian fit
			FitGaussian( fAlphaSpectrumVector[i] );
			
			// Crystal ball fit
			FitCrystalBall( fAlphaSpectrumVector[i] );
			
			// Done
			break;
		}
	}


	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::FitGaussian( AlphaSpectrum *a ){
	// Define array to store fit parameters
	double par[3*gNumberOfAlphaPeaks + 1];
	
	// Define total fit
	TF1 *total = new TF1( "totalfit", "gaus(0) + gaus(3) + gaus(6) + gaus(9) + [12]", TMath::Max( gPeakChannelLowerThreshold, gAlphaHistLB ), gAlphaHistUB );
	
	// Format total fit
	total->SetNpx(200);
	total->SetLineColor(kBlack);
	total->SetLineWidth(4);
	
	// Define individual fits
	TF1 *indie_peaks[gNumberOfAlphaPeaks];
	
	// Get the histogram
	TH1F* h = a->GetHist();
	
	// Set up individual fits and set parameter limits
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		// Individual peaks
		indie_peaks[i] = new TF1( Form( "ind%i", i ), "gaus(0) + [3]", a->GetPeakChannel(i) - gAlphaPeakWidthEstimate, a->GetPeakChannel(i) + gAlphaPeakWidthEstimate );
		indie_peaks[i]->SetNpx(200);
		indie_peaks[i]->SetLineColor(kRed);
		indie_peaks[i]->SetLineWidth(1);
		
		// Parameter limits
		total->SetParLimits( 3*i + 0, 0.4*h->GetBinContent(h->FindBin(a->GetPeakChannel(i))), 1.1*h->GetBinContent(h->FindBin(a->GetPeakChannel(i))) );	// Amplitude
		total->SetParLimits( 3*i + 1, 0.8*a->GetPeakChannel(i), 1.2*a->GetPeakChannel(i) );		// Mu
		total->SetParLimits( 3*i + 2, 0.01*gAlphaPeakWidthEstimate, 0.5*gAlphaPeakWidthEstimate );	// Sigma
		
		// Initial parameters
		total->SetParameter( 3*i + 0, h->GetBinContent(h->FindBin(a->GetPeakChannel(i))) );
		total->SetParameter( 3*i + 1, a->GetPeakChannel(i) );
		total->SetParameter( 3*i + 2, 0.5*gAlphaPeakWidthEstimate/( 2*TMath::Sqrt( 2*TMath::Log(2) ) ) );
		
		// Parameter names
		total->SetParName( 3*i + 0, Form( "%02i Amp.",i) );
		total->SetParName( 3*i + 1, Form( "%02i Mean",i) );
		total->SetParName( 3*i + 2, Form( "%02i Sigma",i) );
	}
	
	// Set bg limits
	total->SetParLimits( 12, 0, 100 );	// Flat background
	total->SetParName( 12, "Flat bg" );
	
	// Do the fit
	h->Fit("totalfit","Q0");
	
	// Store the fit parameters
	total->GetParameters( &par[0] );
	
	// Fix the individual peak parameters
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		for ( int j = 0; j < 4; j++ ){
			if ( j < 3 ){
				indie_peaks[i]->FixParameter(j, par[3*i + j] );
			}
			else{
				indie_peaks[i]->FixParameter(j, par[12] );
			}
		}
	}
	
	TCanvas *c = new TCanvas("CANVAS-GAUSSIANFIT", "CANVAS-GAUSSIANFIT", 1200, 900 );
	h->Draw();
	
	total->Draw("SAME");
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		indie_peaks[i]->Draw("SAME");
	}
	
	c->Print("gauss.png");
	std::cout << "Fit Gaussian." << std::endl;
	
	// Print results
	std::cout << "GAUSSIAN FIT" << std::endl;
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		std::cout << "Peak " << i << "  Mean = " << indie_peaks[i]->GetParameter(1) << std::endl;
	}
	std::cout << std::endl << std::endl;
	
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumFitter::FitCrystalBall( AlphaSpectrum *a ){
	std::cout << "Fitting Crystal Ball." << std::endl;
	// Define array to store fit parameters
	double par[5*gNumberOfAlphaPeaks + 1];
	
	// Define total fit
	TF1 *total = new TF1( "totalfit", MultiCrystalBallFunctionBG, TMath::Max( gPeakChannelLowerThreshold, gAlphaHistLB ), gAlphaHistUB, 5*gNumberOfAlphaPeaks + 1 );
	
	// Format total fit
	total->SetNpx(500);
	total->SetLineColor(kBlack);
	total->SetLineWidth(4);
	
	// Define individual fits
	TF1 *indie_peaks[gNumberOfAlphaPeaks];
	
	// Get the histogram
	TH1F* h = a->GetHist();
	
	// Set up individual fits and set parameter limits
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		// Individual peaks
		indie_peaks[i] = new TF1( Form( "ind%i", i ), CrystalBallFunctionBG, a->GetPeakChannel(i) - 2*gAlphaPeakWidthEstimate, a->GetPeakChannel(i) + 2*gAlphaPeakWidthEstimate, 6 );
		indie_peaks[i]->SetNpx(500);
		indie_peaks[i]->SetLineColor(kRed);
		indie_peaks[i]->SetLineWidth(1);
		
		// Parameter limits
		total->SetParLimits( 5*i + 0, 0, 20 );	// Alpha
		total->SetParLimits( 5*i + 1, 0, 20 );	// n
		total->SetParLimits( 5*i + 2, 0.01*gAlphaPeakWidthEstimate, 0.5*gAlphaPeakWidthEstimate );	// Sigma
		total->SetParLimits( 5*i + 3, 0.8*a->GetPeakChannel(i), 1.2*a->GetPeakChannel(i) );	// Mu
		total->SetParLimits( 5*i + 4, 0.4*h->GetBinContent(h->FindBin(a->GetPeakChannel(i))), 1.1*h->GetBinContent(h->FindBin(a->GetPeakChannel(i))) );	// Amp
		
		// Initial parameters
		total->SetParameter( 5*i + 0, 2 );	// Alpha
		total->SetParameter( 5*i + 1, 2 );	// n
		total->SetParameter( 5*i + 2, 0.5*gAlphaPeakWidthEstimate/( 2*TMath::Sqrt( 2*TMath::Log(2) ) ) );	// Sigma
		total->SetParameter( 5*i + 3, a->GetPeakChannel(i));	// Mu
		total->SetParameter( 5*i + 4, h->GetBinContent(h->FindBin(a->GetPeakChannel(i))) );	// Amp
		
		// Parameter names
		total->SetParName( 5*i + 0, Form( "%02i Alpha.",i) );
		total->SetParName( 5*i + 1, Form( "%02i n",i) );
		total->SetParName( 5*i + 2, Form( "%02i Sigma",i) );
		total->SetParName( 5*i + 3, Form( "%02i Mean",i) );
		total->SetParName( 5*i + 4, Form( "%02i Amp.",i) );
	}
	
	// Set bg limits
	total->SetParLimits( 20, 0, 100 );	// Flat background
	total->SetParName( 20, "Flat bg" );
	
	// Do the fit
	h->Fit("totalfit","Q0");
	
	// Store the fit parameters
	total->GetParameters( &par[0] );
	
	// Fix the individual peak parameters
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		for ( int j = 0; j < 6; j++ ){
			if ( j < 5 ){
				indie_peaks[i]->FixParameter(j, par[5*i + j] );
			}
			else{
				indie_peaks[i]->FixParameter(j, par[20] );
			}
		}
	}
	
	TCanvas *c = new TCanvas("CANVAS-CBFIT", "CANVAS-CBFIT", 1200, 900 );
	h->Draw();
	
	total->Draw("SAME");
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		indie_peaks[i]->Draw("SAME");
	}
	
	c->Print("crystal_ball.png");
	
	// Print results
	std::cout << "CRYSTAL BALL FIT" << std::endl;
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		std::cout << "Peak " << i << "  Mean = " << indie_peaks[i]->GetParameter(3) << std::endl;
	}
	std::cout << std::endl << std::endl;

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
	// Create a canvas
	TCanvas *c = new TCanvas( "CANVAS", "CANVAS", 800, 600 );
	c->cd();
	
	// Define the progress bar
	fProgressBar = new ProgressBar( fAlphaSpectrumVector.size() );
	
	// Used for setting the file name based on the number of peaks in the spectrum
	int num = 0;
	
	std::cout << "Printing images..." << std::endl;
	// Loop over spectra
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		// Draw the spectrum (with the peaks highlighted)
		fAlphaSpectrumVector[i]->Draw();
		
		// Get the number of peaks found and set the file name
		num = fAlphaSpectrumVector[i]->GetNumberOfPeaks();
		TString file_name = "images/" + ( num > 0 && num < 10 ? "0" + (TString)Form( "%i", num ) + "/" : "" )+ (TString)(fAlphaSpectrumVector[i]->GetName()) + "__" + num + ".png";
		
		// Print the spectrum
		c->Print( file_name );
		
		// Check Progress
		fProgressBar->UpdateProgress(i);
	}

	return;
}







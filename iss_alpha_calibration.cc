#include <iostream>

#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrumFitter.hh"
#include "AlphaSpectrumGenerator.hh"
#include "AlphaSpectrumManipulator.hh"
#include "ChainMaker.hh"
#include "ProgressBar.hh"

int main( int argc, char *argv[] ){
	// Initialise global variables etc.
	InitialiseGlobalSettings();

	// Define the variables that characterise each stage
	TString output_name_chain = "alpha_chain.root";
	TString output_name_hists = "alpha_hists.root";
	TString output_name_hists_fit = "alpha_hists_fit.root";
	
	// Define tester file
	TFile *fTest;
	
	// STAGE 1: MAKE THE TCHAIN
	std::cout << "\n +++ ISS Analysis:: Making TChain +++" << std::endl;
	
	// Run test file check
	fTest = new TFile( output_name_chain, "READ" );
	
	// Check if file already exists, and only run if it doesn't
	if ( fTest->IsOpen() == 0 ){
		std::cout << "Creating " << output_name_chain << std::endl;
		ChainMaker chainmaker( argc, argv );
		chainmaker.SetOutputFile( output_name_chain );
		chainmaker.MakeChain();
		chainmaker.WriteChain();
		chainmaker.CloseOutput();
	}
	else{
		std::cout << output_name_chain << " already exists. Moving on..." << std::endl;
		fTest->Close();
	}



	// STAGE 2: GENERATE THE ALPHA SPECTRA
	std::cout << "\n +++ ISS Analysis:: Generating alpha spectra +++" << std::endl;
	
	// Run test file check
	fTest = new TFile( output_name_hists, "READ" );
	
	// Check if file already exists, and only run if it doesn't
	if ( fTest->IsOpen() == 0 ){
		std::cout << "Creating " << output_name_hists << std::endl;
		
		// Generate the spectra
		AlphaSpectrumGenerator al_spec_gen;
		al_spec_gen.SetInputFile( output_name_chain );
		al_spec_gen.SetOutputFile( output_name_hists );
		al_spec_gen.PopulateSpectra();
		al_spec_gen.LabelEmptySpectra();
		al_spec_gen.WriteSpectraToFile();
		al_spec_gen.CloseInput();
		al_spec_gen.CloseOutput();
	}
	else{
		std::cout << output_name_hists << " already exists. Moving on..." << std::endl;
		fTest->Close();
	}
	
	
	
	// STAGE 3: FIT THE ALPHA SPECTRA
	std::cout << "\n +++ ISS Analysis:: Fitting alpha spectra +++" << std::endl;
	
	// Run test file check
	//fTest = new TFile( output_name_hists_fit, "READ" );
	
	// Check if file already exists, and only run if it doesn't
	//if ( fTest->IsOpen() == 0 ){
		std::cout << "Creating " << output_name_hists_fit << std::endl;
		AlphaSpectrumFitter al_spec_fit;
		al_spec_fit.SetInputFile( output_name_hists );
		al_spec_fit.SetOutputFile( output_name_hists_fit );
		al_spec_fit.GetSpectra();
		al_spec_fit.FindPeaks();
		//al_spec_fit.FitSpectra();
		al_spec_fit.WriteFitsToFile();
		al_spec_fit.WriteFitsToImage();
		al_spec_fit.CloseInput();
		al_spec_fit.CloseOutput();
		
	//}
	//else{
		//std::cout << output_name_hists_fit << " already exists. Moving on..." << std::endl;
		//fTest->Close();
	//}
	
	
	
/*		

	TODO
	// Run checks to see if spectra already fitted
	AlphaSpectrumFitter al_spec_fit;
	al_spec_fit.SetInputFile( input_file );
	al_spec_fit.SetOutputFile( output_file );
	al_spec_fit.FitSpectra();
	al_spec_fit.CloseOutput();
	
	// Store calibration
	AlphaCalibration alpha_cal;
	al_spec_fit.GetFitData( &alpha_cal );
	
	
	std::cout << "\n +++ ISS Analysis:: Writing calibration file +++" << std::endl;
	CalibrationWriter cal_write( &alpha_cal );
	cal_write.SetOutputFile(output_file);
	cal_write.WriteCalibration();
	cal_write.CloseOutput();
*/





	std::cout << std::endl;
	return 0;
}




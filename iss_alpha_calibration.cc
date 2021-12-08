#include <iostream>

#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include "AlphaCalibrationGlobals.hh"
#include "AlphaSpectrumGenerator.hh"
#include "ChainMaker.hh"

int main( int argc, char *argv[] ){

	// Make the chain
	std::cout << "\n +++ ISS Analysis:: Making TChain +++" << std::endl;
	
	TString output_name_chain = "alpha_chain.root";
	TFile *fTest = new TFile( output_name_chain, "READ" );
	
	// Check if file already exists, and only run if it doesn't
	if ( !fTest->IsOpen() ){
		std::cout << "Making " << output_name_chain << std::endl;
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

	// Now generate all of the alpha spectra from these files
	std::cout << "\n +++ ISS Analysis:: Generating alpha spectra +++" << std::endl;
	AlphaSpectrumGenerator al_spec_gen;
	al_spec_gen.SetInputFile( "alpha_chain.root" );
	al_spec_gen.SetOutputFile( "alpha_hists.root" );
	al_spec_gen.PopulateSpectra();
	al_spec_gen.LabelEmptySpectra();
	al_spec_gen.WriteSpectraToFile();
	
/*		

	TODO
	
	// Run checks to see if spectra already generated
	AlphaSpectrumGenerator al_spec_gen( alpha_chain );
	al_spec_gen.Initialise();
	al_spec_gen.SetOutput(output_file);
	al_spec_gen.PopulateSpectra();
	al_spec_gen.PurgeEmptySpectra();
	al_spec_gen.WriteSpectraToFile();
	
	
	std::cout << "\n +++ ISS Analysis:: Fitting alpha spectra +++" << std::endl;
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






	return 0;
}




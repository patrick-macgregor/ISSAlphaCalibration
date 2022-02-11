#include "CalibrationCreator.hh"

CalibrationCreator::CalibrationCreator(){}
// --------------------------------------------------------------------------------------------- //
CalibrationCreator::~CalibrationCreator(){}
// --------------------------------------------------------------------------------------------- //
void CalibrationCreator::GetSpectra(){
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
void CalibrationCreator::GetOffsetAndGainFromSpectrum( AlphaSpectrum *a, double &quadgain, double &gain, double &offset ){
	// Do a LINEAR or QUADRATIC fit to generate the calibration
	bool quadratic = gQuadraticAlphaCalibration;
	
	// Test if there's capacity for quadratic based on the number of peaks (for safety!)
	if ( a->GetNumberOfPeaks() < 3 ){
		quadratic = 0;
	}
	if ( a->GetNumberOfPeaks() != gNumberOfAlphaPeaks ){
		// Impossible to determine how to fit this
		quadgain = 0;
		gain = 0;
		offset = 0;
		return;
	}
	
	// First define common quantities to each: X = channel, Y = alpha energy
	double X1 = 0.0, X2 = 0.0, X3 = 0.0, X4 = 0.0, Y0 = 0.0, Y1 = 0.0, Y2 = 0.0;
	int X0 = gNumberOfAlphaPeaks;
	
	// Calculate fitting parameters
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		X1 += a->GetPeakChannel(i);
		X2 += TMath::Power( a->GetPeakChannel(i), 2 );
		Y0 += gAlphaParticleEnergy[i];
		Y1 += gAlphaParticleEnergy[i]*a->GetPeakChannel(i);
		
		if ( quadratic ){
			X3 += TMath::Power( a->GetPeakChannel(i), 3 );
			X4 += TMath::Power( a->GetPeakChannel(i), 4 );
			Y2 += gAlphaParticleEnergy[i]*TMath::Power( a->GetPeakChannel(i), 2 );
		}
		
	}
	
	// Final calculations
	double det;
	double term[9];
	if ( quadratic ){
		det = X0*( X2*X4 - X3*X3 ) - X1*( X4*X1 - X3*X2 ) + X2*( X3*X1 - X2*X2 );
		term[0] = X4*X2 - X3*X3;
		term[1] = X3*X2 - X4*X1;
		term[2] = X3*X1 - X2*X2;
		term[3] = term[1];
		term[4] = X4*X0 - X2*X2;
		term[5] = X2*X1 - X3*X0;
		term[6] = term[2];
		term[7] = term[5];
		term[8] = X2*X0 - X1*X1;
	}
	else{
		det = X0*X2 - X1*X1;
		term[0] = X2;
		term[1] = -X1;
		term[2] = 0;
		term[3] = term[1];
		term[4] = X0;
		term[5] = 0;
		term[6] = term[2];
		term[7] = term[5];
		term[8] = 0;
	}
	
	quadgain = ( term[6]*Y0 + term[7]*Y1 + term[8]*Y2 )/det;
	gain =     ( term[3]*Y0 + term[4]*Y1 + term[5]*Y2 )/det;
	offset =   ( term[0]*Y0 + term[1]*Y1 + term[2]*Y2 )/det;
	
	return;
}
// --------------------------------------------------------------------------------------------- //
TString CalibrationCreator::GetCalibrationParameters( AlphaSpectrum* a, double &quadgain, double &gain, double &offset ){
	// Check the quadratic term
	bool b_quadgain = 0;
	if ( quadgain != 0 ){ b_quadgain = 1; }
	
	 // Now write the correct lines (N.B. assuming quadgain = 0 is default, so won't bother printing if this is true)
	TString asic_number = Form( "asic_%i_%i_%i.", a->GetModule(), a->GetAsic(), a->GetChannel() );
	
	const int num_cal_lines = 3;
	const int cal_line_length_before_number = 24;
	TString calibration_line[num_cal_lines];
	
	calibration_line[0] = asic_number + "Offset:";
	calibration_line[1] = asic_number + "Gain:";
	calibration_line[2] = ( b_quadgain ? asic_number + "GainQuadr:" : "" );
	
	// Append spaces
	for ( int i = 0; i < num_cal_lines; i++ ){
		calibration_line[i].Append( ' ', cal_line_length_before_number - calibration_line[i].Length() );
	}
	
	// Append number
	calibration_line[0].Append( ( offset != 0 ? Form("%10.4f", offset ) : "????????" ) );
	calibration_line[1].Append( ( gain != 0 ? Form("%10.4f", gain ) : "????????" ) );
	if ( b_quadgain ){ calibration_line[2].Append( Form("%8.8f", quadgain ) ); }
	
	// Create final return string
	TString final_string = ( offset == 0 ? "# " : "" ) + calibration_line[0] + "\n" + 
		( gain == 0 ? "# " : "" ) + calibration_line[1] + "\n" + 
		( b_quadgain ? calibration_line[2] + "\n" : "" );
		
	return final_string;
	
}

// --------------------------------------------------------------------------------------------- //
void CalibrationCreator::WriteCalibration(){
	// Define quantities to store the offset parameters
	double quadgain = 0.0;
	double gain = 0.0;
	double offset = 0.0;
	
	// Loop over fit spectra
	for  ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		// Get offset and gain
		GetOffsetAndGainFromSpectrum( fAlphaSpectrumVector[i], quadgain, gain, offset );
		
		// Typeset the results and write to file
		TString line = GetCalibrationParameters( fAlphaSpectrumVector[i], quadgain, gain, offset );
		
		// Write to file
		fOutputFileDat << line;

	}
	
	return;
}
// --------------------------------------------------------------------------------------------- //
void CalibrationCreator::SetOutputFile( TString file ){
	fOutputFileDat.open( file.Data(), std::fstream::trunc );
	if ( !fOutputFileDat.is_open() ){
		std::cerr << "Calibration data file " << file << " failed to open." << std::endl;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void CalibrationCreator::CloseOutput(){
	fOutputFileDat.close();
	return;
}

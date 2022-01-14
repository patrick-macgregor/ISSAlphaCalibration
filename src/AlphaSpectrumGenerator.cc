#include "AlphaSpectrumGenerator.hh"

AlphaSpectrumGenerator::AlphaSpectrumGenerator(){
	Initialise();
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrumGenerator::~AlphaSpectrumGenerator(){}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::Initialise(){
	
	fAlphaSpectrumVector.resize( NumberOfArrayModules );

	for ( int i = 0; i < NumberOfArrayModules; i++ ){
		fAlphaSpectrumVector[i].resize( NumberOfArrayASICs );

		for ( int j = 0; j < NumberOfArrayASICs; j++ ){
			fAlphaSpectrumVector[i][j].resize( NumberOfPArrayChannels );

			for ( int k = 0; k < NumberOfPArrayChannels; k++ ){
				fAlphaSpectrumVector[i][j][k] = new AlphaSpectrum(i,j,k);
			}
		}		
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
template <typename T>
bool AlphaSpectrumGenerator::CheckTTreeReaderArray( TTreeReaderArray<T>& arr, T lb, T ub ){
	// Check the size of the array
	if ( arr.IsEmpty() ){ return false; }
	else{
		// Loop over the array and check the numbers are within the bounds
		for ( unsigned int i = 0; i < arr.GetSize(); i++ ){
			if ( arr.At(i) < lb ){ return false; }
			if ( arr.At(i) > ub ){ return false; }
		}
	}
	
	return true;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::PopulateSpectra(){
	if ( fInputFile == NULL || fOutputFile == NULL ){
		std::cerr << "Not all files assigned!" << std::endl;
		return;
	}
	
	// Access tree
	TChain *chain = (TChain*)fInputFile->Get("iss");
	
	// Determine progress bar
	long num_entries = chain->GetEntries();
	fProgressBar = new ProgressBar( chain->GetEntries() );
	
	// Set TTree Reader to look at this tree
	TTreeReader tree_reader(chain);
	
	// Get reader arrays to look at different sources
	TTreeReaderArray<unsigned char> tree_channel = {tree_reader, "asic_packets.ch"};
	TTreeReaderArray<unsigned char> tree_asic = {tree_reader, "asic_packets.asic"};
	TTreeReaderArray<unsigned char> tree_mod = {tree_reader, "asic_packets.mod"};
	TTreeReaderArray<unsigned short> tree_energy = {tree_reader, "asic_packets.adc_value"};
	
	// Loop over entries
	std::cout << "Making histograms..." << std::endl;
	for ( long i = 0; i < num_entries; i++ ){
		// Set the entry
		tree_reader.SetEntry(i);
		fProgressBar->UpdateProgress(i);

		// Check that all arrays have the same size and are filled
		bool bAllDataAreGood = ( 
			CheckTTreeReaderArray<unsigned char>( tree_channel, 0, (unsigned char)NumberOfPArrayChannels ) &&
			CheckTTreeReaderArray<unsigned char>( tree_asic, 0, (unsigned char)NumberOfArrayASICs ) &&
			CheckTTreeReaderArray<unsigned char>( tree_mod, 0, (unsigned char)NumberOfArrayModules ) &&
			CheckTTreeReaderArray<unsigned short>( tree_energy, 0, 65535 ) &&
			(
				tree_channel.GetSize() == tree_asic.GetSize() &&
				tree_channel.GetSize() == tree_mod.GetSize() && 
				tree_channel.GetSize() == tree_energy.GetSize()
			)
		);
		
		
		// Populate hists if they are good
		if ( bAllDataAreGood ){
			// Loop over all data points in array (which probably has a length of 1!)
			for ( unsigned int j = 0; j < tree_channel.GetSize(); j++ ){
				fAlphaSpectrumVector[ tree_mod.At(j) ][ tree_asic.At(j) ][ tree_channel.At(j) ]->FillHistogram( tree_energy.At(j) );
			}
			
		}
		
	}
	
	return;
	
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::LabelEmptySpectra(){
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		for ( unsigned int j = 0; j < fAlphaSpectrumVector[i].size(); j++ ){
			for ( unsigned int k = 0; k < fAlphaSpectrumVector[i][j].size(); k++ ){
				fAlphaSpectrumVector[i][j][k]->CheckIsEmpty();
			}
		}
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::WriteSpectraToFile(){
	fOutputFile->cd();
	for ( unsigned int i = 0; i < fAlphaSpectrumVector.size(); i++ ){
		for ( unsigned int j = 0; j < fAlphaSpectrumVector[i].size(); j++ ){
			for ( unsigned int k = 0; k < fAlphaSpectrumVector[i][j].size(); k++ ){
				if ( !fAlphaSpectrumVector[i][j][k]->IsEmpty() ){
					fOutputFile->WriteTObject( fAlphaSpectrumVector[i][j][k] );	
				}
			}
		}
	}
	return;
}












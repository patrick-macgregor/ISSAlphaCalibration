#include "AlphaSpectrumGenerator.hh"

// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::AlphaSpectrum(){
	fChannel = 0;
	fModule = 0;
	fAsic = 0;
	fIsEmpty = 0;
	fHist = NULL;
}
// --------------------------------------------------------------------------------------------- //
AlphaSpectrum::AlphaSpectrum( int mod, int asic, int ch ){
	fChannel = ch;
	fModule = mod;
	fAsic = asic;
	fIsEmpty = 0;
	CreateHistogram();
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
		fHist = new TH1F( \
			Form( "alpha_spectrum_%i_%i_%i", fModule, fAsic, fChannel ), \
			Form( "Mod. %i | Asic %i | Ch. %i", fModule, fAsic, fChannel ), \
			325, 0, 6500 \
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
// ============================================================================================= //
AlphaSpectrumGenerator::AlphaSpectrumGenerator(){
	Initialise();
	fInputFile = NULL;
	fOutputFile = NULL;
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
void AlphaSpectrumGenerator::SetFile( TString file, TFile*& f, TString file_open_state ){
	f = new TFile( file, file_open_state );
	std:: cout << f->GetName() << " created." << std::endl;
	if ( !f->IsOpen() ){
		std::cerr << "Alpha spectrum file " << file << " failed to open." << std::endl;
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::SetInputFile( TString file ){
	SetFile( file, fInputFile, "READ" );
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::SetOutputFile(  TString file  ){
	SetFile( file, fOutputFile, "RECREATE" );
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
	float progress_frac = 0.01; // Fraction of total
	int progress_div = num_entries*progress_frac;
	int progress_ctr = 0;
	
	// Set TTree Reader to look at this tree
	TTreeReader tree_reader(chain);
	
	// Get reader arrays to look at different sources
	TTreeReaderArray<unsigned char> tree_channel = {tree_reader, "asic_packets.ch"};
	TTreeReaderArray<unsigned char> tree_asic = {tree_reader, "asic_packets.asic"};
	TTreeReaderArray<unsigned char> tree_mod = {tree_reader, "asic_packets.mod"};
	TTreeReaderArray<float> tree_energy = {tree_reader, "asic_packets.energy"};
	
	// Loop over entries
	for ( long i = 0; i < num_entries; i++ ){
		// Set the entry
		tree_reader.SetEntry(i);
		
		// Spit out progres
		if ( i % progress_div == 0 ){
			std::cout << "Histogram-making: " << std::setw(3) << std::setprecision(0) << std::fixed << progress_ctr*progress_frac*100 << " %\r" << std::flush;
			progress_ctr++;
		}

		// Check that all arrays have the same size and are filled
		bool bAllDataAreGood = ( 
			CheckTTreeReaderArray<unsigned char>( tree_channel, 0, (unsigned char)NumberOfPArrayChannels ) &&
			CheckTTreeReaderArray<unsigned char>( tree_asic, 0, (unsigned char)NumberOfArrayASICs ) &&
			CheckTTreeReaderArray<unsigned char>( tree_mod, 0, (unsigned char)NumberOfArrayModules ) &&
			CheckTTreeReaderArray<float>( tree_energy, 0, 1e8 ) &&
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
				fAlphaSpectrumVector[ tree_mod.At(j) ][ tree_asic.At(j) ][ tree_channel.At(j) ]->Fill( tree_energy.At(j) );
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
					fAlphaSpectrumVector[i][j][k]->Write();	
				}
			}
		}
	}
	return;
}
// --------------------------------------------------------------------------------------------- //
void AlphaSpectrumGenerator::CloseOutput(){}











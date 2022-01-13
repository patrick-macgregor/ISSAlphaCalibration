#include <TH1F.h>
#include <TFile.h>
#include <TList.h>
#include "../include/AlphaSpectrum.hh"


int MinMaxBins(){
	TFile *f = new TFile("../alpha_hists.root");
	TList *l = (TList*)f->GetListOfKeys();
	
	AlphaSpectrum *a;
	TH1F *h;
	int min_bin = 10000;
	int max_bin = 0;
	
	bool found_min;
	int current_bin;
	
	for ( int i = 0; i < l->GetSize(); i++ ){
		a = (AlphaSpectrum*)f->Get( l->At(i)->GetName() );
		h = a->GetHist();
		
		// Reset conditions
		current_bin = 0;
		found_min = 0;
		
		for ( int j = 0; j < h->GetNbinsX(); j++ ){
			if ( h->GetBinContent(j) > 0 ){
				current_bin = j;
				
				// Sort out minimum
				if ( found_min == 0 ){
					found_min = 1;
					if ( current_bin < min_bin ){
						min_bin = current_bin;
					}
				}
			}
		}
		
		if ( current_bin > max_bin ){
			max_bin = current_bin;
		}
		
		std::cout << l->At(i)->GetName() << "    " << min_bin << "    " << max_bin << std::endl;
		
	}


}

#include <TH1F.h>
#include <TFile.h>
#include <TList.h>
#include "../include/AlphaSpectrum.hh"

#include <iostream>

// Open the hists file and see which one has the most counts
int BestAlphaSpectrum(){
	TFile *f = new TFile("../alpha_hists.root");
	TList *l = (TList*)f->GetListOfKeys();
	
	AlphaSpectrum *a;
	TH1F *h;
	double max_counts = 0;
	
	for ( int i = 0; i < l->GetSize(); i++ ){
		a = (AlphaSpectrum*)f->Get( l->At(i)->GetName() );
		h = a->GetHist();
		if ( h->Integral() > max_counts ){
			std::cout << l->At(i)->GetName() << " takes max from " << max_counts << " to " << h->Integral() << std::endl;
			max_counts = h->Integral();
		}
	}
	
	return 0;
}

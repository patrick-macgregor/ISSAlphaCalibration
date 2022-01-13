#include <TFile.h>
#include <TList.h>
#include "../include/AlphaSpectrum.hh"
#include <iostream>

void FindBadPeaks(){
	TFile *f = new TFile("../alpha_hists_fit.root");
	TList *l = (TList*)f->GetListOfKeys();
	
	AlphaSpectrum *a;
	
	for ( int i = 0; i < l->GetSize(); i++ ){
		a = (AlphaSpectrum*)f->Get( l->At(i)->GetName() );
		if ( a->GetNumberOfPeaks() != 4 ){
			std::cout << l->At(i)->GetName() << "    " << a->GetNumberOfPeaks() << std::endl;
		}
	}
	

}

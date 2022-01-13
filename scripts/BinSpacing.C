#include <iostream>

#include "../include/AlphaSpectrum.hh"
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>

int BinSpacing(){
	TFile *f = new TFile("alpha_chain.root");
	TTree *t = (TTree*)f->Get("iss");
	
	t->Draw("asic_packets.energy>>hist(6000,0,6000)","asic_packets.mod == 0 && asic_packets.asic == 0 && asic_packets.ch == 11","goff");

	TH1F* hist = (TH1F*)gDirectory->Get("hist");
	
	int bin_ctr = 0;
	bool start_cnt = 0;
	TH1F* h_bin_ctr = new TH1F( "bin_ctr", "bin_ctr", 100, 0, 100 );

	for ( int i = 0; i < hist->GetNbinsX(); i++ ){
		if ( hist->GetBinContent(i) != 0 ){
			if ( start_cnt == 0 ){ start_cnt = 1; }
			else{
				h_bin_ctr->Fill( bin_ctr );
				bin_ctr = 0;
			}
		}
		else{
			if ( start_cnt == 1 ){ bin_ctr++; }
		}
	}

	h_bin_ctr->Draw();

	return 0;
}

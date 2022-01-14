#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <iomanip>
#include <iostream>

#include <TString.h>

class ProgressBar{
	private:
		int fMaxEntries;				// Number of entries to loop over
		float fCurrentPercentage;		// Current percentage progress based on divisor
		float fProgressPercentage;		// The step in percentage
		int fProgressDivisor;			// The divisor to make the numbers work
		
		void PrintProgress();
		TString GetProgressBar();
		
	public:
		ProgressBar( int max_entries );
		ProgressBar( int max_entries, float progress_fraction );
		~ProgressBar();
		
		inline void SetMaxEntries( int max_entries ){ fMaxEntries = max_entries; ResetProgress(); }
		void UpdateProgress( int i );
		void ResetProgress();

};

#endif

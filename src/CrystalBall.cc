#include "CrystalBall.hh"

// Adapted from https://root-forum.cern.ch/t/crystalball-fitting-function/26973
// and https://en.wikipedia.org/wiki/Crystal_Ball_function
// --------------------------------------------------------------------------------------------- //
double CrystalBallFunction(double x, double alpha, double n, double sigma, double mean, double amp) {
	// Check sigma, n, alpha, and amp have good values
	if ( sigma <= 0.0 || alpha <= 0.0 || n <= 0.0 || amp <= 0.0){
		return 0.0;
	}
	
	// Define z and absalpha
	double z = (x - mean)/sigma;
	double abs_alpha = TMath::Abs( alpha );
	
	// Define A,B,C,D,N
	double A = TMath::Power( n/abs_alpha, n)*TMath::Exp( -0.5*TMath::Power( abs_alpha, 2 ) );
	double B = ( n/abs_alpha ) - abs_alpha;
	
	// Wikipedia defines C & D, but they're not needed here!
	//double C = ( n/( abs_alpha*( n - 1 ) ) )*TMath::Exp( -0.5*TMath::Power( abs_alpha, 2 ) );
	//double D = TMath::Sqrt( TMath::Pi()/2 )*( 1 + TMath::Erf( abs_alpha/TMath::Sqrt(2) ) );
	
	// Define function
	if ( z > -abs_alpha ){
		return amp*TMath::Exp( -0.5*z*z );
	}
	else {
		return amp*A*TMath::Power( B - z, -n );
	}
}
// --------------------------------------------------------------------------------------------- //
double CrystalBallFunction(const double *x, const double *p) {
	return CrystalBallFunction( x[0], p[0], p[1], p[2], p[3], p[4] );
}
// --------------------------------------------------------------------------------------------- //
double CrystalBallFunctionBG(const double *x, const double *p) {
	return CrystalBallFunction( x[0], p[0], p[1], p[2], p[3], p[4] ) + p[5];
}
// --------------------------------------------------------------------------------------------- //
double MultiCrystalBallFunction( const double *x, const double *p ){
	double sum = 0;
	for ( int i = 0; i < gNumberOfAlphaPeaks; i++ ){
		sum += CrystalBallFunction( x, &p[5*i] );
	}
	return sum;
}
// --------------------------------------------------------------------------------------------- //
double MultiCrystalBallFunctionBG( const double *x, const double *p ){
	return MultiCrystalBallFunction( x, p ) + p[gNumberOfAlphaPeaks*5];
}

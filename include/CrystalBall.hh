#ifndef _CRYSTAL_BALL_HH_
#define _CRYSTAL_BALL_HH_

#include <TMath.h>
#include "AlphaCalibrationGlobals.hh"

double CrystalBallFunction(double x, double alpha, double n, double sigma, double mean, double amp);
double CrystalBallFunction(const double *x, const double *p);
double MultiCrystalBallFunction( const double *x, const double *p );
double CrystalBallFunctionBG(const double *x, const double *p);
double MultiCrystalBallFunctionBG( const double *x, const double *p );

#endif

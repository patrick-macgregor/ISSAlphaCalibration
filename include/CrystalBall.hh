#ifndef _CRYSTAL_BALL_HH_
#define _CRYSTAL_BALL_HH_

#include <TMath.h>
#include "AlphaCalibrationGlobals.hh"

// This header file just contains fitting functions for crystal ball functions

// Individual crystal ball
double CrystalBallFunction(double x, double alpha, double n, double sigma, double mean, double amp);
double CrystalBallFunction(const double *x, const double *p);

// Add a flat background
double CrystalBallFunctionBG(const double *x, const double *p);

// Multiple crystal ball
double MultiCrystalBallFunction( const double *x, const double *p );

// Multiple crystal ball with a flat background
double MultiCrystalBallFunctionBG( const double *x, const double *p );

#endif

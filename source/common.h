#pragma once

#include <cmath> // erf, sqrt


struct GlucoseState
{
    double amount;
    // double bloodGlucose;
    double bloodBGL;
    double bloodMinBGL;
};

struct FatState
{
    double amount;
    double bodyWeight;
};

// Cummulative distribution function
double cdf(double x, double mean, double stdDev);

// Michaelis-Menten kinetics
double mmk(double Vmax, double S, double Km);
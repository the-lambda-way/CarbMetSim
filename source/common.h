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


double cdf(double x, double mean, double stdDev);

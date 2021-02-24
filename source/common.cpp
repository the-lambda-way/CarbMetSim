#include "common.h"


double cdf(double x, double mean, double stdDev)
{
    return 0.5 * (1 + std::erf((x - mean) / (stdDev * std::sqrt(2))));
}

double mmk(double Vmax, double S, double Km)
{
    return Vmax * S / (S + Km);
}
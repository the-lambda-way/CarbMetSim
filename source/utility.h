#pragma once

#include <cmath> // erf, sqrt


double cdf(double x, double mean, double stdDev)
{
    return 0.5 * (1 + std::erf((x - mean) / (stdDev * sqrt(2))));
}

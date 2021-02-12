#include "common.h"


double cdf(double x, double mean, double stdDev)
{
    return 0.5 * (1 + std::erf((x - mean) / (stdDev * std::sqrt(2))));
}

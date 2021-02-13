#include "Blood.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <iterator> // std::cbegin, std::cend
#include <numeric> // std::reduce
#include <random>
#include <stdlib.h>
#include "HumanBody.h"
#include "Liver.h"

using namespace std;


Blood::Blood(HumanBody* body)
    : body{body}
{
    // initial number of RBCs
    for (auto& bin : ageBins)
    {
        bin.RBCs         = 0.94 * rbcBirthRate;
        bin.glycatedRBCs = 0.06 * rbcBirthRate;
    }
}

void Blood::updateRBCs()
{
    // will be called once a day

    --bin0;
    if (bin0 < 0)    bin0 = MAX_AGE;

    // New RBCs take birth
    ageBins[bin0].RBCs         = rbcBirthRate;
    ageBins[bin0].glycatedRBCs = 0;

    // Old (100 to 120 days old) RBCs die
    int start_bin = bin0 + HUNDRED_DAYS;
    if (start_bin > MAX_AGE)    start_bin -= MAX_AGE + 1;

    for (int i = 0; i <= MAX_AGE - HUNDRED_DAYS; ++i)
    {
        int j = start_bin + i;
        if (j > MAX_AGE)    j -= MAX_AGE + 1;

        assert(((void)"RBC bin value negative", j >= 0));

        killRate[i] = static_cast<double>(i) / static_cast<double>(MAX_AGE - HUNDRED_DAYS);

        ageBins[j].RBCs         *= 1.0 - killRate[i];
        ageBins[j].glycatedRBCs *= 1.0 - killRate[i];

        killBin[i] = j;
    }

    // glycate the RBCs
    double glycationProb = avgBGLOneDay * glycationProbSlope + glycationProbConst;

    for (auto& bin : ageBins)
    {
        double newlyGlycated = glycationProb * bin.RBCs;

        bin.RBCs         -= newlyGlycated;
        bin.glycatedRBCs += newlyGlycated;
    }
}

double Blood::currentHbA1c() const
{
    double RBCs         = 0;
    double glycatedRBCs = 0;

    for (const auto& bin : ageBins)
    {
        RBCs         += bin.RBCs;
        glycatedRBCs += bin.glycatedRBCs;
    }

    assert(RBCs > 0);

    return glycatedRBCs / (RBCs + glycatedRBCs);
}

void Blood::processTick()
{
    glycolysis();
    updateInsulinLevel();

    // calculating average bgl during a day

    if (avgBGLOneDayCount == ONE_DAY)
    {
        avgBGLOneDay      = avgBGLOneDaySum / ONE_DAY;
        avgBGLOneDaySum   = 0;
        avgBGLOneDayCount = 0;

        updateRBCs();
        RBCsUpdated = true;
    }
    else
        RBCsUpdated = false;

    avgBGLOneDaySum += glucose / fluidVolume;
    ++avgBGLOneDayCount;

}

void Blood::setParams(const BloodParams& params)
{
    rbcBirthRate        = params.rbcBirthRate;
    glycationProbSlope  = params.glycationProbSlope;
    glycationProbConst  = params.glycationProbConst;
    minGlucoseLevel     = params.minGlucoseLevel;
    baseGlucoseLevel    = params.baseGlucoseLevel;
    glucose             = fluidVolume * baseGlucoseLevel;

    if (params.baseInsulinLevel >= body->insulinImpactOnGNG_Mean)
        throw out_of_range("error configuring baseInsulinLevel and insulinImpactOnGNG");
    if (params.baseInsulinLevel >= body->insulinImpactGlycogenBreakdownInLiver_Mean)
        throw out_of_range("error configuring baseInsulinLevel and insulinImpactGlycogenBreakdownInLiver");
    baseInsulinLevel = params.baseInsulinLevel;

    if (params.baseInsulinLevel >= body->insulinImpactOnGNG_Mean)
        throw out_of_range("error configuring baseInsulinLevel and insulinImpactOnGNG");
    insulinLevel = baseInsulinLevel;

    peakInsulinLevel = params.peakInsulinLevel;

    if (params.highGlucoseLevel <= baseGlucoseLevel)    throw out_of_range("highGlucoseLevel <= baseGlucoseLevel");
    highGlucoseLevel = params.highGlucoseLevel;

    highLactateLevel    = params.highLactateLevel;
    glycolysisMin       = params.glycolysisMin;
    glycolysisMax       = params.glycolysisMax;
    glycolysisToLactate = params.glycolysisToLactate;
}

void Blood::glycolysis()
{
    static std::poisson_distribution<int> glycolysisMin__ (1000.0 * glycolysisMin);

    // RBCs consume about 25mg of glucose every minute and convert it to lactate via glycolysis.
    // Gerich: Glycolysis. Depends on insulin level. Some of the consumed glucose becomes lactate.

    double x = static_cast<double>(glycolysisMin__(body->generator())) / 1000.0;
    glycolysisPerTick = min(glucose, body->glycolysis(x, glycolysisMax));

    glucose -= glycolysisPerTick;
    lactate += glycolysisToLactate * glycolysisPerTick;

    totalGlycolysisSoFar += glycolysisPerTick;
}

void Blood::updateInsulinLevel()
{
    double bgl = glucose / fluidVolume;

    if (bgl >= highGlucoseLevel)
        insulinLevel = peakInsulinLevel;

    else if (bgl <= minGlucoseLevel)
        insulinLevel = 0;

    else if (bgl >= baseGlucoseLevel)
        insulinLevel = baseInsulinLevel + (peakInsulinLevel - baseInsulinLevel)
                     * (bgl - baseGlucoseLevel) / (highGlucoseLevel - baseGlucoseLevel);

    else if (body->isExercising())
    {
        if (body->percentVO2Max >= body->intensityPeakGlucoseProd)
            insulinLevel = 0;

        else
        {
            double restIntensity = 3.5 * 2.0 / body->vo2Max;

            assert(((void)"%VO2 less than restIntensity when body is exercising",
                   body->percentVO2Max >= restIntensity));

            insulinLevel = baseInsulinLevel - (body->percentVO2Max - restIntensity)
                         * baseInsulinLevel / (body->intensityPeakGlucoseProd - restIntensity);
        }
    }

    else
        insulinLevel = baseInsulinLevel * (bgl - minGlucoseLevel) / (baseGlucoseLevel - minGlucoseLevel);
}

void Blood::removeGlucose(double howmuch)
{
    glucose -= howmuch;
}

void Blood::addGlucose(double howmuch)
{
    glucose += howmuch;
}

double Blood::getBGL() const
{
    return glucose / fluidVolume;
}

double Blood::getGNGSubstrates() const
{
    return gngSubstrates + lactate + alanine + glutamine;
}

double Blood::baseBGL() const
{
    return baseGlucoseLevel;
}

double Blood::highBGL() const
{
    return highGlucoseLevel;
}

double Blood::volume() const
{
    return fluidVolume;
}

// rate is in units of mg per kg per minute
double Blood::gngFromHighLactate(double rate)
{
    // Gluconeogenesis will occur even in the presence of high insulin in proportion to lactate concentration. High
    // lactate concentration (e.g. due to high glycolytic activity) would cause gluconeogenesis to happen even if
    // insulin concentration is high. But then Gluconeogenesis would contribute to glycogen store of the liver (rather
    // than generating glucose).

    double x = rate * lactate / highLactateLevel;
    x = min(x, lactate);

    lactate -= x;
    return x;
}

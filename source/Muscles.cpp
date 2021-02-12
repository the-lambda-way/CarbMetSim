#include "Muscles.h"
#include <algorithm>
#include <cassert>
#include <random>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "common.h"
#include "HumanBody.h"

using namespace std;


void Muscles::setParams(const MusclesParams& params)
{
    glycogen                           = 1000.0 * params.glycogen;
    glycogenMax                        = 1000.0 * params.glycogenMax;
    Glut4Km                            = params.Glut4Km;
    Glut4VMAX                          = params.Glut4VMAX;
    PeakGlut4VMAX                      = params.PeakGlut4VMAX;
    maxGlucoseAbsorptionDuringExercise = params.maxGlucoseAbsorptionDuringExercise;
    basalGlucoseAbsorbed               = params.basalGlucoseAbsorbed;
    baaToGlutamine                     = params.baaToGlutamine;
    glycolysisMin                      = params.glycolysisMin;
    glycolysisMax                      = params.glycolysisMax;
    glucoseToGlycogen                  = params.glucoseToGlycogen;
    glycogenShareDuringExerciseMean    = params.glycogenShareDuringExerciseMean;
}

void Muscles::processTick()
{
    // we assume that all exercise is aerobic exercise.

    // molecular mass of glucose is 180.1559 g/mol
    // During glycolysis, 1 molecule of glucose results in 2 molecules of pyruvate (and hence 2 molecules of lactate).
    // So 1mmol/l of lactate corresponds to 0.5mmol/l of glucose.

    static std::poisson_distribution<int> rand__{100};
    static std::poisson_distribution<int> glycolysisMin__{1000.0 * glycolysisMin};

    glucoseAbsorbedPerTick   = 0;
    glycogenBreakdownPerTick = 0;
    glycogenOxidizedPerTick  = 0;
    oxidationPerTick         = 0;

    currEnergyNeed = body->currentEnergyExpenditure();

    hadExercise = body->isExercising();
    if (hadExercise)
    {
        glucoseAbsorption(rand__);
        glycogenToGlucose(rand__);
        glycogenToLactate(glycolysisMin__);
        fattyAcidConsumptionActive();
    }
    else
    {
        basalAbsorption(rand__);
        glycogenSynthesis();
        glucoseToLactate(glycolysisMin__);
        oxidation();
        fattyAcidConsumptionResting();
    }

    assert(((void)"Glycogen in muscles went negative\n", glycogen >= 0));

    // aminoAcidConsumption();

    // totalGlucoseAbsorbed += glucoseAbsorbedPerTick;
}

Muscles::Muscles(HumanBody* myBody)
    : body{myBody}
{
}

void Muscles::glucoseAbsorption(std::poisson_distribution<int>& rand)
{
    // 10% of energy comes from glucose on average
    // oxidation of 1g of carbs yields 4kcal of energy

    double x = 0.9 + static_cast<double>(rand(body->generator())) / 1000.0;
    oxidationPerTick = 0.1 * x * 1000.0 * currEnergyNeed / 4.0; // in milligrams
    oxidationPerTick = min(oxidationPerTick, maxGlucoseAbsorptionDuringExercise * body->bodyWeight);

    absorbedFromBlood = oxidationPerTick - glucose;

    if (absorbedFromBlood > 0)
    {
        glucose = 0;

        body->blood.removeGlucose(absorbedFromBlood);
        glucoseAbsorbedPerTick += absorbedFromBlood;

        absorptionState.amount      = absorbedFromBlood;
        absorptionState.bloodBGL    = body->blood.getBGL();
        absorptionState.bloodMinBGL = body->blood.minGlucoseLevel;
    }
    else
        glucose -= oxidationPerTick;
}

void Muscles::glycogenToGlucose(std::poisson_distribution<int>& rand)
{
    double intensity = body->percentVO2Max;
    double mean      = glycogenShareDuringExerciseMean;
    double stddev    = 0.2;
    glycogenShare    = 0.9 * cdf(intensity, mean, stddev);

    double x = 0.9 + static_cast<double>(rand(body->generator())) / 1000.0;
    glycogenOxidizedPerTick = glycogenShare * x * 1000.0 * currEnergyNeed / 4.0; // in milligrams

    glycogen                 -= glycogenOxidizedPerTick;
    glycogenBreakdownPerTick += glycogenOxidizedPerTick;
}

void Muscles::glycogenToLactate(std::poisson_distribution<int>& glycolysisMin_)
{
    double intensity = body->percentVO2Max;

    double x = static_cast<double>(glycolysisMin_(body->generator()));
    double g = x * body->bodyWeight / 1000.0;
    g        = min(g, glycolysisMax * body->bodyWeight);

    glycolysisPerTick = g + intensity * (glycolysisMax * body->bodyWeight - g);

    glycogen                 -= glycolysisPerTick;
    body->blood.lactate     += glycolysisPerTick;
    glycogenBreakdownPerTick += glycolysisPerTick;
}

void Muscles::fattyAcidConsumptionActive()
{
    double kcalgenerated = 0.004 * (oxidationPerTick + glycogenOxidizedPerTick + glycolysisPerTick / 15.0);

    if (kcalgenerated < currEnergyNeed)
        body->adiposeTissue.consumeFat(currEnergyNeed - kcalgenerated);
}

void Muscles::basalAbsorption(std::poisson_distribution<int>& rand)
{
    static std::poisson_distribution<int> basalAbsorption__{1000.0 * basalGlucoseAbsorbed};

    double x = static_cast<double>(basalAbsorption__(body->generator()));
    double g = x * body->bodyWeight / 1000.0;

    body->blood.removeGlucose(g);
    glucoseAbsorbedPerTick = g;
    glucose               += g;

    basalBase.amount      = g;
    basalBase.bloodBGL    = body->blood.getBGL();
    basalBase.bloodMinBGL = body->blood.minGlucoseLevel;

    // Absorption via GLUT4
    double bgl  = body->blood.getBGL();
    double diff = bgl - glucose / volume;

    basalGLUT4Occurred = diff > 0;
    if (basalGLUT4Occurred)
    {
        double scale = body->glut4Impact;

        if (body->ticks() > static_cast<unsigned>(body->lastHardExerciseAt + 60) || bgl < body->blood.baseBGL())
            scale *= body->blood.insulinLevel;

        scale *= PeakGlut4VMAX - glycogen * (PeakGlut4VMAX - Glut4VMAX) / glycogenMax;
        scale *= diff / (diff + Glut4Km);
        x = 0.9 + static_cast<double>(rand(body->generator())) / 1000.0;
        g = scale * x * body->bodyWeight;

        body->blood.removeGlucose(g);
        glucoseAbsorbedPerTick += g;
        glucose                += g;

        basalGLUT4.amount      = g;
        basalGLUT4.bloodBGL    = body->blood.getBGL();
        basalGLUT4.bloodMinBGL = body->blood.minGlucoseLevel;
    }
}

void Muscles::glycogenSynthesis()
{
    static std::poisson_distribution<int> glucoseToGlycogen__{1000.0 * glucoseToGlycogen};

    double x = static_cast<double>(glucoseToGlycogen__(body->generator()));
    glycogenSynthesizedPerTick = x * body->bodyWeight / 1000.0;
    glycogenSynthesizedPerTick = min({glycogenSynthesizedPerTick, glucose, (glycogenMax - glycogen)});

    glucose  -= glycogenSynthesizedPerTick;
    glycogen += glycogenSynthesizedPerTick;
}

void Muscles::glucoseToLactate(std::poisson_distribution<int>& glycolysisMin_)
{
    double x = static_cast<double>(glycolysisMin_(body->generator())) / 1000.0;
    glycolysisPerTick = min(glucose, body->glycolysis(x, glycolysisMax));

    glucose              -= glycolysisPerTick;
    body->blood.lactate += glycolysisPerTick;


    // double x = static_cast<double>(glycolysisMin_(body->generator())) / 1000.0;
    // glycolysisPerTick = body->glycolysis(x, glycolysisMax);

    // double toGlycolysis = glycolysisPerTick;
    // double fromGlucose = min(toGlycolysis, glucose);

    // glucose              -= fromGlucose;
    // body->blood.lactate += fromGlucose;
    // glycolysisPerTick     = fromGlucose;

    // double toGlycolysis -= fromGlucose;
    // if (toGlycolysis > 0)
    // {
    //     double fromGlycogen = min(toGlycolysis, glycogen);

    //     glycogen                 -= fromGlycogen;
    //     body->blood.lactate     += fromGlycogen;
    //     glycogenBreakdownPerTick += fromGlycogen;
    //     glycolysisPerTick        += fromGlycogen;

    // }
}

void Muscles::oxidation()
{
    // oxidationPerTick = 0.5 * body->blood.insulinLevel * glucoseAbsorbedPerTick;
    oxidationPerTick = glucose;
    glucose = 0;
}


void Muscles::fattyAcidConsumptionResting()
{
    // consume fat for the remaining energy needs during resting state
    double kcalgenerated = 0.004 * (oxidationPerTick + glycolysisPerTick / 15.0);
    // oxidation produces 15 times more energy than glycolysis

    if (kcalgenerated < currEnergyNeed)
        body->adiposeTissue.consumeFat(currEnergyNeed - kcalgenerated);
}

// void Muscles::aminoAcidConsumption()
// {
//     static std::poisson_distribution<int> baaToGlutamine__{1000.0 * baaToGlutamine};
//     double x = static_cast<double>(baaToGlutamine__(body->generator())) / 1000.0;
//     double a = min(x, body->blood.branchedAminoAcids);

//     body->blood.branchedAminoAcids -= a;
//     body->blood.glutamine          += a;
// }

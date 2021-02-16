#include "HumanBody.h"
#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include "common.h"
#include "SimCtl.h"

using namespace std;


HumanBody::HumanBody(SimCtl* sim,
                     map<BodyState, MetabolicParams> metabolicParameters,
                     map<unsigned, FoodType> foodTypes,
                     map<unsigned, ExerciseType> exerciseTypes)
    : sim{sim},
      foodTypes{move(foodTypes)},
      exerciseTypes{move(exerciseTypes)},
      metabolicParameters{move(metabolicParameters)}
{
    setParams(this->metabolicParameters[bodyState]);
}

double HumanBody::insulinImpactOnGlycolysis()
{
    return cdf(blood.insulinLevel, insulinImpactOnGlycolysis_Mean, insulinImpactOnGlycolysis_StdDev);
}

/********************************************
double HumanBody::insulinImpactOnGNG()
{
    return 1.0;
}
********************************************/

double HumanBody::insulinImpactOnGNG()
{
    assert(((void)"gngImpact less than 1", gngImpact >= 1.0));

    if (blood.insulinLevel >= blood.baseInsulinLevel)
        return 1.0 - cdf(blood.insulinLevel, insulinImpactOnGNG_Mean, insulinImpactOnGNG_StdDev);
    else
        return gngImpact - blood.insulinLevel * (gngImpact - 1.0) / blood.baseInsulinLevel;
}

double HumanBody::insulinImpactOnGlycogenBreakdownInLiver()
{
    assert(((void)"liverGlycogenBreakdownImpact less than 1", liverGlycogenBreakdownImpact >= 1.0));

    if (blood.insulinLevel >= blood.baseInsulinLevel)
        return 1.0 - cdf(blood.insulinLevel, insulinImpactGlycogenBreakdownInLiver_Mean, insulinImpactGlycogenBreakdownInLiver_StdDev);
    else
        return liverGlycogenBreakdownImpact - blood.insulinLevel * (liverGlycogenBreakdownImpact - 1.0) / blood.baseInsulinLevel;
}

double HumanBody::insulinImpactOnGlycogenSynthesisInLiver()
{
    return cdf(blood.insulinLevel, insulinImpactGlycogenSynthesisInLiver_Mean, insulinImpactGlycogenSynthesisInLiver_StdDev);
}

std::default_random_engine& HumanBody::generator()
{
    return sim->generator;
}

unsigned HumanBody::ticks()
{
    return sim->ticks();
}

double HumanBody::glycolysis(double min, double max)
{
    double max_ = max * bodyWeight * glycolysisMaxImpact;
    double min_ = min * bodyWeight * glycolysisMinImpact;
    min_ = std::min(min_, max_);

    return min_ + insulinImpactOnGlycolysis() * (max_ - min_);
}

// returns energy expenditure in kcal/minute
double HumanBody::currentEnergyExpenditure()
{
    return bodyWeight * currEnergyExpenditure;
}

void HumanBody::stomachEmpty()
{
    // oldState = bodyState;

    switch (bodyState)
    {
        case BodyState::FED_RESTING:
            bodyState = BodyState::POSTABSORPTIVE_RESTING;
            break;
        case BodyState::FED_EXERCISING:
            bodyState = BodyState::POSTABSORPTIVE_EXERCISING;
            break;
        default:
            break;
    }
}

double HumanBody::getGlucoseNeedsOutsideMuscles()
{
    return intestine.glycolysisPerTick
         + liver.glycolysisPerTick
         + kidneys.glycolysisPerTick
         + blood.glycolysisPerTick
         + brain.oxidationPerTick
         + heart.oxidationPerTick;
}

TotalsState HumanBody::getTotals()
{
    return {
        totalGlycolysisSoFar,
        totalExcretionSoFar,
        totalOxidationSoFar,
        totalGNGSoFar,
        totalLiverGlycogenStorageSoFar,
        totalLiverGlycogenBreakdownSoFar,
        totalMusclesGlycogenStorageSoFar,
        totalMusclesGlycogenBreakdownSoFar,
        totalGlucoseFromIntestineSoFar
    };
}

void HumanBody::resetTotals()
{
    totalGlycolysisSoFar               = 0;
    totalExcretionSoFar                = 0;
    totalOxidationSoFar                = 0;
    totalGNGSoFar                      = 0;
    totalLiverGlycogenStorageSoFar     = 0;
    totalLiverGlycogenBreakdownSoFar   = 0;
    totalMusclesGlycogenStorageSoFar   = 0;
    totalMusclesGlycogenBreakdownSoFar = 0;
    totalGlucoseFromIntestineSoFar     = 0;

    // dailyCarbs = 0;
}

void HumanBody::processTick()
{
    // Gerich: In terms of whole-body glucose economy, normally approximately 45% of ingested glucose is thought to be
    // converted to glycogen in the liver, 30% is taken up by skeletal muscle and later converted to glycogen,
    // 15% is taken up by the brain, 5% is taken up by the adipose tissue and 10% is taken up by the kidneys

    portalVein.processTick();
    stomach.processTick();
    intestine.processTick();
    liver.processTick();
    adiposeTissue.processTick();
    brain.processTick();
    heart.processTick();
    muscles.processTick();
    kidneys.processTick();
    blood.processTick();

    totalGlycolysisPerTick = intestine.glycolysisPerTick + liver.glycolysisPerTick + muscles.glycolysisPerTick
        + kidneys.glycolysisPerTick + blood.glycolysisPerTick;
    totalGlycolysisSoFar += totalGlycolysisPerTick;

    totalGNGPerTick = kidneys.gngPerTick + liver.gngPerTick;
    totalGNGSoFar  += totalGNGPerTick;

    totalOxidationPerTick = brain.oxidationPerTick + heart.oxidationPerTick + muscles.oxidationPerTick;
    totalOxidationSoFar  += totalOxidationPerTick;

    totalGlycogenStoragePerTick       = liver.toGlycogenPerTick + muscles.glycogenSynthesizedPerTick;
    totalLiverGlycogenStorageSoFar   += liver.toGlycogenPerTick;
    totalMusclesGlycogenStorageSoFar += muscles.glycogenSynthesizedPerTick;

    totalGlycogenBreakdownPerTick       = liver.fromGlycogenPerTick + muscles.glycogenBreakdownPerTick;
    totalLiverGlycogenBreakdownSoFar   += liver.fromGlycogenPerTick;
    totalMusclesGlycogenBreakdownSoFar += muscles.glycogenBreakdownPerTick;

    totalEndogeneousGlucoseReleasePerTick = liver.fromGlycogenPerTick + kidneys.gngPerTick + liver.gngPerTick;
    totalEndogeneousGlucoseReleaseSoFar  += totalEndogeneousGlucoseReleasePerTick;

    totalGlucoseReleasePerTick = intestine.toPortalVeinPerTick + liver.fromGlycogenPerTick
                               + kidneys.gngPerTick + liver.gngPerTick;
    totalGlucoseReleaseSoFar  += totalGlucoseReleasePerTick;

    totalExcretionSoFar            += kidneys.excretionPerTick;
    totalGlucoseFromIntestineSoFar += intestine.toPortalVeinPerTick;

    if (sim->dayOver())
    {
        dayEndTotals = getTotals();
        resetTotals();
    }

    if (bodyState == BodyState::FED_EXERCISING && sim->ticks() == exerciseOverAt)
    {
        bodyState = BodyState::FED_RESTING;
        currEnergyExpenditure = 1.0 / 60.0;
        percentVO2Max = 3.5 * 1.0 / vo2Max;
        // energy expenditure in resting state is 1 MET
        // setParams();
    }

    if (bodyState == BodyState::POSTABSORPTIVE_EXERCISING && sim->ticks() == exerciseOverAt)
    {
        bodyState = BodyState::POSTABSORPTIVE_RESTING;
        currEnergyExpenditure = 1.0 / 60.0;
        percentVO2Max = 3.5 * 1.0 / vo2Max;
        // setParams();
    }

    if (sim->ticks() == 600)
    {
        tempGNG               = totalGNGSoFar;
        tempGlycolysis        = totalGlycolysisSoFar;
        tempOxidation         = totalOxidationSoFar;
        tempExcretion         = kidneys.totalExcretion;
        tempGlycogenStorage   = totalLiverGlycogenStorageSoFar + totalMusclesGlycogenStorageSoFar;
        tempGlycogenBreakdown = totalLiverGlycogenBreakdownSoFar + totalMusclesGlycogenBreakdownSoFar;

        baseBGL = blood.getBGL();
        peakBGL = blood.getBGL();
    }

    if (sim->ticks() > 600)
        peakBGL = max(peakBGL, blood.getBGL());
}

void HumanBody::setParams(const MetabolicParams& params)
{
    // send new metabolic rates (based on the new state) to each organ

    /*
    Insulin resistance effects:

    1) Absorption of glucose by muscles and adipose tissue slows down
    2) Absorption of glucose by liver (to form glycogen) slows down
    3) glycogen breakdown and gluconeogenesis does not slow down even in presence of high insulin
    4) Glycerol release via lipolysis in adipose tissue does not slow down even in presence of high insulin
    */

    if (params.body.age >= 80)    throw out_of_range("Age 80 and above not supported.");
    if (params.body.age < 20)     throw out_of_range("Age below 20 not supported.");
    age = params.body.age;

    if (params.body.gender != 0 && params.body.gender != 1)    throw out_of_range("Invalid gender value.");
    gender = params.body.gender;


    fitnessLevel                                  = params.body.fitnessLevel;
    glut4Impact                                   = params.body.glut4Impact;
    glycolysisMinImpact                           = params.body.glycolysisMinImpact;
    glycolysisMaxImpact                           = params.body.glycolysisMaxImpact;
    excretionKidneysImpact                        = params.body.excretionKidneysImpact;
    liverGlycogenBreakdownImpact                  = params.body.liverGlycogenBreakdownImpact;
    liverGlycogenSynthesisImpact                  = params.body.liverGlycogenSynthesisImpact;
    // maxLiverGlycogenBreakdownDuringExerciseImpact = params.body.maxLiverGlycogenBreakdownDuringExerciseImpact;
    gngImpact                                     = params.body.gngImpact;

    bodyWeight         = params.body.bodyWeight;
    adiposeTissue.fat = fatFraction * bodyWeight * 1000.0;

    insulinImpactOnGlycolysis_Mean               = params.body.insulinImpactOnGlycolysis_Mean;
    insulinImpactOnGlycolysis_StdDev             = params.body.insulinImpactOnGlycolysis_StdDev;
    insulinImpactOnGNG_Mean                      = params.body.insulinImpactOnGNG_Mean;
    insulinImpactOnGNG_StdDev                    = params.body.insulinImpactOnGNG_StdDev;
    insulinImpactGlycogenBreakdownInLiver_Mean   = params.body.insulinImpactGlycogenBreakdownInLiver_Mean;
    insulinImpactGlycogenBreakdownInLiver_StdDev = params.body.insulinImpactGlycogenBreakdownInLiver_StdDev;
    insulinImpactGlycogenSynthesisInLiver_Mean   = params.body.insulinImpactGlycogenSynthesisInLiver_Mean;
    insulinImpactGlycogenSynthesisInLiver_StdDev = params.body.insulinImpactGlycogenSynthesisInLiver_StdDev;
    intensityPeakGlucoseProd                     = params.body.intensityPeakGlucoseProd;

    setVO2Max();


    stomach.setParams(params.stomach);
    intestine.setParams(params.intestine);
    portalVein.setParams(params.portalVein);
    liver.setParams(params.liver);

    // No parameters in Adipose Tissue
    adiposeTissue.setParams();

    brain.setParams(params.brain);
    heart.setParams(params.heart);
    muscles.setParams(params.muscles);
    blood.setParams(params.blood);
    kidneys.setParams(params.kidneys);
}

void HumanBody::setVO2Max()
{
    constexpr int upperAges[]     = {30, 40, 50, 60, 70, 80};
    constexpr int fitnessLevels[] = {5, 10, 25, 50, 75, 90};

    // depth: gender, rows: age, columns: fitnessLevel
    // mLO2 per kg per min
    constexpr double vo2Chart[2][6][7] = {
        {
            // male
            // <=5   <=10  <=25  <=50  <=75  <=90  >90
            {  29.0, 32.1, 40.1, 48.0, 55.2, 61.8, 66.3  }, // ages 20 - 29
            {  27.2, 30.2, 35.9, 42.4, 49.2, 56.5, 59.8  }, // ages 30 - 39
            {  24.2, 26.8, 31.9, 37.8, 45.0, 52.1, 55.6  }, // ages 40 - 49
            {  20.9, 22.8, 27.1, 32.6, 39.7, 45.6, 50.7  }, // ages 50 - 59
            {  17.4, 19.8, 23.7, 28.2, 34.5, 40.3, 43.0  }, // ages 60 - 69
            {  16.3, 17.1, 20.4, 24.4, 30.4, 36.6, 39.7  }  // ages 70 - 79
        },
        {
            // female
            // <=5   <=10  <=25  <=50  <=75  <=90  >90
            {  21.7, 23.9, 30.5, 37.6, 44.7, 51.3, 56.0  }, // ages 20 - 29
            {  19.0, 20.9, 25.3, 30.2, 36.1, 41.4, 45.8  }, // ages 30 - 39
            {  17.0, 18.8, 22.1, 26.7, 32.4, 38.4, 41.7  }, // ages 40 - 49
            {  16.0, 17.3, 19.9, 23.4, 27.6, 32.0, 35.9  }, // ages 50 - 59
            {  13.4, 14.6, 17.2, 20.0, 23.8, 27.0, 29.4  }, // ages 60 - 69
            {  13.1, 13.6, 15.6, 18.3, 20.8, 23.1, 24.1  }  // ages 70 - 79
        }
    };

    auto upperAge = upper_bound(begin(upperAges), end(upperAges), age);
    int row = distance(upperAges, upperAge);

    auto upperLevel = lower_bound(begin(fitnessLevels), end(fitnessLevels), fitnessLevel);
    int col = distance(fitnessLevels, upperLevel);

    vo2Max = vo2Chart[gender][row][col];

    percentVO2Max = 3.5 * 1.0 / vo2Max;
    // Assuming rest MET is 1.0
}

void HumanBody::processFoodEvent(unsigned foodID, unsigned howmuch)
{
    stomach.addFood(foodTypes[foodID], howmuch);

    // oldState = bodyState;

    switch (bodyState)
    {
        case BodyState::POSTABSORPTIVE_RESTING:
            bodyState = BodyState::FED_RESTING;
            break;
        case BodyState::POSTABSORPTIVE_EXERCISING:
            bodyState = BodyState::FED_EXERCISING;
            break;
        default:
            break;
    }
}

bool HumanBody::isExercising()
{
    return bodyState == BodyState::FED_EXERCISING || bodyState == BodyState::POSTABSORPTIVE_EXERCISING;
}

void HumanBody::processExerciseEvent(unsigned exerciseID, unsigned duration)
{
    // how much calorie would be consumed per minute for this exercise?
    // where would this much calorie come from?

    if (isExercising())    throw runtime_error("Exercise within Exercise!");

    assert(((void)"vo2Max not known", vo2Max > 0));

    percentVO2Max = 3.5 * exerciseTypes[exerciseID].intensity / vo2Max;
    if (percentVO2Max > 1.0)    throw runtime_error("Exercise intensity beyond the capacity of the user");

    // oldState = bodyState;

    currExercise = exerciseID;
    currEnergyExpenditure = exerciseTypes[exerciseID].intensity / 60.0;
    // intensity is in METs, where one MET is 1kcal/(kg.hr)

    switch (bodyState)
    {
        case BodyState::FED_RESTING:
            bodyState = BodyState::FED_EXERCISING;
            exerciseOverAt = sim->ticks() + duration;

            if (exerciseTypes[exerciseID].intensity >= 6.0)
                lastHardExerciseAt = static_cast<int>(exerciseOverAt);
            break;

        case BodyState::POSTABSORPTIVE_RESTING:
            bodyState = BodyState::POSTABSORPTIVE_EXERCISING;
            exerciseOverAt = sim->ticks() + duration;

            if (exerciseTypes[exerciseID].intensity >= 6.0)
                lastHardExerciseAt = static_cast<int>(exerciseOverAt);
            break;
        default:
            break;
    }
}

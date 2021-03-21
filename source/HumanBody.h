#pragma once

#include <map>
#include <memory>
#include <string>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "Brain.h"
#include "common.h"
#include "Heart.h"
#include "Intestine.h"
#include "Kidneys.h"
#include "Liver.h"
#include "Muscles.h"
#include "PortalVein.h"
#include "Stomach.h"

class SimCtl;


struct FoodType
{
    unsigned    foodID;
    std::string name;
    double      servingSize; // in grams
    double      RAG;         // rapidly available glucose (in grams)
    double      SAG;         // slowly available glucose (in grams)
    double      protein;     // in grams
    double      fat;         // in grams
};

struct ExerciseType
{
    unsigned    exerciseID;
    std::string name;
    double      intensity; // intensity in METs
};


enum class BodyState {FED_RESTING, FED_EXERCISING, POSTABSORPTIVE_RESTING, POSTABSORPTIVE_EXERCISING};


struct TotalsState
{
    double glycolysis;
    double excretion;
    double oxidation;
    double GNG;
    double liverGlycogenStorage;
    double liverGlycogenBreakdown;
    double musclesGlycogenStorage;
    double musclesGlycogenBreakdown;
    double glucoseFromIntestine;
};

struct HumanParams
{
    double glut4Impact                  = 1.0;
    double liverGlycogenSynthesisImpact = 1.0;
    // double maxLiverGlycogenBreakdownDuringExerciseImpact;
    double glycolysisMinImpact    = 1.0;
    double glycolysisMaxImpact    = 1.0;
    double excretionKidneysImpact = 1.0;

    double age;          // in years
    double gender;       // 0 male, 1 female
    double fitnessLevel; // between 0 and 100

    double bodyWeight;
    // double bodyWeight = 65; // kg
    // must specify body weight in the params

    double intensityPeakGlucoseProd = 0.2; // exercise intensity in %VO2Max at which peak GNG, glycogen breakdown takes place

    double gngImpact                    = 6.0;
    double liverGlycogenBreakdownImpact = 6.0;

    double insulinImpactOnGlycolysis_Mean                = 0.5;
    double insulinImpactOnGlycolysis_StdDev              = 0.2;
    double insulinImpactOnGNG_Mean                       = 0.5;
    double insulinImpactOnGNG_StdDev                     = 0.2;
    double insulinImpactGlycogenBreakdownInLiver_Mean    = 0.1;
    double insulinImpactGlycogenBreakdownInLiver_StdDev  = 0.02;
    double insulinImpactGlycogenSynthesisInLiver_Mean    = 0.5;
    double insulinImpactGlycogenSynthesisInLiver_StdDev  = 0.2;
};

struct MetabolicParams
{
    BloodParams      blood;
    BrainParams      brain;
    HeartParams      heart;
    HumanParams      body;
    IntestineParams  intestine;
    KidneysParams    kidneys;
    LiverParams      liver;
    MusclesParams    muscles;
    PortalVeinParams portalVein;
    StomachParams    stomach;
};

class HumanBody
{
public:
    HumanBody(SimCtl* sim,
              std::map<BodyState, MetabolicParams> metabolicParameters,
              std::map<unsigned, FoodType> foodTypes,
              std::map<unsigned, ExerciseType> exerciseTypes);

    void setParams(const MetabolicParams& params); // send organs their new rates when the state changes

    BodyState bodyState = BodyState::POSTABSORPTIVE_RESTING;
    // BodyState oldState = BodyState::POSTABSORPTIVE_RESTING;

    double glut4Impact;
    double liverGlycogenSynthesisImpact;
    // double maxLiverGlycogenBreakdownDuringExerciseImpact;
    double excretionKidneysImpact;

    int    age;           // in years
    int    gender;        // 0 male, 1 female
    int    fitnessLevel;  // between 0 and 100
    double VO2Max = 0;    // estimated from age, gender and fitnessLevel
    double percentVO2Max; // for the current exercise

    double bodyWeight;

    double   fatFraction        = 0.2;
    unsigned exerciseOverAt     = 0;   // when does the current exercise event get over
    int      lastHardExerciseAt = -61; // when was the last "hard" exercise

    AdiposeTissue adiposeTissue{this};
    Blood         blood{this};
    Brain         brain{this};
    Heart         heart{this};
    Intestine     intestine{this};
    Kidneys       kidneys{this};
    Liver         liver{this};
    Muscles       muscles{this};
    PortalVein    portalVein{this};
    Stomach       stomach{this};

    void   processTick();
    void   processFoodEvent(unsigned foodID, unsigned howmuch);
    void   processExerciseEvent(unsigned exerciseID, unsigned duration);
    void   stomachEmpty();
    bool   isExercising() const;
    double currentEnergyExpenditure();
    double getGlucoseNeedsOutsideMuscles();

    double totalGlycolysisSoFar               = 0;
    double totalGNGSoFar                      = 0;
    double totalOxidationSoFar                = 0;
    double totalLiverGlycogenStorageSoFar     = 0;
    double totalLiverGlycogenBreakdownSoFar   = 0;
    double totalMusclesGlycogenStorageSoFar   = 0;
    double totalMusclesGlycogenBreakdownSoFar = 0;
    double totalEndogeneousGlucoseReleaseSoFar;
    double totalGlucoseReleaseSoFar;
    void resetTotals();
    TotalsState getTotals() const;

    double glycolysis(double min, double max) const;

    double insulinImpactOnGlycolysis() const;
    double insulinImpactOnGNG() const;
    double insulinImpactOnGlycogenSynthesisInLiver() const;
    double insulinImpactOnGlycogenBreakdownInLiver() const;

	double tempGNG;
	double tempGlycolysis;
	double tempOxidation;
	double tempExcretion;
	double tempGlycogenStorage;
	double tempGlycogenBreakdown;

	double baseBGL;
	double peakBGL;

    double intensityPeakGlucoseProd; // exercise intensity in %VO2Max at which peak GNG, glycogen breakdown takes place

	double insulinImpactOnGNG_Mean;
	double insulinImpactGlycogenBreakdownInLiver_Mean;

    std::default_random_engine& generator();
    unsigned ticks() const;

    // Diagnostics
    TotalsState dayEndTotals;
    double      totalGlycolysisPerTick;
    double      totalGNGPerTick;
    double      totalOxidationPerTick;
    double      totalGlycogenStoragePerTick;
    double      totalGlycogenBreakdownPerTick;
    double      totalEndogeneousGlucoseReleasePerTick;
    double      totalGlucoseReleasePerTick;
    // BodyState   oldState;

private:
    SimCtl* sim;

    std::map<unsigned, FoodType>         foodTypes;
    std::map<unsigned, ExerciseType>     exerciseTypes;
    std::map<BodyState, MetabolicParams> metabolicParameters;

    void setVO2Max();

    double gngImpact;
    double liverGlycogenBreakdownImpact;

	double insulinImpactOnGlycolysis_Mean;
	double insulinImpactOnGlycolysis_StdDev;
	double insulinImpactGlycogenSynthesisInLiver_Mean;
	double insulinImpactGlycogenSynthesisInLiver_StdDev;
	double insulinImpactOnGNG_StdDev;
	double insulinImpactGlycogenBreakdownInLiver_StdDev;

    double glycolysisMinImpact;
    double glycolysisMaxImpact;

    unsigned currExercise          = 0;
    double   currEnergyExpenditure = 1.0 / 60.0; // current energy expenditure in kcal/minute per kg of body weight
    // energy expenditure in resting state is 1 MET

    double totalExcretionSoFar            = 0;
    double totalGlucoseFromIntestineSoFar = 0;

    // double dailyCarbs;
};

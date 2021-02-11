#pragma once

#include <map>
#include <memory>
#include <string>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "Brain.h"
#include "Heart.h"
#include "Intestine.h"
#include "Kidneys.h"
#include "Liver.h"
#include "Muscles.h"
#include "PortalVein.h"
#include "SimCtl.h"
#include "Stomach.h"


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
enum class BodyOrgan {HUMAN_BODY, STOMACH, INTESTINE, PORTAL_VEIN, LIVER, BLOOD,
                      MUSCLES, BRAIN, HEART, ADIPOSE_TISSUE, KIDNEY};


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

struct TotalsState
{
    double totalGlycolysisSoFar;
    double totalExcretionSoFar;
    double totalOxidationSoFar;
    double totalGNGSoFar;
    double totalLiverGlycogenStorageSoFar;
    double totalLiverGlycogenBreakdownSoFar;
    double totalMusclesGlycogenStorageSoFar;
    double totalMusclesGlycogenBreakdownSoFar;
    double totalGlucoseFromIntestineSoFar;
};

struct HumanParams
{
    double age;
    double gender;
    double fitnessLevel;
    double glut4Impact;
    double glycolysisMinImpact;
    double glycolysisMaxImpact;
    double excretionKidneysImpact;
    double liverGlycogenBreakdownImpact;
    double liverGlycogenSynthesisImpact;
    double maxLiverGlycogenBreakdownDuringExerciseImpact;
    double gngImpact;
    double bodyWeight;
    double insulinImpactOnGlycolysis_Mean;
    double insulinImpactOnGlycolysis_StdDev;
    double insulinImpactOnGNG_Mean;
    double insulinImpactOnGNG_StdDev;
    double insulinImpactGlycogenBreakdownInLiver_Mean;
    double insulinImpactGlycogenBreakdownInLiver_StdDev;
    double insulinImpactGlycogenSynthesisInLiver_Mean;
    double insulinImpactGlycogenSynthesisInLiver_StdDev;
    double intensityPeakGlucoseProd;
};

struct MetabolicParams
{
    BloodParams      bloodParams;
    BrainParams      brainParams;
    HeartParams      heartParams;
    HumanParams      humanParams;
    IntestineParams  intestineParams;
    KidneysParams    kidneysParams;
    LiverParams      liverParams;
    MusclesParams    musclesParams;
    PortalVeinParams portalVeinParams;
    StomachParams    stomachParams;
};

class HumanBody
{
public:
    HumanBody(std::shared_ptr<SimCtl> sim,
              std::map<BodyState, MetabolicParams> metabolicParameters,
              std::map<unsigned, FoodType> foodTypes,
              std::map<unsigned, ExerciseType> exerciseTypes);

    void setParams(const MetabolicParams& params); // send organs their new rates when the state changes

    BodyState bodyState = BodyState::POSTABSORPTIVE_RESTING;
    // BodyState oldState = BodyState::POSTABSORPTIVE_RESTING;

    double glut4Impact                  = 1.0;
    double liverGlycogenSynthesisImpact = 1.0;
    // double maxLiverGlycogenBreakdownDuringExerciseImpact;
    double excretionKidneysImpact       = 1.0;

    int    age;           // in years
    int    gender;        // 0 male, 1 female
    int    fitnessLevel;  // between 0 and 100
    double vo2Max = 0;    // estimated from age, gender and fitnessLevel
    double percentVO2Max; // for the current exercise

    double bodyWeight;
    // double bodyWeight = 65; // kg
    // must specify body weight in the params
    double   fatFraction        = 0.2;
    unsigned exerciseOverAt     = 0;   // when does the current exercise event get over
    int      lastHardExerciseAt = -61; // when was the last "hard" exercise

    std::unique_ptr<Stomach>       stomach       = std::make_unique<Stomach>(this);
    std::unique_ptr<Intestine>     intestine     = std::make_unique<Intestine>(this);
    std::unique_ptr<PortalVein>    portalVein    = std::make_unique<PortalVein>(this);
    std::unique_ptr<Liver>         liver         = std::make_unique<Liver>(this);
    std::unique_ptr<AdiposeTissue> adiposeTissue = std::make_unique<AdiposeTissue>(this);
    std::unique_ptr<Brain>         brain         = std::make_unique<Brain>(this);
    std::unique_ptr<Muscles>       muscles       = std::make_unique<Muscles>(this);
    std::unique_ptr<Blood>         blood         = std::make_unique<Blood>(this);
    std::unique_ptr<Heart>         heart         = std::make_unique<Heart>(this);
    std::unique_ptr<Kidneys>       kidneys       = std::make_unique<Kidneys>(this);

    void   processTick();
    void   processFoodEvent(unsigned foodID, unsigned howmuch);
    void   processExerciseEvent(unsigned exerciseID, unsigned duration);
    void   stomachEmpty();
    bool   isExercising();
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
    TotalsState getTotals();

    double glycolysis(double min, double max);

    double insulinImpactOnGlycolysis();
    double insulinImpactOnGNG();
    double insulinImpactOnGlycogenSynthesisInLiver();
    double insulinImpactOnGlycogenBreakdownInLiver();

	double tempGNG;
	double tempGlycolysis;
	double tempOxidation;
	double tempExcretion;
	double tempGlycogenStorage;
	double tempGlycogenBreakdown;

	double baseBGL;
	double peakBGL;

    double intensityPeakGlucoseProd = 0.2; // exercise intensity in %VO2Max at which peak GNG, glycogen breakdown takes place

	double insulinImpactOnGNG_Mean                    = 0.5;
	double insulinImpactGlycogenBreakdownInLiver_Mean = 0.1;

    std::default_random_engine& generator();
    unsigned ticks();

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
    std::shared_ptr<SimCtl> sim;

    std::map<unsigned, FoodType>         foodTypes;
    std::map<unsigned, ExerciseType>     exerciseTypes;
    std::map<BodyState, MetabolicParams> metabolicParameters;

    void setVO2Max();

    double gngImpact                    = 6.0;
    double liverGlycogenBreakdownImpact = 6.0;

	double insulinImpactOnGlycolysis_Mean               = 0.5;
	double insulinImpactGlycogenSynthesisInLiver_Mean   = 0.5;
	double insulinImpactOnGlycolysis_StdDev             = 0.2;
	double insulinImpactOnGNG_StdDev                    = 0.2;
	double insulinImpactGlycogenBreakdownInLiver_StdDev = 0.02;
	double insulinImpactGlycogenSynthesisInLiver_StdDev = 0.2;

    double glycolysisMinImpact = 1.0;
    double glycolysisMaxImpact = 1.0;

    unsigned currExercise          = 0;
    double   currEnergyExpenditure = 1.0 / 60.0; // current energy expenditure in kcal/minute per kg of body weight
    // energy expenditure in resting state is 1 MET

    double totalExcretionSoFar            = 0;
    double totalGlucoseFromIntestineSoFar = 0;

    // double dailyCarbs;
};

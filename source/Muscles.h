#pragma once

#include <random>
#include "common.h"

class HumanBody;


struct MusclesParams
{
    double glycogen = 500 * 1000.0;

    // glycogenMax = 0.4*(body->bodyWeight)*15000.0; // 40% of body weight is muscles
    // glycogen storing capacity of muscles: 15g/kg of wet muscle weight
    // Frayn Chapter 9
    double glycogenMax = 500 * 1000.0;

    double glycogenShareDuringExerciseMean = 0.53;

    // GLUT1 is also expressed in skeletal muscle and may play a role in uptake of a glucose at a “basal” rate. Muscle
    // glucose uptake averaged 1.91 +- 0.23 micromol/kg/min before glucose ingestion and accounted for 22.0 +- 3.7% of
    // systemic glucose disposal.
    // "Splanchnic and Leg Substrate Exchange After Ingestion of a Natural Mixed Meal in Humans".

    // 1.91 micromol of glucose translates to 1.91*180.1559/1000 mg = 0.344 mg

    // double basalGlucoseAbsorbed = 1.91 * 0.1801559; // mg per kg body weight per minute
    double basalGlucoseAbsorbed = 0; // mg per kg body weight per minute

    double maxGlucoseAbsorptionDuringExercise = 30.0 * 0.1801559;
    double baaToGlutamine = 0;

    // See the explanation in processTick()
    double glycolysisMin = 0.35 * 1.0 * 0.1801559; // mg per kg per minute
    double glycolysisMax = 0.9 * 0.35 * 15.0 * 0.1801559; // mg per kg per minute

    double glucoseToGlycogen = 15.0 * 0.1801559; // 15 micromol per kg per minute;

    double Glut4Km       = 5 * 180.1559 / 10.0; // mg/dl equivalent of 5 mmol/l
    double Glut4VMAX     = 3.5; // mg per kg per minute
    double peakGlut4VMAX = 2 * 3.5; // mg per kg per minute
};

class Muscles
{
public:
    Muscles(HumanBody* myBody);

    void processTick();
    void setParams(const MusclesParams& params);

    double glucoseAbsorbedPerTick;
    double glycogenSynthesizedPerTick;
    double glycogenBreakdownPerTick;
    double oxidationPerTick;
    double glycogenOxidizedPerTick;
    double glycolysisPerTick;

    // Diagnostics
    bool         hadExercise        = false;
    double       absorbedFromBlood  = 0;
    GlucoseState absorptionState;
    double       glycogenShare;
    GlucoseState basalBase;
    GlucoseState basalGLUT4;
    bool         basalGLUT4Occurred = false;

    double glycogen;

private:
    HumanBody* body;

    double glycogenMax;
    double glycogenShareDuringExerciseMean;
    double glucose = 0; // mg
    double volume  = 250; // in dl

    double basalGlucoseAbsorbed;
    double maxGlucoseAbsorptionDuringExercise;
    double baaToGlutamine;
    double glycolysisMin;
    double glycolysisMax;

    double glucoseToGlycogen;

    double Glut4Km;
    double Glut4VMAX;     // mg per kg per minute
    double peakGlut4VMAX; // mg per kg per minute

    double totalGlucoseAbsorbed = 0;
    double currEnergyNeed;

    void glucoseAbsorption(std::poisson_distribution<int>& rand);
    void glycogenToGlucose(std::poisson_distribution<int>& rand);
    void glycogenToLactate(std::poisson_distribution<int>& glycolysisMin_);
    void fattyAcidConsumptionActive();
    void basalAbsorption(std::poisson_distribution<int>& rand);
    void glycogenSynthesis();
    void glucoseToLactate(std::poisson_distribution<int>& glycolysisMin_);
    void oxidation();
    void fattyAcidConsumptionResting();
    // void aminoAcidConsumption();
};

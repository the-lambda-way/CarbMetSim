#pragma once

#include <random>

class HumanBody;


struct LiverParams
{
    double glycogen    = 100.0 * 1000.0; // equivalent of 100g of glucose
    double glycogenMax = 120.0 * 1000.0; // 120 g of glucose

    // Frayn Chapter 9
    // 5 micromol per kg per minute = 5*180.1559/1000 mg per kg per minute (ref: Gerich paper)
	// default max glycogen breakdown rate is 5 micromol per kg per minute
    // glycogenToGlucose = 5.5 * 0.1801559;
    // glucoseToGlycogen = 33.0 * 0.1801559;
    double glycogenToGlucoseInLiver = 0.9; // in units of mg of glucose per kg per minute
    double glucoseToGlycogenInLiver = 4.5;

    // double maxLipogenesis = 1000.0;
    double maxLipogenesis = 400.0; // in units of mg per minute

    // Gerich paper: Liver consumes 1 micromol per kg per minute to 16.5 micromol per kg per minute of glucose depending
    // upon post-absorptive/post-prandial state.
    double glycolysisMin = 0.35 * 0.1801559; // mg per kg per minute
    double glycolysisMax = 0.9 * 0.35 * 10 * 0.1801559; // mg per kg per minute

    double glycolysisToLactateFraction = 1; // by default glycolysis just generates all lactate

    // 1 micromol per kg per minute = 0.1801559 mg per kg per minute
    // double micromol = 0.1801559;
    // gng = 0.42 * 4.0 * micromol;
    double gngLiver = 0.16;

    double glucoseToNEFA = 0;

    double fluidVolume = 12; // dl; Meyer paper on gluconeogenesis did measurements on liver volume
    double Glut2Km     = 20 * 180.1559 / 10.0; // mg/deciliter equal to 20 mmol/l (Frayn Table 2.2.1)
    double Glut2VMAX   = 50; // mg per kg per minute
};

// struct LipogenesisState
// {
//     double bodyWeight;
//     double toLipogenesis;
//     double fat;
// };

struct GlycogenSynthesisState
{
    double glycogen;
    double glucose;
};

struct GluconeogenesisState
{
    double glucose;
    double glycogen;
    double bloodGlucose;
    double bloodLactate;
};

class Liver
{
public:
    Liver(HumanBody* body);
    void processTick();
    void setParams(const LiverParams& params);

    double glycogen;
    double fluidVolume = 12; // dl; Meyer paper on gluconeogenesis did measurements on liver volume
    double glucose     = 100 * fluidVolume; // assuming glucose concentration to be 100mg/dl

    double absorptionPerTick;
    double toGlycogenPerTick;
    double fromGlycogenPerTick;
    double glycolysisPerTick;
    double gngPerTick;
    double releasePerTick;

    // Diagnostics
    double excessGlucoseAbsorption = 0;
    double glucoseNeeded           = 0;
    double maxGNGDuringExercise    = 0;
    bool   lipogenesisOccurred     = false;
    // LipogenesisState       preLiposgenesis;
    // LipogenesisState       postLipogenesis;
    GlycogenSynthesisState postGlycogen;
    GluconeogenesisState   postGluconeogenesis;

private:
    HumanBody* body;

    double glycogenMax;

    double glucoseToGlycogenInLiver;
    double glycogenSynth_Insulin_Mean;
    double glycogenSynth_Insulin_StdDev;

    double glycogenToGlucoseInLiver; // in units of mg of glucose per kg per minute
    double maxLipogenesis; // in units of mg per minute

    double glycolysisMin; // mg per kg per minute
    double glycolysisMax;

    double glycolysisToLactateFraction;

    double gngLiver;

    double glucoseToNEFA;

    double Glut2Km;
    double Glut2VMAX;

    void glucoseAbsorption(std::poisson_distribution<int>& Glut2VMAX_);
    void glycogenSynthesis();
    void glycolysis(std::poisson_distribution<int>& rand);
    void glycogenBreakdown(std::poisson_distribution<int>& rand);
    void aminoAcidConsumption(std::poisson_distribution<int>& Glut2VMAX_);
    // void glucoseHomeostasis();
};

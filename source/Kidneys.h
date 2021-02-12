#pragma once

#include "common.h"

class HumanBody;


struct KidneysParams
{
    double glycolysisMin;
    double glycolysisMax;
    double gngKidneys;
};

class Kidneys
{
public:
    Kidneys(HumanBody* myBody);

    void processTick();
    void setParams(const KidneysParams& params);

    // As BGL increases from 11 mmol/l to 22 mmol/l, glucose excretion in urine increases
    // from 0 mg/min to 100mg/min.
	double totalExcretion = 0;

    double glycolysisPerTick;
    double gngPerTick;
    double excretionPerTick;

    // Diagnostics
    double       toGlycolysis = 0;
    GlucoseState postGlycolysis;
    GlucoseState postGluconeogenesis;
    GlucoseState postGlucoseExtraction;

private:
    HumanBody* body;

    // Gerich: insulin dependent: 1 to 5 micromol per kg per minute
    double glycolysisMin = 0.35 * 0.5 * 0.1801559; // mg per kg per minute
    double glycolysisMax = 0.9 * 0.35 * 2.0 * 0.1801559; // mg per kg per minute

    // 1 micromol per kg per minute = 0.1801559 mg per kg per minute
    // double micromol = 0.1801559;
    // gng = 0.42 * 2.2 * micromol;
    double gngKidneys = 0.16;

    double reabsorptionThreshold = 11 * 180.1559 / 10; // mg/dl equivalent of 11 mmol/l
    double glucoseExcretionRate  = 100 / (11 * 180.1559 / 10); // mg per minute per(mg/dl)

    double absorptionPerTick;
    double releasePerTick;

    void glycolysis();
    void gluconeogenesis();
    void glucoseExcretionInUrine();
};

#pragma once

#include "common.h"

class HumanBody;


struct BrainParams
{
    // 6 micromol per kg per minute = 6*180.1559/1000 mg per kg per minute = 1.08 mg per kg per minute
	// 120 g per day = 83.333 mg per minute of glucose oxidized by brain
    double glucoseOxidized  = 83.333; // mg per minute
    double glucoseToAlanine = 0;
    double bAAToGlutamine   = 0;
};

class Brain
{
public:
    Brain(HumanBody* body);
    void processTick();
    void setParams(const BrainParams& params);
    // void glucoseToLactate(int rate);

    double oxidationPerTick;

    // Diagnostics
    GlucoseState glucoseRemoved;

private:
    HumanBody* body;

    double glucoseOxidized;
    double glucoseToAlanine;
    double bAAToGlutamine;

    void oxidizeGlucose();
    void glutamineFromBlood();
};

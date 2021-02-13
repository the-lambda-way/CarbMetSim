#pragma once

#include "common.h"

class HumanBody;


struct PortalVeinParams
{
    double fluidVolume = 5; // dl
};

class PortalVein
{
public:
    PortalVein(HumanBody* body);
    void processTick();
    void setParams(const PortalVeinParams& params);
    double getConcentration() const;
    void addGlucose(double g);
    double getGlucose() const;
    void removeGlucose(double g);
    void releaseAllGlucose();
    void addAminoAcids(double aa);
    void releaseAminoAcids();

    // Diagnostics
    GlucoseState fromBlood;

private:
    HumanBody* body;
    
    double glucose      = 0; // mg
    double branchedAA   = 0; // mg
    double unbranchedAA = 0; // mg
    double fluidVolume;
};

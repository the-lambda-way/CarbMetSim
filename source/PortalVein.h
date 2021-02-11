#pragma once

class HumanBody;
class GlucoseState;


struct PortalVeinParams
{
    double fluidVolume;
};

class PortalVein
{
public:
    PortalVein(HumanBody* body);
    void processTick();
    void setParams(const PortalVeinParams& params);
    double getConcentration();
    void addGlucose(double g);
    double getGlucose();
    void removeGlucose(double g);
    void releaseAllGlucose();
    void addAminoAcids(double aa);
    void releaseAminoAcids();

    // Diagnostics
    GlucoseState fromBlood;

private:
    double glucose      = 0; // mg
    double branchedAA   = 0; // mg
    double unbranchedAA = 0; // mg
    double fluidVolume  = 5; // dl
    HumanBody* body;
};

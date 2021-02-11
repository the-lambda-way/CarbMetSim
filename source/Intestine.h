#pragma once

#include <vector>

class HumanBody;
class GlucoseState;


struct Chyme // undigested carbs chyme
{
    double   origRAG;
    double   origSAG;
    double   RAG;
    double   SAG;
    unsigned ts; // time (in ticks) when the chyme entered the intestine
};

struct ChymeConsumed
{
    double RAG;
    double SAG;
    double origRAG;
    double origSAG;
    double RAGConsumed;
    double SAGConsumed;
};

struct IntestineParams
{
    double aminoAcidsAbsorptionRate;
    double glutamineOxidationRate;
    double glutamineToAlanineFraction;
    double Glut2VMAX_In;
    double Glut2Km_In;
    double Glut2VMAX_Out;
    double Glut2Km_Out;
    double sglt1Rate;
    double fluidVolumeInLumen;
    double fluidVolumeInEnterocytes;
    double glycolysisMin;
    double glycolysisMax;
    double RAG_Mean;
    double RAG_StdDev;
    double SAG_Mean;
    double SAG_StdDev;
};

class Intestine
{
public:
    Intestine(HumanBody* body);
    void setParams(const IntestineParams& params);
    void processTick();
    void addChyme(double rag, double sag, double proteinInChyme, double fat);

    double glycolysisPerTick;
    double toPortalVeinPerTick;

    // Diagnostics
    std::vector<ChymeConsumed> chymeConsumed;
    double totalRAGConsumed           = 0;
    double totalSAGConsumed           = 0;
    double activeAbsorption           = 0;
    double passiveAbsorption          = 0;
    double glLumen                    = 0;
    double glEnterocytes              = 0;
    double glPortalVein               = 0;
    double glPortalVeinConcentration  = 0;
    GlucoseState excessGlucoseInEnterocytes;

private:
    HumanBody* body;

    double totalRAGDigested = 0;
    double totalSAGDigested = 0;

    std::vector<Chyme> chyme;
    double protein = 0; // mg

    double glucoseInLumen       = 0; // in milligrams
    double glucoseInEnterocytes = 0; // in milligrams

    // Carb digestion parameters
    // support only normal distribution for RAG/SAG digestion so far.
    double fluidVolumeInEnterocytes = 3; // in deciliters
    double fluidVolumeInLumen       = 4; // in deciliters

    // Michaelis Menten parameters for glucose transport
    double Glut2Km_In    = 20 * 180.1559 / 10.0; // mg/deciliter equal to 20 mmol/l (Frayn Table 2.2.1)
    double Glut2VMAX_In  = 800; // mg
    double Glut2Km_Out   = 20 * 180.1559 / 10.0; // mg/deciliter equal to 20 mmol/l (Frayn Table 2.2.1)
    double Glut2VMAX_Out = 800; // mg
    // active transport rate
    double sglt1Rate = 30; // mg per minute;
    double peakGlucoseConcentrationInLumen = 200 * 180.1559 / 10.0; // mg/dl equivalent of 200mmol/l

    // glycolysis
    // Gerich: insulin dependent: 0.5 to 5 micromol per kg per minute
    double glycolysisMin = 0.35 * 0.5 * 0.1801559;
    double glycolysisMax = 0.9 * 0.35 * 2.0 * 0.1801559;

    // Amino Acid Absorption
    double aminoAcidsAbsorptionRate   = 1; //mg per minute;
    double glutamineOxidationRate     = 1; // mg per minute;
    double glutamineToAlanineFraction = 0.5;

    // Carb digestion parameters
    // support only normal distribution for RAG/SAG digestion so far.
    double RAG_Mean   = 2;
    double RAG_StdDev = 0.5;
    double SAG_Mean   = 30;
    double SAG_StdDev = 10;

    void absorbGlucose();
    double consumeGlucose(double& glucose, double origGlucose, double mean, double stddev, double deltaTicks);
    void absorbAminoAcids();
    void digestChyme();
};

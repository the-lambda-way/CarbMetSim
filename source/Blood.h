#pragma once

#include "common.h"

class HumanBody;


struct RBCBin
{
    double RBCs;
    double glycatedRBCs;
};

struct BloodParams
{
    double rbcBirthRate; // how many million RBCs take birth each minute
    double glycationProbSlope = 0.085 / 10000.0; // g*l_i + c is the probability that an unglycated RBC glycates during a minute
    double glycationProbConst = 0;

    // Gerich: insulin dependent: 0.5 to 5 micromol per kg per minute
    double glycolysisMin = 0.35 * 0.5 * 0.1801559;
    // double glycolysisMax = 0.35 * 5 * 0.1801559;
    double glycolysisMax = 0.9 * 0.35 * 2 * 0.1801559;

    double glycolysisToLactate = 1.0; // what fraction of glycolysed glucose becomes lactate?

    double minGlucoseLevel  = 40; // mg/dl
    double baseGlucoseLevel = 100; // mg/dl
    double highGlucoseLevel = 200; // mg/dl
    double highLactateLevel = 4053.51; // mg
    // 9 mmol/l of lactate = 4.5 mmol/l of glucose = 4.5*180.1559*5 mg of glucose = 4053.51mg of glucose
    double peakInsulinLevel = 1.0;

    // double baseInsulinLevel = 1.0 / 6.0;
    double baseInsulinLevel = 0;
};

class Blood
{
    static const int ONE_DAY = 24 * 60;
    static const int MAX_AGE = 120; // minutes in 120 days
    // static const int INSULIN_DELAY = 15;
    static const int HUNDRED_DAYS = 100; // minutes in 100 days

    double baseGlucoseLevel = 100;
    double fluidVolume      = 50.0; // in deciliters

public:
    Blood(HumanBody* body);

    // Red Blood cells use glucose during glycolysis and produce lactate
    void processTick();
    void setParams(const BloodParams& params);

    void removeGlucose(double howmuch);
    void addGlucose(double howmuch);

    double getBGL() const;
    double getGNGSubstrates() const;
    double baseBGL() const;
    double highBGL() const;
    double volume() const;
    double gngFromHighLactate(double rate);
    void updateInsulinLevel();
    
    double minGlucoseLevel;

    // All the metabolites are in units of milligrams of glucose
    double glucose              = baseGlucoseLevel * fluidVolume; // in milligrams
    double baseInsulinLevel     = 0;
    double insulinLevel         = baseInsulinLevel;
    double lactate              = 450.39; // mg
    // 1mmol/l of lactate = 0.5mmol/l of glucose = 0.5*180.1559*5 mg of glucose = 450.39 mg of glucose
    double branchedAminoAcids   = 0;
    // see Table 2.2 in Frayn
    double glutamine            = 0;
    double alanine              = 0;
    double unbranchedAminoAcids = 0;
    double gngSubstrates        = 0; // glycerol and other gng substrates (not including lactate, glutamine and alanine), all in units of glucose
    double glycolysisPerTick    = 0; // keep track of the glucose consumed via glycolysis this tick
    double totalGlycolysisSoFar = 0.0;

    // Diagnostics
    double killRate[MAX_AGE - HUNDRED_DAYS];
    int    killBin[MAX_AGE - HUNDRED_DAYS];
    bool   RBCsUpdated   = false;
    double glLumen       = 0;
    double glEnterocytes = 0;
    double glPortalVein  = 0;

private:
    HumanBody* body;

    RBCBin ageBins[MAX_AGE + 1]; // Aging Bins
    int bin0 = 1; // Current age 0 bin

    double rbcBirthRate = 144.0 * 60 * 24; // how many million RBCs take birth each day (144 million RBCs take birth every minute)
    double glycationProbSlope; // g*l_i + c is the probability that an unglycated RBC glycates during a minute
    double glycationProbConst;
    double glycolysisMin;
    double glycolysisMax;
    double glycolysisToLactate;

    double currentHbA1c() const;
    void updateRBCs();

    double highGlucoseLevel;
    double highLactateLevel;
    double peakInsulinLevel;

    double avgBGL            = 100.0;
    double avgBGLOneDay      = 0;
    double avgBGLOneDaySum   = 0;
    double avgBGLOneDayCount = 0;

    void glycolysis();
};

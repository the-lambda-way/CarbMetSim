#ifndef __HumanBody__Blood__
#define __HumanBody__Blood__

#include "HumanBody.h"

#define ONEDAY 24*60

#define MAXAGE 120
// minutes in 120 days

//#define INSULIN_DELAY 15

#define HUNDREDDAYS 100
// minutes in 100 days

struct RBCBin {
    double 	RBCs;
    double	glycatedRBCs;
};

class Blood
{
    RBCBin AgeBins[MAXAGE+1];// Aging Bins
    int bin0;	// Current age 0 bin
    
    double  rbcBirthRate_; // how many million RBCs take birth each minute
    double glycationProbSlope_; // g*l_i + c is the probability that an unglycated RBC glycates during a minute
    double glycationProbConst_;
    double glycolysisMin_;
    double glycolysisMax_;
    double glycolysisToLactate_; // what fraction of glycolysed glucose becomes lactate?
    
    double currentHbA1c();
    void updateRBCs();
    HumanBody* body;

    double minGlucoseLevel_;
    double baseGlucoseLevel_;
    double highGlucoseLevel_;
    double highLactateLevel_;
    double peakInsulinLevel_;
    
    double fluidVolume_; // in deciliters
    
    double avgBGL;
    double avgBGLOneDay;
    double avgBGLOneDaySum;
    double avgBGLOneDayCount;
    
public:
    //All the metabolites are in units of milligrams of glucose
    double glucose; // in milligrams
    double insulinLevel;
    double baseInsulinLevel_;
    double lactate;
    double branchedAminoAcids;
// see Table 2.2 in Frayn
    double glutamine;
    double alanine;
    double unbranchedAminoAcids;
    double gngSubstrates; // glycerol and other gng substrates (not including lactate, glutamine and alanine), all in units of glucose
    double glycolysisPerTick; // keep track of the glucose consumed via glycolysis this tick    
    double totalGlycolysisSoFar;

    Blood(HumanBody* myBody);
    
    //Red Blood cells use glucose during glycolysis and produce lactate
    void processTick();
    void setParams();
    void removeGlucose(double howmuch);
    void addGlucose(double howmuch);
    double getBGL(){return glucose/fluidVolume_;}
    double getGNGSubstrates(){ return (gngSubstrates + lactate + alanine + glutamine);}
    double gngFromHighLactate(double rate_);
    double baseBGL() {return baseGlucoseLevel_;}
    double highBGL() {return highGlucoseLevel_;}
    double volume() {return fluidVolume_;}
    void updateInsulinLevel();
};

#endif /* defined(__HumanBody__Blood__) */

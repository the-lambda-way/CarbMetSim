#include "Intestine.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <random>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "HumanBody.h"
#include "PortalVein.h"
#include "common.h"

using namespace std;


Intestine::Intestine(HumanBody* body)
    : body{body}
{
}

void Intestine::addChyme(double rag, double sag, double proteinInChyme, double fat)
{
    chyme.push_back({rag, sag, rag, sag, body->ticks()});

    protein += proteinInChyme;

    // very simple processing of fat for now
    body->adiposeTissue.addFat(fat);
}

double Intestine::consumeGlucose(double& glucose, double origGlucose, double mean, double stddev, double deltaTicks)
{
    double consumed;

    // we assume that glucose appears in blood as per a normal distributon with a user specified mean/stddev
    if (deltaTicks == 0)
        consumed = origGlucose * cdf(deltaTicks, mean, stddev);
    else
        consumed = origGlucose * (cdf(deltaTicks, mean, stddev) - cdf(deltaTicks - 1, mean, stddev));

    consumed = min(glucose, consumed);
    // if (glucose < 0.01 * origGlucose)    consumed = glucose;

    glucose        -= consumed;
    glucoseInLumen += consumed;

    return consumed;
}

void Intestine::processTick()
{
    digestChyme();
    absorbGlucose();
    absorbAminoAcids();
}

void Intestine::setParams(const IntestineParams& params)
{
    aminoAcidsAbsorptionRate   = params.aminoAcidsAbsorptionRate;
    glutamineOxidationRate     = params.glutamineOxidationRate;
    glutamineToAlanineFraction = params.glutamineToAlanineFraction;
    Glut2VMAX_In               = params.Glut2VMAX_In;
    Glut2Km_In                 = params.Glut2Km_In;
    Glut2VMAX_Out              = params.Glut2VMAX_Out;
    Glut2Km_Out                = params.Glut2Km_Out;
    sglt1Rate                  = params.sglt1Rate;

    if (params.fluidVolumeInLumen <= 0)    throw out_of_range("fluidVolumeInLumen not above zero");
    fluidVolumeInLumen         = params.fluidVolumeInLumen;

    fluidVolumeInEnterocytes   = params.fluidVolumeInEnterocytes;
    glycolysisMin              = params.glycolysisMin;
    glycolysisMax              = params.glycolysisMax;
    RAG_Mean                   = params.RAG_Mean;
    RAG_StdDev                 = params.RAG_StdDev;
    SAG_Mean                   = params.SAG_Mean;
    SAG_StdDev                 = params.SAG_StdDev;
}

void Intestine::glucoseFromLumen()
{
    static std::poisson_distribution<int> basalAbsorption__{1000.0 * sglt1Rate};
    static std::poisson_distribution<int> Glut2VMAX_In__{1000.0 * Glut2VMAX_In};

    // Active transport first
    activeAbsorption = static_cast<double>(basalAbsorption__(body->generator())) / 1000.0;
    activeAbsorption = min(activeAbsorption, glucoseInLumen);

    glucoseInLumen       -= activeAbsorption;
    glucoseInEnterocytes += activeAbsorption;

    if (glucoseInLumen > 0)
    {
        // passive transport via GLUT2s now
        glLumen       = glucoseInLumen / fluidVolumeInLumen;
        glEnterocytes = glucoseInEnterocytes / fluidVolumeInEnterocytes;
        double excess = glLumen - glEnterocytes;

        if (excess > 0)
        {
            // glucose concentration in lumen decides the number of GLUT2s available for transport.
            // so, Vmax depends on glucose concentration in lumen
            double x = static_cast<double>(Glut2VMAX_In__(body->generator())) / 1000.0;
            double effectiveVmax = x * glLumen / peakGlucoseConcentrationInLumen;
            effectiveVmax        = min(effectiveVmax, Glut2VMAX_In);

            passiveAbsorption = mmk(effectiveVmax, excess, Glut2Km_In);
            passiveAbsorption = min(passiveAbsorption, glucoseInLumen);

            glucoseInLumen       -= passiveAbsorption;
            glucoseInEnterocytes += passiveAbsorption;
        }
    }
}

void Intestine::glucoseToPortalVein()
{
    static std::poisson_distribution<int> Glut2VMAX_Out__{1000.0 * Glut2VMAX_Out};

    glEnterocytes = glucoseInEnterocytes / fluidVolumeInEnterocytes;
    glPortalVein  = body->portalVein.getConcentration();

    toPortalVeinPerTick = 0;

    double excess = glEnterocytes - glPortalVein;

    if (excess > 0)
    {
        double x = static_cast<double>(Glut2VMAX_Out__(body->generator())) / 1000.0;
        toPortalVeinPerTick = mmk(x, excess, Glut2Km_Out);
        toPortalVeinPerTick = min(toPortalVeinPerTick, glucoseInEnterocytes);

        glucoseInEnterocytes -= toPortalVeinPerTick;
        body->portalVein.addGlucose(toPortalVeinPerTick);
    }
}

void Intestine::glycolysisToLactate()
{
    static std::poisson_distribution<int> glycolysisMin__{1000.0 * glycolysisMin};

    double x = static_cast<double>(glycolysisMin__(body->generator())) / 1000.0;
    glycolysisPerTick = body->glycolysis(x, glycolysisMax);

    double fromEnterocytes = min(glucoseInEnterocytes, glycolysisPerTick);
    glucoseInEnterocytes -= fromEnterocytes;

    double needed = glycolysisPerTick - fromEnterocytes;
    body->blood.removeGlucose(needed);

    body->blood.lactate += glycolysisPerTick;

    // Diagnostics
    glucoseFromBlood.amount      = needed;
    glucoseFromBlood.bloodBGL    = body->blood.getBGL();
    glucoseFromBlood.bloodMinBGL = body->blood.minGlucoseLevel;
}

void Intestine::absorbGlucose()
{
    // some of the glucose is absorbed by the enterocytes (some of which moves to the portal vein)

    activeAbsorption  = 0;
    passiveAbsorption = 0;

    // first, absorb some glucose from intestinal lumen
    if (glucoseInLumen > 0)    glucoseFromLumen();

    // release some glucose to portal vein via Glut2s
    glucoseToPortalVein();

    // Modeling the glucose consumption by enterocytes: glycolysis to lactate.
    // Glycolysis depends on insulin level. Consumed glucose becomes lactate (Ref: Gerich).
    glycolysisToLactate();

    // log all the concentrations (in mmol/l)
    // peak concentrations should be 200mmol/l (lumen), 100mmol/l(enterocytes), 10mmol/l(portal vein)

    glLumen                   = (10.0 / 180.1559) * glucoseInLumen / fluidVolumeInLumen; // in mmol/l
    glEnterocytes             = (10.0 / 180.1559) * glucoseInEnterocytes / fluidVolumeInEnterocytes;
    glPortalVeinConcentration = body->portalVein.getConcentration();
    glPortalVein              = (10.0 / 180.1559) * glPortalVeinConcentration;
}

// The BCAAs, leucine, isoleucine, and valine, represent 3 of the 20 amino acids that are used in the formation of
// proteins. Thus, on average, the BCAA content of food proteins is about 15% of the total amino acid content.
// "Interrelationship between Physical Activity and Branched-Chain Amino Acids"

// The average content of glutamine in protein is about %3.9. "The Chemistry of Food" By Jan Velisek
// Do we consider the dietary glutamine? I did not consider in my code but I can add if we need it.

// Looks like cooking destroys dietary glutamine. So, no need to consider diet as source of glutamine.
// -Mukul
void Intestine::absorbAminoAcids()
{
    static std::poisson_distribution<int> aminoAcidsAbsorptionRate__{1000.0 * aminoAcidsAbsorptionRate};
    static std::poisson_distribution<int> glutamineOxidationRate__{1000.0 * glutamineOxidationRate};

    double absorbedAA = static_cast<double>(aminoAcidsAbsorptionRate__(body->generator())) / 1000.0;
    absorbedAA = min(absorbedAA, protein);

    body->portalVein.addAminoAcids(absorbedAA);
    protein -= absorbedAA;

    // Glutamine is oxidized
    double g = static_cast<double>(glutamineOxidationRate__(body->generator())) / 1000.0;
    g = min(g, body->blood.glutamine);

    body->blood.alanine   += glutamineToAlanineFraction * g;
    body->blood.glutamine -= g;
}

void Intestine::digestChyme()
{
    totalRAGConsumed = 0;
    totalSAGConsumed = 0;
    bool allDigested = true;

    chymeConsumed.clear();

    for (auto& chm : chyme)
    {
        double t = body->ticks() - chm.ts;

        double RAGConsumed = consumeGlucose(chm.RAG, chm.origRAG, RAG_Mean, RAG_StdDev, t);
        totalRAGConsumed += RAGConsumed;

        double SAGConsumed = consumeGlucose(chm.SAG, chm.origSAG, SAG_Mean, SAG_StdDev, t);
        totalSAGConsumed += SAGConsumed;

        if (chm.RAG > 0 || chm.SAG > 0)    allDigested = false;

        chymeConsumed.push_back({chm.RAG, chm.SAG, chm.origRAG, chm.origSAG, RAGConsumed, SAGConsumed});
    }

    if (allDigested)    chyme.clear();

    totalRAGDigested += totalRAGConsumed;
    totalSAGDigested += totalSAGConsumed;
}

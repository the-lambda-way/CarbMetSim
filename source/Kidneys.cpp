#include "Kidneys.h"
#include <random>
#include "Blood.h"
#include "Liver.h"

using namespace std;


Kidneys::Kidneys(HumanBody* myBody)
    : body{myBody}
{
}

void Kidneys::processTick()
{
    glycolysis();
    gluconeogenesis();
    glucoseExcretionInUrine();
}

void Kidneys::glycolysis()
{
    static std::poisson_distribution<int> glycolysisMin__{1000.0 * glycolysisMin};

    // Glycolysis depends on insulin level. Some of the consumed glucose becomes lactate.

    // Gerich says:
    // The metabolic fate of glucose is different in different regions of the kidney. Because of its low oxygen tension,
    // and low levels of oxidative enzymes, the renal medulla is an obligate user of glucose for its energy requirement
    // and does so anaerobically. Consequently, lactate is the main metabolic end product of glucose taken up in the
    // renal medulla, not carbon dioxide (CO2) and water. In contrast, the renal cortex has little glucose
    // phosphorylating capacity but a high level of oxidative enzymes. Consequently, this part of the kidney does not
    // take up and use very much glucose, with oxidation of FFAs acting as the main source of energy. A major
    // energy-requiring process in the kidney is the reabsorption of glucose from glomerular filtrate in the proximal
    // convoluted tubule.

    double x = static_cast<double>(glycolysisMin__(body->generator())) / 1000.0;
    toGlycolysis = body->glycolysis(x, glycolysisMax);

    body->blood->removeGlucose(toGlycolysis);
    body->blood->lactate += toGlycolysis;

    absorptionPerTick = toGlycolysis;
    glycolysisPerTick = toGlycolysis;

    postGlycolysis.amount      = toGlycolysis;
    postGlycolysis.bloodBGL    = body->blood->getBGL();
    postGlycolysis.bloodMinBGL = body->blood->minGlucoseLevel;
}

void Kidneys::gluconeogenesis()
{
    static std::poisson_distribution<int> rand__{100};

    // double x = static_cast<double>(rand__(body->generator())) / 100.0;
    double x = 0.9 + (static_cast<double>(rand__(body->generator())) / 1000.0);
    double scale = gngKidneys * body->insulinImpactOnGNG();
    gngPerTick = scale * x * body->bodyWeight;

    body->blood->addGlucose(gngPerTick);
    releasePerTick = gngPerTick;

    postGluconeogenesis.amount      = gngPerTick;
    postGluconeogenesis.bloodBGL    = body->blood->getBGL();
    postGluconeogenesis.bloodMinBGL = body->blood->minGlucoseLevel;
}

void Kidneys::glucoseExcretionInUrine()
{
    static std::poisson_distribution<int> glucoseExcretionRate__{1000.0 * glucoseExcretionRate};
    excretionPerTick = 0;

    double bgl = body->blood->getBGL();

    if (bgl > reabsorptionThreshold)
    {
        double x = static_cast<double>(glucoseExcretionRate__(body->generator())) / 1000.0;
        excretionPerTick = body->excretionKidneysImpact * x * (bgl - reabsorptionThreshold);

        body->blood->removeGlucose(excretionPerTick);
    }

    totalExcretion += excretionPerTick;

    postGlucoseExtraction.amount      = excretionPerTick;
    postGlucoseExtraction.bloodBGL    = body->blood->getBGL();
    postGlucoseExtraction.bloodMinBGL = body->blood->minGlucoseLevel;
}

void Kidneys::setParams(const KidneysParams& params)
{
    glycolysisMin = params.glycolysisMin;
    glycolysisMax = params.glycolysisMax;
    gngKidneys    = params.gngKidneys;
}

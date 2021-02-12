#include "Liver.h"
#include <algorithm>
#include <cassert>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "HumanBody.h"
#include "Intestine.h"
#include "Muscles.h"
#include "PortalVein.h"

using namespace std;


Liver::Liver(HumanBody* body)
    : body{body}
{
}

void Liver::processTick()
{
    static std::poisson_distribution<int> Glut2VMAX_{1000.0 * Glut2VMAX};
    glucoseAbsorption(Glut2VMAX_);

    // release all portalVein glucose to blood
    body->portalVein.releaseAllGlucose();

    // glycogen synthesis (depends on insulin level and insulin resistance)
    glycogenSynthesis();

    // Glycolysis. Depends on insulin level. Some of the consumed glucose becomes lactate.
    static std::poisson_distribution<int> rand{100};
    glycolysis(rand);

    // glycogen breakdown
    glycogenBreakdown(rand);

    // try to maintain glucose homeostasis.
    // glucoseHomeostasis();

    aminoAcidConsumption(Glut2VMAX_);
}

void Liver::glucoseAbsorption(std::poisson_distribution<int>& Glut2VMAX_)
{
    absorptionPerTick       = 0;
    excessGlucoseAbsorption = 0;

    double diff = body->portalVein.getConcentration() - glucose / fluidVolume;

    if (diff > 0)
    {
        double x = static_cast<double>(Glut2VMAX_(body->generator()));
        double scale = diff / (diff + Glut2Km);
        absorptionPerTick = scale * x * body->bodyWeight / 1000.0;

        if (absorptionPerTick > body->portalVein.getGlucose())
        {
            excessGlucoseAbsorption = absorptionPerTick;
            absorptionPerTick = body->portalVein.getGlucose();
        }

        body->portalVein.removeGlucose(absorptionPerTick);
        glucose += absorptionPerTick;
    }
}

void Liver::glycogenSynthesis()
{
    // depends on insulin level and insulin resistance

    static std::poisson_distribution<int> glucoseToGlycogen__{1000.0 * glucoseToGlycogenInLiver};

    double x = static_cast<double>(glucoseToGlycogen__(body->generator()));
    double scale = body->liverGlycogenSynthesisImpact * body->insulinImpactOnGlycogenSynthesisInLiver();
    toGlycogenPerTick = scale * x * body->bodyWeight / 1000.0;
    toGlycogenPerTick = min({toGlycogenPerTick, glucose, (glycogenMax + maxLipogenesis - glycogen)});

    glycogen += toGlycogenPerTick;

    lipogenesisOccurred = glycogen > glycogenMax;

    if (lipogenesisOccurred)
    {
        double toLipogenesis = glycogen - glycogenMax;

        // preLiposgenesis = LipogenesisState{body->bodyWeight, toLipogenesis, body->adiposeTissue.fat};
        body->adiposeTissue.lipogenesis(toLipogenesis);
        // postLiposgenesis = LipogenesisState{body->bodyWeight, toLipogenesis, body->adiposeTissue.fat};

        glycogen = glycogenMax;
    }

    glucose -= toGlycogenPerTick;

    postGlycogen.glycogen = glycogen;
    postGlycogen.glucose = glucose;
}

void Liver::glycolysis(std::poisson_distribution<int>& rand)
{
    // Depends on insulin level. Some of the consumed glucose becomes lactate.

    // Gerich paper: Liver consumes 1.65 micomol per kg per minute to 16.5 micomol per kg per minute of glucose
    // depending upon post-absorptive/post-prandial state.

    static std::poisson_distribution<int> glycolysisMin__{1000.0 * glycolysisMin};

    double x = static_cast<double>(glycolysisMin__(body->generator())) / 1000.0;
    glycolysisPerTick = min(glucose, body->glycolysis(x, glycolysisMax));

    glucose              -= glycolysisPerTick;
    body->blood.lactate += glycolysisPerTick * glycolysisToLactateFraction;

    // gluconeogenesis.
    x = 0.9 + static_cast<double>(rand(body->generator())) / 1000.0;
    // x = static_cast<double>(rand(body->generator())) / 100.0;
    double gng = gngLiver * x * body->insulinImpactOnGNG() * body->bodyWeight;

    glucose   += gng;
    gngPerTick = gng;

    postGluconeogenesis.glucose      = glucose;
    postGluconeogenesis.glycogen     = glycogen;
    postGluconeogenesis.bloodGlucose = body->blood.glucose;
    postGluconeogenesis.bloodLactate = body->blood.lactate;
}

void Liver::glycogenBreakdown(std::poisson_distribution<int>& rand)
{
    // double x = static_cast<double>(rand(body->generator())) / 100.0;
    double x = 0.9 + static_cast<double>(rand(body->generator())) / 1000.0;

    fromGlycogenPerTick = glycogenToGlucoseInLiver * x * body->insulinImpactOnGlycogenBreakdownInLiver();
    fromGlycogenPerTick *= body->bodyWeight;
    fromGlycogenPerTick = min(fromGlycogenPerTick, glycogen);

    glucose  += fromGlycogenPerTick;
    glycogen -= fromGlycogenPerTick;
}

// void Liver::glucoseHomeostasis()
// {
//     // try to maintain glucose homeostasis.
//     glucoseNeeded  = body->muscles.glucoseAbsorbedPerTick + body->getGlucoseNeedsOutsideMuscles();
//     glucoseNeeded += body->muscles.glucoseAbsorbedPerTick;
//     glucoseNeeded -= body->intestine.toPortalVeinPerTick;
//     glucoseNeeded -= gngPerTick;
//     glucoseNeeded -= body->kidneys.gngPerTick;
//     glucoseNeeded -= fromGlycogenPerTick;
//     glucoseNeeded  = max(glucoseNeeded, 0);

//     fromGlycogenPerTick = 0.0;

//     if (body->isExercising())
//     {
//         double max_ = maxGlycogenToGlucoseDuringExercise * body->bodyWeight
//         max_       *= body->maxLiverGlycogenBreakdownDuringExerciseImpact;

//         fromGlycogenPerTick = glucoseNeeded;

//         if (glucoseNeeded > max_ || glucoseNeeded > glycogen)
//             fromGlycogenPerTick = min(max_, glycogen);
//     }

//     glycogen -= fromGlycogenPerTick;
//     glucose  += fromGlycogenPerTick;

//     // if no liver glycogen left, invoke GNG again to produce glucose to maintain glucose homeostasis
//     maxGNGDuringExercise = 0;

//     if (glycogen == 0)
//     {
//         glucoseNeeded -= fromGlycogenPerTick;

//         if (body->isExercising())
//         {
//             maxGNGDuringExercise = static_cast<double>(maxGNGDuringExercise__(body->generator()));
//             // maxGNGDuringExercise *= body->percentVO2Max * body->bodyWeight / 1000.0;
//             maxGNGDuringExercise *= body->bodyWeight / 1000.0;

//             glucoseNeeded = min(glucoseNeeded, maxGNGDuringExercise - gngPerTick);
//         }

//         glucose    += glucoseNeeded;
//         gngPerTick += glucoseNeeded;
//     }
// }

void Liver::aminoAcidConsumption(std::poisson_distribution<int>& Glut2VMAX_)
{
    // 93% of unbranched amino acids in portal vein are retained in Liver, because the leaked amino acids from Intestine
    // consists of 15% branched and 85% unbranched, but after liver consumption the percentage needs to be 70% branched,
    // 30% unbranched. To provide these percentages 93% of unbranched amino acids in portal vein are retained in liver.
    // (From Frayn's book)

    releasePerTick = 0;

    body->portalVein.releaseAminoAcids();

    double diff = glucose / fluidVolume - body->blood.getBGL();

    if (diff > 0)
    {
        double x = static_cast<double>(Glut2VMAX_(body->generator()));
        double scale = diff / (diff + Glut2Km);
        releasePerTick = scale * x * body->bodyWeight / 1000.0;

        assert(((void)"Releasing more glucose to blood than what is present in liver!", releasePerTick <= glucose));

        glucose -= releasePerTick;
        body->blood.addGlucose(releasePerTick);
    }
}

void Liver::setParams(const LiverParams& params)
{
    glycogen                    = 1000.0 * params.glycogen;
    glycogenMax                 = 1000.0 * params.glycogenMax;
    fluidVolume                 = params.fluidVolume;
    Glut2Km                     = params.Glut2Km;
    Glut2VMAX                   = params.Glut2VMAX;
    glucoseToGlycogenInLiver    = params.glucoseToGlycogenInLiver;
    glycogenToGlucoseInLiver    = params.glycogenToGlucoseInLiver;
    glycolysisMin               = params.glycolysisMin;
    glycolysisMax               = params.glycolysisMax;
    glycolysisToLactateFraction = params.glycolysisToLactateFraction;
    gngLiver                    = params.gngLiver;
    glucoseToNEFA               = params.glucoseToNEFA;
    maxLipogenesis              = params.maxLipogenesis;
}

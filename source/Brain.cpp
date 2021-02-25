#include "Brain.h"
#include <random>
#include "Blood.h"
#include "HumanBody.h"

using namespace std;


Brain::Brain(HumanBody* body)
    : body{body}
{
}

// Release lactate to blood
// void Brain::glucoseToLactate(int rate)
// {
//     glucoseRateToLactate = rate;

//     body->blood.glucose -= rate;
//     body->blood.lactate += rate;
// }

void Brain::processTick()
{
    oxidizeGlucose();
    glutamineFromBlood();
}

void Brain::oxidizeGlucose()
{
    static std::poisson_distribution<int> glucoseOxidized__{1000.0 * glucoseOxidized};

    oxidationPerTick = static_cast<double>(glucoseOxidized__(body->generator())) / 1000.0;

    glucoseRemoved.amount      = oxidationPerTick + glucoseToAlanine;
    glucoseRemoved.bloodBGL    = body->blood.getBGL();
    glucoseRemoved.bloodMinBGL = body->blood.minGlucoseLevel;

    body->blood.removeGlucose(glucoseRemoved.amount);
    body->blood.alanine += glucoseToAlanine;
}

void Brain::glutamineFromBlood()
{
    // Brain generates glutamine from branched amino acids.
    double x = min(body->blood.branchedAminoAcids, bAAToGlutamine);
    body->blood.branchedAminoAcids -= x;
    body->blood.glutamine          += x;
}

void Brain::setParams(const BrainParams& params)
{
    glucoseOxidized  = params.glucoseOxidized;
    glucoseToAlanine = params.glucoseToAlanine;
    bAAToGlutamine   = params.bAAToGlutamine;
}

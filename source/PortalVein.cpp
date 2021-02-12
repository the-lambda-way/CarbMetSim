#include "PortalVein.h"
#include <cassert>
#include "Blood.h"
#include "HumanBody.h"

using namespace std;


PortalVein::PortalVein(HumanBody* body)
    : body{body}
{
}

void PortalVein::processTick()
{
    double bgl = body->blood.getBGL();
    double glucoseFromBlood = bgl * fluidVolume;

    body->blood.removeGlucose(glucoseFromBlood);
    glucose += glucoseFromBlood;

    fromBlood.amount      = glucoseFromBlood;
    fromBlood.bloodBGL    = bgl;
    fromBlood.bloodMinBGL = body->blood.minGlucoseLevel;
}

void PortalVein::releaseAllGlucose()
{
    body->blood.addGlucose(glucose);
    glucose = 0;
}

void PortalVein::removeGlucose(double g)
{
    glucose -= g;
    assert(((void)"PortalVein glucose went negative\n", glucose >= 0));
}

double PortalVein::getConcentration() const
{
    return glucose / fluidVolume;
}

void PortalVein::addGlucose(double g)
{
    glucose += g;
}

double PortalVein::getGlucose() const
{
    return glucose;
}

void PortalVein::setParams(const PortalVeinParams& params)
{
    fluidVolume = params.fluidVolume;
}

void PortalVein::addAminoAcids(double aa)
{
    branchedAA   += 0.15 * aa;
    unbranchedAA += 0.85 * aa;
}

void PortalVein::releaseAminoAcids()
{
    // 93% unbranched amino acids consumed by liver to make alanine
    body->blood.alanine              += 0.93 * unbranchedAA;
    body->blood.unbranchedAminoAcids += 0.07 * unbranchedAA;
    unbranchedAA = 0;

    body->blood.branchedAminoAcids += branchedAA;
    branchedAA = 0;

    // who consumes these amino acids from blood other than liver?
    // brain consumes branched amino acids
}


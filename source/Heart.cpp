#include "Heart.h"
#include <random>
#include "Blood.h"
#include "HumanBody.h"

using namespace std;


Heart::Heart(HumanBody* body)
    : body{body}
{
}

void Heart::processTick()
{
    static std::poisson_distribution<int> basalGlucoseAbsorbed__{1000.0 * basalGlucoseAbsorbed};

    // double amount = basalGlucoseAbsorbed * body->bodyWeight;
    double amount = static_cast<double>(basalGlucoseAbsorbed__(body->generator())) / 1000.0;
    body->blood.removeGlucose(amount);

    oxidationPerTick = amount;

    basalAbsorption.amount      = amount;
    basalAbsorption.bloodBGL    = body->blood.getBGL();
    basalAbsorption.bloodMinBGL = body->blood.minGlucoseLevel;

   /**********
    // Absorption via GLUT4
    double bgl = body->blood.getBGL();
    double scale = (1.0 - body->insulinResistance) * body->blood.insulinLevel * body->bodyWeight;
    Glut4Absorption = mmk(scale * glut4VMAX, bgl, glut4Km);

    body->blood.removeGlucose(Glut4Absorption);
    oxidationPerTick += Glut4Absorption;
   ***************/

    // static std::poisson_distribution<int> lactateOxidized__{1000.0 * lactateOxidized};
    // double lactateOxidized_ = static_cast<double>(lactateOxidized__(body->generator())) / 1000.0;
    // body->blood.lactate -= min(body->blood.lactate, lactateOxidized_)
}

void Heart::setParams(const HeartParams& params)
{
    lactateOxidized      = params.lactateOxidized;
    basalGlucoseAbsorbed = params.basalGlucoseAbsorbed;
    Glut4Km              = params.Glut4Km;
    Glut4VMAX            = params.Glut4VMAX;
}

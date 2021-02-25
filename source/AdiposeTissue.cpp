#include "AdiposeTissue.h"
#include "HumanBody.h"

using namespace std;


void AdiposeTissue::processTick()
{
}

void AdiposeTissue::setParams()
{
}

AdiposeTissue::AdiposeTissue(HumanBody* body)
    : body{body}
{
    // fat = body->fatFraction * body->bodyWeight * 1000.0;
    // will be set by HumanBody once it gets the bodyWeight
}

void AdiposeTissue::lipogenesis(double glucoseInMG)
{
    // one gram of glucose has 4 kcal of energy
    // one gram of TAG has 9 kcal of energy
    addFat(glucoseInMG * 4.0 / 9.0);
}

void AdiposeTissue::consumeFat(double kcal)
{
    addFat(-1000.0 * kcal / 9.0);
}

void AdiposeTissue::addFat(double newFatInMG)
{
    double newFat = newFatInMG / 1000.0;
    fat += newFat;
    body->bodyWeight += newFat / 1000.0;
}

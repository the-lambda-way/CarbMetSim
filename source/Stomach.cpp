#include "Stomach.h"
#include <random>
#include "AdiposeTissue.h"
#include "Blood.h"
#include "HumanBody.h"
#include "Intestine.h"

using namespace std;


Stomach::Stomach(HumanBody* body)
    : body{body}
{
}

// howmuch is in grams
void Stomach::addFood(const FoodType& food, double howmuch)
{
    double numServings = howmuch / food.servingSize;

    RAG     += 1000.0 * numServings * food.RAG;     // in milligrams
    SAG     += 1000.0 * numServings * food.SAG;     // in milligrams
    protein += 1000.0 * numServings * food.protein; // in milligrams
    fat     += 1000.0 * numServings * food.fat;     // in milligrams

    if (RAG > 0 || SAG > 0 || protein > 0 || fat > 0)    stomachEmpty = false;
}

void Stomach::setParams(const StomachParams& params)
{
    geConstant = params.geConstant;
    geSlopeMin = params.geSlopeMin;
}

void Stomach::processTick()
{
	// send some chyme to the intestine.

	// We assume that the nutrient composition of the chyme is same as that of the food in the stomach.

	// The amount of chyme leaking out of stomach during a minute is equal to a certain minimum
	// plus a part proportional to the total amount of chyme present in the stomach at that time.
	// The proportionality constant depends on the energy density of the chyme. If the chyme is all
	// fat, its energy density would be 9kcal/g. If it is all carbs, the energy density is 4kcal/g.
	// The proportionality constant should decrease with increase in the energy density of the chyme.

	// When we consider the fraction of leaked chyme that is dependent on the total amount of food in the
	// stomach, the energy content should be same when the stomach has 9grams of carbs or 4 grams of fat.
	// Energy leaked when the stomach has x grams of fat: k9*x*1000*9 cal
	// Energy leaked when the stomach has x grams of carbs: k4*x*1000*4 cal

	// Hence

	// k9*x*1000*9 = k4*x*1000*4
	// =>k9*9 = k4*4
	// => k4 = 9*k9/4
	// Similarly
	// k5 = 9*k9/5

	// So, we just need to know the proportinality constant k9 (geSlopeMin) for pure fat.

	// Chyme leakage does not change the relative fraction of carbs/fats/proteins in the chyme left in the stomach.

	totalFood = RAG + SAG + protein + fat;

	if (stomachEmpty)    return;

    static std::poisson_distribution<int> geConstant__{1000.0 * geConstant};
    double x = static_cast<double>(geConstant__(body->generator())) / 1000.0;

	calorificDensity = (4.0 * (RAG + SAG + protein) + 9.0 * fat) / totalFood;
	geSlope = 9.0 * geSlopeMin / calorificDensity;

    double geBolus = min(x + geSlope * totalFood, totalFood);

	ragInBolus     = geBolus * RAG / totalFood;
	sagInBolus     = geBolus * SAG / totalFood;
	proteinInBolus = geBolus * protein / totalFood;

	fatInBolus.amount     = geBolus * fat / totalFood;
    fatInBolus.bodyWeight = body->bodyWeight;

	RAG     -= ragInBolus;
	SAG     -= sagInBolus;
	protein -= proteinInBolus;
	fat     -= fatInBolus.amount;

    body->intestine->addChyme(ragInBolus, sagInBolus, proteinInBolus, fatInBolus.amount);

	if (RAG <= 0.001 && SAG <= 0.001 && protein <= 0.001 && fat <= 0.001)
    {
        stomachEmpty = true;
        body->stomachEmpty();
    }
}

#pragma once

#include <random>
#include "common.h"

class HumanBody;
class FoodType;


struct StomachParams
{
    double geConstant = 500.0; // mg
    double geSlopeMin = 0.03; // Min value for GE slope (applicable to fat)
};


class Stomach
{
public:
    Stomach(HumanBody* body);

    void addFood(const FoodType& food, double howmuch);
    void setParams(const StomachParams& params);
    void processTick();

    double RAG     = 0; // mg
    double SAG     = 0; // mg
    double protein = 0; // mg
    double fat     = 0; // mg

    // Diagnostics
    double totalFood        = 0;
    double calorificDensity = 0;
    double geSlope          = 0;
    double ragInBolus       = 0;
    double sagInBolus       = 0;
    double proteinInBolus   = 0;
    FatState fatInBolus;
    bool stomachEmpty       = true;
    bool stomachBecameEmpty = false;

private:
    HumanBody* body;

    double geConstant; // mg
    double geSlopeMin; // Min value for GE slope (applicable to fat)
   	// the amount of gastric emptying (mg per minute) = geConstant + geSlope * (total food in stomach in milligrams)
};

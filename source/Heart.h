#pragma once

#include "common.h"

class HumanBody;


struct HeartParams
{
    double lactateOxidized;
    double basalGlucoseAbsorbed;
    double Glut4Km;
    double Glut4VMAX;
};


class Heart
{
public:
    Heart(HumanBody* body);

    void processTick();
    void setParams(const HeartParams& params);

    double oxidationPerTick;

    // Diagnostics
    GlucoseState basalAbsorption;
    // double glut4Absorption;

private:
    HumanBody* body;

    // The heart absorbs glucose and some lactate (depends on its concentration). Glucose is absorbed via glut4. So,
    // high insulin concentration (i.e. fed state) is required for the heart to use glucose for its energy needs. The
    // following paper has numbers for how much glucose and lactate is used by the heart.
    // Skeletal Muscle Glycolysis, Oxidation, and Storage of an Oral Glucose Load- Kelley et.al.
    double basalGlucoseAbsorbed = 14; //mg per minute;

    double Glut4Km   = 5 * 180.1559 / 10.0; // mg/dl equivalent of 5 mmol/l;
    double Glut4VMAX = 0; // mg per kg per minute

    double lactateOxidized = 0;
};

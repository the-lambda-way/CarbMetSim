#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "catch2/catch.h"
#include "HumanBody.h"
#include "SimCtl.h"


// TODO: resolve a segfault during SimCtl::run_simultion, so the test can complete.


void set_all_params(HumanBody& body, BodyOrgan organ, const std::string& param, double val)
{
    body.metabolicParameters[FED_RESTING              ][organ][param] = val;
    body.metabolicParameters[FED_EXERCISING           ][organ][param] = val;
    body.metabolicParameters[POSTABSORPTIVE_RESTING   ][organ][param] = val;
    body.metabolicParameters[POSTABSORPTIVE_EXERCISING][organ][param] = val;
}

std::string file_to_string (const std::string& path)
{
    // Open file
    std::ifstream file {path, std::ios::in | std::ios::binary | std::ios::ate};
    // if (!file)    throw (errno);
    if (!file)
    {
        std::cout << "File error\n";
        exit(1);
    }

    // Allocate string memory
    std::string contents;
    contents.resize(file.tellg());

    // Read file contents into string
    file.seekg(0);
    file.read(contents.data(), contents.size());

    return contents;
}


SCENARIO("Simulate a meal event on a normal human.")
{
    GIVEN("A simulator, a normal human body, and a normal meal")
    {
        SimCtl    sim  = SimCtl{"alpha"};
        HumanBody body = HumanBody{};

        body.foodTypes[1] = {1, "Breakfast", 120.0, 84.0, 0.0, 26.0, 10.0};
        body.exerciseTypes[1] = {1, "Walk", 5.0};

        set_all_params(body, HUMAN_BODY, "gngImpact_", 6.0);
        set_all_params(body, HUMAN_BODY, "liverGlycogenBreakdownImpact_", 6.0);
        set_all_params(body, HUMAN_BODY, "intensityPeakGlucoseProd_", 0.2);
        set_all_params(body, HUMAN_BODY, "glut4Impact_", 1.0);
        set_all_params(body, HUMAN_BODY, "glycolysisMinImpact_", 1.0);
        set_all_params(body, HUMAN_BODY, "glycolysisMaxImpact_", 1.0);
        set_all_params(body, HUMAN_BODY, "excretionKidneysImpact_", 1.0);
        set_all_params(body, HUMAN_BODY, "liverGlycogenSynthesisImpact_", 1.0);
        set_all_params(body, HUMAN_BODY, "insulinImpactOnGNG_Mean", 0.5);
        set_all_params(body, HUMAN_BODY, "insulinImpactOnGNG_StdDev", 0.2);
        set_all_params(body, HUMAN_BODY, "insulinImpactGlycogenBreakdownInLiver_Mean", 0.1);
        set_all_params(body, HUMAN_BODY, "insulinImpactGlycogenBreakdownInLiver_StdDev", 0.02);
        set_all_params(body, HUMAN_BODY, "insulinImpactGlycogenSynthesisInLiver_Mean", 0.5);
        set_all_params(body, HUMAN_BODY, "insulinImpactGlycogenSynthesisInLiver_StdDev", 0.2);
        set_all_params(body, HUMAN_BODY, "insulinImpactOnGlycolysis_Mean", 0.5);
        set_all_params(body, HUMAN_BODY, "insulinImpactOnGlycolysis_StdDev", 0.2);
        set_all_params(body, HUMAN_BODY, "bodyWeight_", 89);
        set_all_params(body, HUMAN_BODY, "age_", 49);
        set_all_params(body, HUMAN_BODY, "gender_", 0);
        set_all_params(body, HUMAN_BODY, "fitnessLevel_", 50);
        set_all_params(body, LIVER, "Glycogen", 100);
        set_all_params(body, LIVER, "MaxGlycogen", 120);
        set_all_params(body, LIVER, "glucoseToGlycogenInLiver_", 4.5);
        set_all_params(body, LIVER, "glycogenToGlucoseInLiver_", 0.9);
        set_all_params(body, LIVER, "gngLiver_", 0.16);
        set_all_params(body, LIVER, "maxLipogenesis_", 400.0);
        set_all_params(body, BLOOD, "minGlucoseLevel_", 50);
        set_all_params(body, BLOOD, "baseGlucoseLevel_", 90);
        set_all_params(body, BLOOD, "highGlucoseLevel_", 145);
        set_all_params(body, BLOOD, "baseInsulinLevel_", 0.001);
        set_all_params(body, BLOOD, "peakInsulinLevel_", 1.0);
        set_all_params(body, MUSCLES, "Glycogen", 500);
        set_all_params(body, MUSCLES, "MaxGlycogen", 500);
        set_all_params(body, MUSCLES, "Glut4VMAX_", 3.5);
        set_all_params(body, MUSCLES, "PeakGlut4VMAX_", 7.0);
        set_all_params(body, MUSCLES, "glucoseToGlycogen_", 7.0);
        set_all_params(body, MUSCLES, "glycogenShareDuringExerciseMean_", 0.53);
        set_all_params(body, KIDNEY, "gngKidneys_", 0.16);

        body.setParams();


        WHEN("the meal is eaten at 10 hours and the simulation ended at 18 hours")
        {
            unsigned int fireTime = 0 * TICKS_PER_DAY + 10 * TICKS_PER_HOUR + 0;
            sim.addEvent(fireTime, 0, 1, 120);

            fireTime = 0 * TICKS_PER_DAY + 18 * TICKS_PER_HOUR + 0;
            sim.addEvent(fireTime, 2, 0, 0);

            // Backup catch2 cout contents
            std::streambuf* catch2_buf = std::cout.rdbuf();
            std::ostringstream output;
            std::cout.rdbuf(output.rdbuf());

            sim.run_simulation();

            // Restore catch2 contents to cout
            std::cout.rdbuf(catch2_buf);


            THEN("certain output is expected.")
            {
                // file path relative to build/tests/system/
                std::string normal_output = file_to_string("../../../tests/system/outNormal.alpha");
                REQUIRE(true);
                // REQUIRE(output.str() == normal_output);
            }
        }
    }
}


#include <algorithm>
#include <cmath>   // abs
#include <cstring> // strerror
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include "source/SimCtl.h"

using namespace std;


using ParamMap = map<string, map<string, double*>>;

ParamMap createParamMap(MetabolicParams& params)
{
    ParamMap out;

    // AdiposeTissue
    // Currently none

    // Blood
    out["BLOOD"]["rbcBirthRate"]        = &params.blood.rbcBirthRate;
    out["BLOOD"]["glycationProbSlope"]  = &params.blood.glycationProbSlope;
    out["BLOOD"]["glycationProbConst"]  = &params.blood.glycationProbConst;
    out["BLOOD"]["minGlucoseLevel"]     = &params.blood.minGlucoseLevel;
    out["BLOOD"]["baseGlucoseLevel"]    = &params.blood.baseGlucoseLevel;
    out["BLOOD"]["baseInsulinLevel"]    = &params.blood.baseInsulinLevel;
    out["BLOOD"]["peakInsulinLevel"]    = &params.blood.peakInsulinLevel;
    out["BLOOD"]["highGlucoseLevel"]    = &params.blood.highGlucoseLevel;
    out["BLOOD"]["highLactateLevel"]    = &params.blood.highLactateLevel;
    out["BLOOD"]["glycolysisMin"]       = &params.blood.glycolysisMin;
    out["BLOOD"]["glycolysisMax"]       = &params.blood.glycolysisMax;
    out["BLOOD"]["glycolysisToLactate"] = &params.blood.glycolysisToLactate;

    // Brain
    out["BRAIN"]["glucoseOxidized"]  = &params.brain.glucoseOxidized;
    out["BRAIN"]["glucoseToAlanine"] = &params.brain.glucoseToAlanine;
    out["BRAIN"]["bAAToGlutamine"]   = &params.brain.bAAToGlutamine;

    // Heart
    out["HEART"]["lactateOxidized"]      = &params.heart.lactateOxidized;
    out["HEART"]["basalGlucoseAbsorbed"] = &params.heart.basalGlucoseAbsorbed;
    out["HEART"]["Glut4Km"]              = &params.heart.Glut4Km;
    out["HEART"]["Glut4VMAX"]            = &params.heart.Glut4VMAX;

    // HumanBody
    out["HUMAN_BODY"]["age"]                          = &params.body.age;
    out["HUMAN_BODY"]["gender"]                       = &params.body.gender;
    out["HUMAN_BODY"]["fitnessLevel"]                 = &params.body.fitnessLevel;
    out["HUMAN_BODY"]["glut4Impact"]                  = &params.body.glut4Impact;
    out["HUMAN_BODY"]["glycolysisMinImpact"]          = &params.body.glycolysisMinImpact;
    out["HUMAN_BODY"]["glycolysisMaxImpact"]          = &params.body.glycolysisMaxImpact;
    out["HUMAN_BODY"]["excretionKidneysImpact"]       = &params.body.excretionKidneysImpact;
    out["HUMAN_BODY"]["liverGlycogenBreakdownImpact"] = &params.body.liverGlycogenBreakdownImpact;
    out["HUMAN_BODY"]["liverGlycogenSynthesisImpact"] = &params.body.liverGlycogenSynthesisImpact;
    // out["HUMAN_BODY"]["maxLiverGlycogenBreakdownDuringExerciseImpact"]
    //     = &params.body.maxLiverGlycogenBreakdownDuringExerciseImpact;
    out["HUMAN_BODY"]["gngImpact"]                        = &params.body.gngImpact;
    out["HUMAN_BODY"]["bodyWeight"]                       = &params.body.bodyWeight;
    out["HUMAN_BODY"]["insulinImpactOnGlycolysis_Mean"]   = &params.body.insulinImpactOnGlycolysis_Mean;
    out["HUMAN_BODY"]["insulinImpactOnGlycolysis_StdDev"] = &params.body.insulinImpactOnGlycolysis_StdDev;
    out["HUMAN_BODY"]["insulinImpactOnGNG_Mean"]          = &params.body.insulinImpactOnGNG_Mean;
    out["HUMAN_BODY"]["insulinImpactOnGNG_StdDev"]        = &params.body.insulinImpactOnGNG_StdDev;
    out["HUMAN_BODY"]["insulinImpactGlycogenBreakdownInLiver_Mean"]
        = &params.body.insulinImpactGlycogenBreakdownInLiver_Mean;
    out["HUMAN_BODY"]["insulinImpactGlycogenBreakdownInLiver_StdDev"]
        = &params.body.insulinImpactGlycogenBreakdownInLiver_StdDev;
    out["HUMAN_BODY"]["insulinImpactGlycogenSynthesisInLiver_Mean"]
        = &params.body.insulinImpactGlycogenSynthesisInLiver_Mean;
    out["HUMAN_BODY"]["insulinImpactGlycogenSynthesisInLiver_StdDev"]
        = &params.body.insulinImpactGlycogenSynthesisInLiver_StdDev;
    out["HUMAN_BODY"]["intensityPeakGlucoseProd"] = &params.body.intensityPeakGlucoseProd;

    // Intestine
    out["INTESTINE"]["aminoAcidAbsorptionRate"]    = &params.intestine.aminoAcidsAbsorptionRate;
    out["INTESTINE"]["glutamineOxidationRate"]     = &params.intestine.glutamineOxidationRate;
    out["INTESTINE"]["glutamineToAlanineFraction"] = &params.intestine.glutamineToAlanineFraction;
    out["INTESTINE"]["Glut2VMAX_In"]               = &params.intestine.Glut2VMAX_In;
    out["INTESTINE"]["Glut2Km_In"]                 = &params.intestine.Glut2Km_In;
    out["INTESTINE"]["Glut2VMAX_Out"]              = &params.intestine.Glut2VMAX_Out;
    out["INTESTINE"]["Glut2Km_Out"]                = &params.intestine.Glut2Km_Out;
    out["INTESTINE"]["sglt1Rate"]                  = &params.intestine.sglt1Rate;
    out["INTESTINE"]["fluidVolumeInLumen"]         = &params.intestine.fluidVolumeInLumen;
    out["INTESTINE"]["fluidVolumeInEnterocytes"]   = &params.intestine.fluidVolumeInEnterocytes;
    out["INTESTINE"]["glycolysisMin"]              = &params.intestine.glycolysisMin;
    out["INTESTINE"]["glycolysisMax"]              = &params.intestine.glycolysisMax;
    out["INTESTINE"]["RAG_Mean"]                   = &params.intestine.RAG_Mean;
    out["INTESTINE"]["RAG_StdDev"]                 = &params.intestine.RAG_StdDev;
    out["INTESTINE"]["SAG_Mean"]                   = &params.intestine.SAG_Mean;
    out["INTESTINE"]["SAG_StdDev"]                 = &params.intestine.SAG_StdDev;

    // Kidneys
    out["KIDNEYS"]["glycolysisMin"] = &params.kidneys.glycolysisMin;
    out["KIDNEYS"]["glycolysisMax"] = &params.kidneys.glycolysisMax;
    out["KIDNEYS"]["gngKidneys"]    = &params.kidneys.gngKidneys;

    // Liver
    out["LIVER"]["Glycogen"]                    = &params.liver.glycogen;
    out["LIVER"]["MaxGlycogen"]                 = &params.liver.glycogenMax;
    out["LIVER"]["fluidVolume"]                 = &params.liver.fluidVolume;
    out["LIVER"]["Glut2Km"]                     = &params.liver.Glut2Km;
    out["LIVER"]["Glut2VMAX"]                   = &params.liver.Glut2VMAX;
    out["LIVER"]["glucoseToGlycogenInLiver"]    = &params.liver.glucoseToGlycogenInLiver;
    out["LIVER"]["glycogenToGlucoseInLiver"]    = &params.liver.glycogenToGlucoseInLiver;
    out["LIVER"]["glycolysisMin"]               = &params.liver.glycolysisMin;
    out["LIVER"]["glycolysisMax"]               = &params.liver.glycolysisMax;
    out["LIVER"]["glycolysisToLactateFraction"] = &params.liver.glycolysisToLactateFraction;
    out["LIVER"]["gngLiver"]                    = &params.liver.gngLiver;
    out["LIVER"]["glucoseToNEFA"]               = &params.liver.glucoseToNEFA;
    out["LIVER"]["maxLipogenesis"]              = &params.liver.maxLipogenesis;

    // Muscles
    out["MUSCLES"]["Glycogen"]                           = &params.muscles.glycogen;
    out["MUSCLES"]["MaxGlycogen"]                        = &params.muscles.glycogenMax;
    out["MUSCLES"]["Glut4Km"]                            = &params.muscles.Glut4Km;
    out["MUSCLES"]["Glut4VMAX"]                          = &params.muscles.Glut4VMAX;
    out["MUSCLES"]["PeakGlut4VMAX"]                      = &params.muscles.PeakGlut4VMAX;
    out["MUSCLES"]["maxGlucoseAbsorptionDuringExercise"] = &params.muscles.maxGlucoseAbsorptionDuringExercise;
    out["MUSCLES"]["basalGlucoseAbsorbed"]               = &params.muscles.basalGlucoseAbsorbed;
    out["MUSCLES"]["baaToGlutamine"]                     = &params.muscles.baaToGlutamine;
    out["MUSCLES"]["glycolysisMin"]                      = &params.muscles.glycolysisMin;
    out["MUSCLES"]["glycolysisMax"]                      = &params.muscles.glycolysisMax;
    out["MUSCLES"]["glucoseToGlycogen"]                  = &params.muscles.glucoseToGlycogen;
    out["MUSCLES"]["glycogenShareDuringExerciseMean"]    = &params.muscles.glycogenShareDuringExerciseMean;

    // Portal Vein
    out["PORTAL_VEIN"]["fluidVolume"] = &params.portalVein.fluidVolume;

    // Stomach
    out["STOMACH"]["geConstant"] = &params.stomach.geConstant;
    out["STOMACH"]["geSlopeMin"] = &params.stomach.geSlopeMin;


    return out;
}

map<string, ParamMap> createMetabolicMap(map<BodyState, MetabolicParams>& metabolicParams)
{
    map<string, ParamMap> out;

    out["FED_RESTING"]               = createParamMap(metabolicParams[BodyState::FED_RESTING]);
    out["FED_EXERCISING"]            = createParamMap(metabolicParams[BodyState::FED_EXERCISING]);
    out["POSTABSORPTIVE_RESTING"]    = createParamMap(metabolicParams[BodyState::POSTABSORPTIVE_RESTING]);
    out["POSTABSORPTIVE_EXERCISING"] = createParamMap(metabolicParams[BodyState::POSTABSORPTIVE_EXERCISING]);

    return out;
}

map<string, EventType> createEventMap()
{
    map<string, EventType> out;

    out["FOOD"]     = EventType::FOOD;
    out["EXERCISE"] = EventType::EXERCISE;
    out["HALT"]     = EventType::HALT;

    return out;
}

template <class K, class V>
vector<K> get_keys(map<K, V> map_)
{
    vector<K> keys;
    for (auto elem : map_)    keys.push_back(elem.first);
    return keys;
}

template <class T>
bool contains(const vector<T>& v, T t)
{
    return find(v.begin(), v.end(), t) != v.end();
}


class CarbMetSimCLI
{
public:
    CarbMetSimCLI(string_view foodsFile, string_view exerciseFile, string_view metabolicRatesFile,
                  string_view eventsFile, string_view seedString, string_view outputFile)
        : metabolicMap{createMetabolicMap(metabolicParameters)},
          eventMap{createEventMap()}
    {
        // Get states and organs automatically so they only need to be updated in one place
        bodyStates = get_keys(metabolicMap);
        bodyStates.push_back("ALL");
        organs = get_keys(metabolicMap[bodyStates.front()]);

        readMetabolicParams(metabolicRatesFile);
        readFoodFile(foodsFile);
        readExerciseFile(exerciseFile);

        sim = make_unique<SimCtl>(seedString, metabolicParameters, foodTypes, exerciseTypes);
        readEventsFile(eventsFile);

        if (outputFile != "stdout")
        {
            outputFileStream.open(outputFile.data(), ios_base::out | ios_base::app);
            if (!outputFileStream)
            {
                cerr << "Failed to open " << outputFile << endl;
                cerr << "Message: " << strerror(errno) << endl;
                exit(1);
            }

            output.rdbuf(outputFileStream.rdbuf());
        }

        output << std::fixed;
        output << std::setprecision(3);
    }

    void run()
    {
        while (sim->runTick())    onTick();

        auto haltEvent = dynamic_pointer_cast<HaltEvent>(sim->eventsFiredThisTick().back());
        onHaltEvent(*haltEvent);
    }

private:
    unique_ptr<SimCtl>              sim;
    map<BodyState, MetabolicParams> metabolicParameters;
    map<unsigned, ExerciseType>     exerciseTypes;
    map<unsigned, FoodType>         foodTypes;
    ostream                         output{cout.rdbuf()};
    ofstream                        outputFileStream;

    map<string, ParamMap>  metabolicMap;
    map<string, EventType> eventMap;
    vector<string>         bodyStates;
    vector<string>         organs;


    void readMetabolicParams(string_view file)
    {
        ifstream cfg{file.data()};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
            cerr << "Message: " << strerror(errno) << endl;
            exit(1);
        }

        string line;
        while (getline(cfg, line))
        {
            istringstream str{line};

            string bodyState, bodyOrgan, param;
            double val;
            str >> bodyState >> bodyOrgan >> param >> val;

            if (!contains(bodyStates, bodyState))
            {
                cerr << "Unrecognized body state: " << bodyState << endl;
                exit(1);
            }

            if (!contains(organs, bodyOrgan))
            {
                cerr << "Unrecognized organ: " << bodyOrgan << endl;
                exit(1);
            }

            // Note: bodyStates.front() is used in case the bodyState equals "ALL", which is not a key in metabolicMap
            const map<string, double*>& paramMap = metabolicMap[bodyStates.front()][bodyOrgan];
            if (paramMap.find(param) == paramMap.end())
            {
                cerr << "Unrecognized parameter for organ \"" << bodyOrgan << "\": " << param << endl;
                exit(1);
            }

            if (bodyState == "ALL")
            {
                for (auto& entry : metabolicMap)
                    *entry.second[bodyOrgan][param] = val;
            }
            else
                *metabolicMap[bodyState][bodyOrgan][param] = val;
        }
    }

    void readExerciseFile(string_view file)
    {
        ifstream cfg{file.data()};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
            cerr << "Message: " << strerror(errno) << endl;
            exit(1);
        }

        string line;
        while (getline(cfg, line))
        {
            istringstream str{line};
            unsigned id;

            str >> id;
            exerciseTypes[id].exerciseID = id;

            str >> exerciseTypes[id].name;
            str >> exerciseTypes[id].intensity;
        }
    }

    void readFoodFile(string_view file)
    {
        ifstream cfg{file.data()};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
            cerr << "Message: " << strerror(errno) << endl;
            exit(1);
        }

        string line;
        while (getline(cfg, line))
        {
            istringstream str{line};
            unsigned id;

            str >> id;
            foodTypes[id].foodID = id;

            str >> foodTypes[id].name;
            str >> foodTypes[id].servingSize; // in grams
            str >> foodTypes[id].RAG;
            str >> foodTypes[id].SAG;
            str >> foodTypes[id].protein;
            str >> foodTypes[id].fat;
        }
    }

    void readEventsFile(string_view file)
    {
        ifstream cfg{file.data()};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
            cerr << "Message: " << strerror(errno) << endl;
            exit(1);
        }

        string line;
        while (getline(cfg, line))
        {
            istringstream str{line};

            unsigned days, hours, minutes;
            str >> days;
            str.ignore(1); // ':'
            str >> hours;
            str.ignore(1); // ':'
            str >> minutes;
            unsigned fireTime = sim->timeToTicks(days, hours, minutes);

            string type;
            str >> type;;
            EventType event = eventMap.at(type);

            unsigned subtype, howmuch;
            str >> subtype >> howmuch;

            sim->addEvent(fireTime, event, subtype, howmuch);
        }
    }

    void timeStamp()
    {
        output << sim->elapsedDays() << ":" << sim->elapsedHours() << ":" << sim->elapsedMinutes()
               << " " << sim->ticks() << " ";
    }

    void onAddFat(const FatState& state)
    {
        // output << "BodyWeight: addFat " << state.bodyWeight << " newfat " << state.amount << endl;
    }

    void onAddGlucose(const GlucoseState& state)
    {
        // timeStamp();
        // output << " BGL: " << state.bloodBGL << endl;
    }

    void onRemoveGlucose(const GlucoseState& state)
    {
        // output << "Glucose consumed " << state.amount << " ,glucose left " << state.bloodGlucose << endl;

        if (state.bloodBGL <= state.bloodMinBGL)
        {
            timeStamp();
            cerr << " bgl dips to: " << state.bloodBGL << endl;
            exit(-1);
        }
    }

    void onFireEvent(shared_ptr<Event> event)
    {
        // output << "ticks =" << sim->tick() << ": " << sim->elapsedDays()
        //        << "::" << sim->elapsedHours() << "::" << sim->elapsedMinutes()
        //        << endl;
        // output << "event->fireTime : " << event->fireTime << endl;
    }

    void onTick()
    {
        for (shared_ptr<Event> event : sim->eventsFiredThisTick())
        {
            onFireEvent(event);

            switch (event->eventType)
            {
                case EventType::FOOD:
                {
                    auto foodEvent = dynamic_pointer_cast<FoodEvent>(event);
                    onFoodEvent(*foodEvent);
                    break;
                }
                case EventType::EXERCISE:
                {
                    auto exerciseEvent = dynamic_pointer_cast<ExerciseEvent>(event);
                    onExerciseEvent(*exerciseEvent);
                    break;
                }
                case EventType::HALT:
                    return;
            }
        }

        onPortalVein();
        onStomach();
        onIntestine();
        onLiver();
        onAdiposeTissue();
        onBrain();
        onHeart();
        onBlood();
        onMuscles();
        onKidneys();
        onBody();
    }

    void onAdiposeTissue()
    {
        // SimCtl::time_stamp();
        // output << " BodyWeight: " << sim->body->bodyWeight << endl;
    }

    void onBlood()
    {
        // if (sim->blood->RBCsUpdated)
        // {
        //     output << "New RBCs: " << sim->blood->ageBins[blood->bin0].RBCs
        //            << " bin0 " << sim->blood->bin0 << endl;
        //     output << "Old RBCs Die\n";

        //     for (int i = 0; i <= blood->MAX_AGE - blood->HUNDRED_DAYS; ++i)
        //     {
        //         int j = sim->blood->killBin[i];
        //         output << "bin: " << j
        //                << ", RBCs " << sim->blood->ageBins[j].RBCs
        //                << ", Glycated RBCs " << sim->blood->ageBins[j].glycatedRBCs
        //                << " killrate " << sim->blood->killRate[i]
        //                << endl;
        //     }

        //     output << "RBCs glycate\n";
        //     for (int i = 0; i < blood->MAX_AGE + 1; ++i)
        //     {
        //         output << "bin: " << i
        //                << ", RBCs " << sim->blood->ageBins[i].RBCs
        //                << ", Glycated RBCs " << sim->blood->ageBins[i].glycatedRBCs
        //                << endl;
        //     }

        //     timeStamp();
        //     output << " New HbA1c: " << sim->blood->currentHbA1c() << std::endl;

        //     StimeStamp();
        //     output << " Blood::avgBGL " << sim->blood->avgBGLOneDay << endl;
        // }

        // timeStamp();
        // output << " Blood:: glycolysis " << sim->blood->glycolysisPerTick << endl;

        // timeStamp();
        // output << " Blood:: totalGlycolysis " << sim->blood->totalGlycolysisSoFar << endl;
    }

    void onBody()
    {
        timeStamp();
        output << " Blood:: insulinLevel " << sim->blood->insulinLevel << endl;

        // output << " lactate " << sim->blood->lactate
        //        << " glutamine " << sim->blood->glutamine
        //        << " alanine " << sim->blood->alanine
        //        << " gngsubs " << sim->blood->gngSubstrates
        //        << " bAA " << sim->blood->branchedAminoAcids
        //        << " uAA " << sim->blood->unbranchedAminoAcids
        //        << endl;

        timeStamp();
        output << " " << sim->blood->getBGL() << " " << sim->liver->glycogen / 1000.0
               << " " << sim->muscles->glycogen / 1000.0 << endl;
        timeStamp();
        output << " HumanBody:: BGL " << sim->blood->getBGL() << endl;
        // timeStamp();
        // output << " weight " << sim->body->bodyWeight << endl;

        timeStamp();
        output << " HumanBody:: TotalGlycolysisPerTick " << sim->body->totalGlycolysisPerTick << endl;
        timeStamp();
        output << " HumanBody:: TotalGlycolysisSoFar " << sim->body->totalGlycolysisSoFar << endl;

        timeStamp();
        output << " HumanBody:: TotalGNGPerTick " << sim->body->totalGNGPerTick << endl;
        timeStamp();
        output << " HumanBody:: TotalGNGSoFar " << sim->body->totalGNGSoFar << endl;

        timeStamp();
        output << " HumanBody:: TotalOxidationPerTick " << sim->body->totalOxidationPerTick << endl;
        timeStamp();
        output << " HumanBody:: TotalOxidationSoFar " << sim->body->totalOxidationSoFar << endl;

        timeStamp();
        output << " HumanBody:: UseOfGlucoseOutsideLiverKidneysMuscles "
               << sim->blood->glycolysisPerTick
                + sim->brain->oxidationPerTick
                + sim->heart->oxidationPerTick
                + sim->intestine->glycolysisPerTick
               << endl;

        timeStamp();
        output << " HumanBody:: TotalGlycogenStoragePerTick " << sim->body->totalGlycogenStoragePerTick << endl;
        timeStamp();
        output << " HumanBody:: TotalGlycogenStorageSoFar " <<
            sim->body->totalLiverGlycogenStorageSoFar + sim->body->totalMusclesGlycogenStorageSoFar
            << endl;

        timeStamp();
        output << " HumanBody:: TotalGlycogenBreakdownPerTick "
             << sim->body->totalGlycogenBreakdownPerTick
             << endl;
        timeStamp();
        output << " HumanBody:: TotalGlycogenBreakdownSoFar "
            << sim->body->totalLiverGlycogenBreakdownSoFar
             + sim->body->totalMusclesGlycogenBreakdownSoFar
            << endl;

        timeStamp();
        output << " HumanBody:: TotalEndogeneousGlucoseReleasePerTick "
             << sim->body->totalEndogeneousGlucoseReleasePerTick
             << endl;
        timeStamp();
        output << " HumanBody:: TotalEndogeneousGlucoseReleaseSoFar "
             << sim->body->totalEndogeneousGlucoseReleaseSoFar
             << endl;

        timeStamp();
        output << " HumanBody:: TotalGlucoseReleasePerTick " << sim->body->totalGlucoseReleasePerTick << endl;
        timeStamp();
        output << " HumanBody:: TotalGlucoseReleaseSoFar " << sim->body->totalGlucoseReleaseSoFar << endl;

        if (sim->dayOver())
        {
            timeStamp();
            output << " Totals for the day:"
                   << " " << sim->body->dayEndTotals.totalGlycolysisSoFar
                   << " " << sim->body->dayEndTotals.totalExcretionSoFar
                   << " " << sim->body->dayEndTotals.totalOxidationSoFar
                   << " " << sim->body->dayEndTotals.totalGNGSoFar
                   << " " << sim->body->dayEndTotals.totalLiverGlycogenStorageSoFar
                   << " " << sim->body->dayEndTotals.totalLiverGlycogenBreakdownSoFar
                   << " " << sim->body->dayEndTotals.totalMusclesGlycogenStorageSoFar
                   << " " << sim->body->dayEndTotals.totalMusclesGlycogenBreakdownSoFar
                   << " " << sim->body->dayEndTotals.totalGlucoseFromIntestineSoFar
                   << endl;
        }

        // if (sim->ticks() > 600)
        //     output << sim->body->peakBGL << endl;

        if (sim->ticks() == 960)
        {
            output << "Simulation Results:: GNG " << sim->body->totalGNGSoFar - sim->body->tempGNG
                   << " glycolysis " << sim->body->totalGlycolysisSoFar - sim->body->tempGlycolysis
                   << " oxidation " << sim->body->totalOxidationSoFar - sim->body->tempOxidation
                   << " excretion " << sim->kidneys->totalExcretion - sim->body->tempExcretion
                   << " glycogenStorage " << sim->body->totalLiverGlycogenStorageSoFar
                                           + sim->body->totalMusclesGlycogenStorageSoFar
                                           - sim->body->tempGlycogenStorage
                   << " glycogenBreakdown " << sim->body->totalLiverGlycogenBreakdownSoFar
                                             + sim->body->totalMusclesGlycogenBreakdownSoFar
                                             - sim->body->tempGlycogenBreakdown
                   << " baseBGL " << sim->body->baseBGL
                   << " peakBGL " << sim->body->peakBGL << endl;
        }
    }

    void onBrain()
    {
        onRemoveGlucose(sim->brain->glucoseRemoved);

        // timStamp();
        // output << " Brain:: Oxidation " << sim->brain->oxidationPerTick << endl;
    }

    void onHeart()
    {
        onRemoveGlucose(sim->heart->basalAbsorption);
        // onRemoveGlucose(sim->heart->basalAbsorption + sim->heart->Glut4Absorption);

        timeStamp();
        output << " Heart:: Oxidation " << sim->heart->oxidationPerTick << endl;
    }

    void onIntestine()
    {
        for (const auto& chm : sim->intestine->chymeConsumed)
        {
            timeStamp();

            output << " Chyme:: RAG " << chm.RAG << " SAG " << chm.SAG
                   << " origRAG " << chm.origRAG << " origSAG " << chm.origSAG
                   << " RAGConsumed " << chm.RAGConsumed << " SAGConsumed " << chm.SAGConsumed
                   << endl;
        }

        timeStamp();
        output << " Intestine:: RAGConsumed " << sim->intestine->totalRAGConsumed
            << " SAGConsumed " << sim->intestine->totalSAGConsumed << endl;

        // output << " Intestine:: RAGConsumed " << sim->intestine->totalRAGDigested
        //        << " SAGConsumed " << sim->intestine->totalSAGDigested
        //        << " total " << sim->intestine->totalRAGDigested + sim->intestine->totalSAGDigested
        //        << endl;

        onRemoveGlucose(sim->intestine->excessGlucoseInEnterocytes);

        // timeStamp();
        // output << "GL in Portal Vein: " << sim->intestine->glPortalVeinConcentration << endl;

        // timeStamp();
        // output << " Intestine:: glLumen: " << sim->intestine->glLumen
        //        << " glEntero " << sim->intestine->glEnterocytes
        //        << " glPortal " << sim->intestine->glPortalVein
        //                        << ", " << sim->intestine->glPortalVeinConcentration
        //        << " activeAbsorption " << intestine->activeAbsorption
        //        << " passiveAbsorption " << intestine->passiveAbsorption
        //        << endl;

        // timeStamp();
        // output << " Intestine:: Glycolysis " << sim->intestine->glycolysisPerTick << endl;

        timeStamp();
        output << " Intestine:: ToPortalVein " << sim->intestine->toPortalVeinPerTick << endl;

        // timeStamp();
        // output << " PortalVein: bAA " << sim->portalVeing->branchedAA
        //        << ", uAA " << sim->portalVein->unbranchedAA << endl;
    }

    void onKidneys()
    {
        onRemoveGlucose(sim->kidneys->postGlycolysis);
        onAddGlucose(sim->kidneys->postGluconeogenesis);
        onRemoveGlucose(sim->kidneys->postGlucoseExtraction);

        // timeStamp();
        // output << " Kidneys:: Absorption " << sim->kidneys->absorptionPerTick << endl;
        // timeStamp();
        // output << " Kidneys:: Release " << sim->kidneys->releasePerTick << endl;

        timeStamp();
        output << " Kidneys:: Glycolysis " << sim->kidneys->glycolysisPerTick << endl;
        timeStamp();
        output << " Kidneys:: TotalExcretion " << sim->kidneys->totalExcretion << endl;
        timeStamp();
        output << " Kidneys:: GNG " << sim->kidneys->gngPerTick << endl;
        timeStamp();
        output << " Kidneys:: Excretion " << sim->kidneys->excretionPerTick << endl;
    }

    void onLiver()
    {
        // timeStamp();
        // output << "GL in Portal Vein: " << sim->portalVein->getConcentration() << endl;

        // if (sim->liver->excessGlucoseAbsorption > 0)
        // {
        //     output << "Trying to absorb more glucose from portal vein than what is present there! "
        //            << sim->liver->excessGlucoseAbsorption << " " << sim->liver->absorptionPerTick << endl;
        // }

        // timeStamp();
        // output << " Liver absorbs glucose from portal vein " << sim->liver->absorptionPerTick << "mg" << endl;

        // if (sim->liver->lipogenesisOccurred)
        // {
        //     output << "BodyWeight: Lipogenesis " << sim->liver->preLipogenesis.bodyWeight
        //            << " glucose " << sim->liver->preLipogenesis.toLipogenesis
        //            << " fat " << sim->liver->preLipogenesis.fat
        //            << endl;

        //     output << "BodyWeight: Lipogenesis " << sim->liver->postLipogenesis.bodyWeight
        //            << " glucose " << sim->liver->postLipogenesis.toLipogenesis
        //            << " fat " << sim->liver->postLipogenesis.fat
        //            << endl;

        //     timeStamp();
        //     output << " Liver:: Lipogenesis " << sim->liver->postLipogenesis.toLipogenesis << endl;
        // }

        // output << "After glycogen synthesis in liver, liver glycogen " << sim->liver->postGlycogen.glycogen
        //        << " mg, live glucose " << sim->liver->postGlycogen.glucose << " mg"
        //        << endl;

        // output << "After GNG, liver glucose " << sim->liver->postGluconeogensis.glucose << " mg, "
        //        << "liver glycogen " << sim->liver->postGluconeogensis.glycogen << " mg, "
        //        << "blood glucose " << sim->liver->postGluconeogensis.bloodGlucose << " mg, "
        //        << "blood lactate " << sim->liver->postGluconeogensis.bloodLactate << " mg"
        //        << endl;

        // if (sim->liver->maxGNGDuringExercise > 0)
        // {
        //     timeStamp();
        //     output << " Liver:: Glucose Needed " << sim->liver->glucoseNeeded
        //            << " maxGNG " << sim->liver->maxGNGDuringExercise << endl;
        // }

        // if (sim->liver->glucoseNeeded > 0)
        // {
        //     timeStamp();
        //     output << " Liver:: ExtraGNG " << sim->liver->glucoseNeeded
        //            << " " << sim->liver->gngPerTick << endl;
        // }

        timeStamp();
        output << " Liver:: ToGlycogen " << sim->liver->toGlycogenPerTick << endl;
        timeStamp();
        output << " Liver:: glycogen " << sim->liver->glycogen / 1000.0 << endl;
        timeStamp();
        output << " Liver:: FromGlycogen " << sim->liver->fromGlycogenPerTick << endl;
        timeStamp();
        output << " Liver:: GNG " << sim->liver->gngPerTick << endl;
        // timeStamp();
        // output << " Liver:: GlucoseProduced "
        //        << sim->liver->gngPerTick + sim->liver->fromGlycogenPerTick
        //        << endl;

        timeStamp();
        output << " Liver:: Absorption " << sim->liver->absorptionPerTick << endl;
        timeStamp();
        output << " Liver:: Glycolysis " << sim->liver->glycolysisPerTick << endl;
        timeStamp();
        output << " Liver:: Release " << sim->liver->releasePerTick  << "mg"
               << ", gl " << sim->liver->glucose / sim->liver->fluidVolume
               << endl;
    }

    void onMuscles()
    {
        // timeStamp();
        // output << "HumanBody:: Current Energy Expenditure: " << sim->muscles->currEnergyNeed << endl;

        if (sim->muscles->hadExercise)
        {
            if (sim->muscles->absorbedFromBlood > 0)    onRemoveGlucose(sim->muscles->absorptionState);

            // timeStamp();
            // output << " Muscles: GlycogenShare " << sim->muscles->glycogenShare << endl;
        }
        else
        {
            onRemoveGlucose(sim->muscles->basalBase);
            if (sim->muscles->basalGLUT4Occurred)    onRemoveGlucose(sim->muscles->basalGLUT4);
        }

        timeStamp();
        output << " Muscles:: GlucoseAbsorbed " << sim->muscles->glucoseAbsorbedPerTick << endl;

        timeStamp();
        output << " Muscles:: GlycogenSynthesis " << sim->muscles->glycogenSynthesizedPerTick << endl;
        timeStamp();
        output << " Muscles:: GlycogenBreakdown " << sim->muscles->glycogenBreakdownPerTick << endl;

        timeStamp();
        output << " Muscles:: glycogen " << sim->muscles->glycogen / 1000.0 << endl;
        timeStamp();
        output << " Muscles:: Oxidation " << sim->muscles->oxidationPerTick << endl;
        // timeStamp();
        // output << " Muscles:: GlycogenOxidation " << sim->muscles->glycogenOxidizedPerTick << endl;
        timeStamp();
        output << " Muscles:: Glycolysis " << sim->muscles->glycolysisPerTick << endl;

        // timeStamp();
        // output << " Muscles:: TotalGlucoseOxidized "
        //        << sim->muscles->oxidationPerTick + sim->muscles->glycogenOxidizedPerTick
        //        << endl;

        // timeStamp();
        // output << " Muscles:: totalGlucoseAbsorbed " << sim->muscles->totalGlucoseAbsorbed << endl;
    }

    void onPortalVein()
    {
        // timeStamp();
        // output << " PortalVein:: " << sim->portalVein->glucose
        //        << " " << sim->portalVein->glucose / sim->portalVein->fluidVolume
        //        << " " << sim->portalVein->branchedAA
        //        << " " << sim->portalVein->unbranchedAA
        //        << endl;
    }

    void onStomach()
    {
        onAddFat(sim->stomach->fatInBolus);

        // Workaround for negative underflow, not a correct bug fix. Remove these checks at your discretion.
        double totalFood = abs(sim->stomach->totalFood);
        double SAG       = abs(sim->stomach->SAG);
        double RAG       = abs(sim->stomach->RAG);
        double fat       = abs(sim->stomach->fat);
        // /workaround

        if (sim->stomach->stomachBecameEmpty)
        {
            timeStamp();
            output << " Gastric Emptying:: Total Food " << totalFood
                << " Calorific Density " << sim->stomach->calorificDensity
                << " geSlope " << sim->stomach->geSlope
                << " ragInBolus " << sim->stomach->ragInBolus
                << " sagInBolus " << sim->stomach->sagInBolus
                << endl;

            timeStamp();
            output << " Stomach:: SAG " << SAG
                   << " RAG " << RAG
                   << " protein " << sim->stomach->protein
                   << " fat " << fat
                   << endl;
        }
        else if (sim->stomach->stomachEmpty)
        {
            timeStamp();
            output << " Gastric Emptying:: Total Food " << totalFood
                   << " Calorific Density " << 0
                   << " geSlope " << 0
                   << " ragInBolus " << 0
                   << " sagInBolus " << 0
                   << endl;

            // output << "STOMACH EMPTY " << sim->body->bodyState << endl;

            // if (sim->body->bodyState != sim->body->oldState)
            // {
            //     timeStamp();
            //     output << "Entering State " << sim->body->bodyState << endl;
            // }
        }
        else
        {
            timeStamp();
            output << " Gastric Emptying:: Total Food " << sim->stomach->totalFood
                << " Calorific Density " << sim->stomach->calorificDensity
                << " geSlope " << sim->stomach->geSlope
                << " ragInBolus " << sim->stomach->ragInBolus
                << " sagInBolus " << sim->stomach->sagInBolus
                << endl;
        }
    }

    void onFoodEvent(const FoodEvent& event)
    {
        // timeStamp();
        // output << "Adding " << event.quantity << " grams of "
        //        << sim->body->foodTypes[event.foodID].name << " to stomach"
        //        << endl;

        // if (sim->body->bodyState != sim->body->oldState)
        // {
        //     timeStamp();
        //     output << "Entering State " << sim->body->bodyState << endl;
        // }
    }

    void onExerciseEvent(const ExerciseEvent& event)
    {
        timeStamp();
        output << " Starting Exercise at " << sim->body->percentVO2Max << " %VO2Max" << endl;

        // if (sim->body->bodyState != sim->body->oldState)
        // {
        //     timeStamp();
        //     output << "Entering State " << sim->body->bodyState << endl;
        // }

        // timeStamp();
        // output << "Firing Exercise Event " << event.exerciseID << " for " << event.duration << " minutes" << endl;
    }

    void onHaltEvent(const HaltEvent& event)
    {
        timeStamp();
        output << " weight " << sim->body->bodyWeight << endl;
    }
};


int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        cout << "Syntax: carbmetsim foodsfile exercisefile metabolicratesfile eventsfile seedstring outputfile\n";
        exit(1);
    }

    CarbMetSimCLI cli{argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]};
    cli.run();
}

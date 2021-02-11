#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include "source/HumanBody.h"
#include "source/Stomach.h"

using namespace std;


using ParamMap = map<string, map<string, double*>>;

ParamMap createParamMap(MetabolicParams& params)
{
    ParamMap out;

    // AdiposeTissue
    // Currently none

    // Blood
    out["BLOOD"]["rbcBirthRate"]        = &params.bloodParams.rbcBirthRate;
    out["BLOOD"]["glycationProbSlope"]  = &params.bloodParams.glycationProbSlope;
    out["BLOOD"]["glycationProbConst"]  = &params.bloodParams.glycationProbConst;
    out["BLOOD"]["minGlucoseLevel"]     = &params.bloodParams.minGlucoseLevel;
    out["BLOOD"]["baseGlucoseLevel"]    = &params.bloodParams.baseGlucoseLevel;
    out["BLOOD"]["baseInsulinLevel"]    = &params.bloodParams.baseInsulinLevel;
    out["BLOOD"]["peakInsulinLevel"]    = &params.bloodParams.peakInsulinLevel;
    out["BLOOD"]["highGlucoseLevel"]    = &params.bloodParams.highGlucoseLevel;
    out["BLOOD"]["highLactateLevel"]    = &params.bloodParams.highLactateLevel;
    out["BLOOD"]["glycolysisMin"]       = &params.bloodParams.glycolysisMin;
    out["BLOOD"]["glycolysisMax"]       = &params.bloodParams.glycolysisMax;
    out["BLOOD"]["glycolysisToLactate"] = &params.bloodParams.glycolysisToLactate;

    // Brain
    out["BRAIN"]["glucoseOxidized"]  = &params.brainParams.glucoseOxidized;
    out["BRAIN"]["glucoseToAlanine"] = &params.brainParams.glucoseToAlanine;
    out["BRAIN"]["bAAToGlutamine"]   = &params.brainParams.bAAToGlutamine;

    // Heart
    out["HEART"]["lactateOxidized"]      = &params.heartParams.lactateOxidized;
    out["HEART"]["basalGlucoseAbsorbed"] = &params.heartParams.basalGlucoseAbsorbed;
    out["HEART"]["Glut4Km"]              = &params.heartParams.Glut4Km;
    out["HEART"]["Glut4VMAX"]            = &params.heartParams.Glut4VMAX;

    // HumanBody
    out["HUMAN_BODY"]["age"]                          = &params.humanParams.age;
    out["HUMAN_BODY"]["gender"]                       = &params.humanParams.gender;
    out["HUMAN_BODY"]["fitnessLevel"]                 = &params.humanParams.fitnessLevel;
    out["HUMAN_BODY"]["glut4Impact"]                  = &params.humanParams.glut4Impact;
    out["HUMAN_BODY"]["glycolysisMinImpact"]          = &params.humanParams.glycolysisMinImpact;
    out["HUMAN_BODY"]["glycolysisMaxImpact"]          = &params.humanParams.glycolysisMaxImpact;
    out["HUMAN_BODY"]["excretionKidneysImpact"]       = &params.humanParams.excretionKidneysImpact;
    out["HUMAN_BODY"]["liverGlycogenBreakdownImpact"] = &params.humanParams.liverGlycogenBreakdownImpact;
    out["HUMAN_BODY"]["liverGlycogenSynthesisImpact"] = &params.humanParams.liverGlycogenSynthesisImpact;
    out["HUMAN_BODY"]["maxLiverGlycogenBreakdownDuringExerciseImpact"]
        = &params.humanParams.maxLiverGlycogenBreakdownDuringExerciseImpact;
    out["HUMAN_BODY"]["gngImpact"]                        = &params.humanParams.gngImpact;
    out["HUMAN_BODY"]["bodyWeight"]                       = &params.humanParams.bodyWeight;
    out["HUMAN_BODY"]["insulinImpactOnGlycolysis_Mean"]   = &params.humanParams.insulinImpactOnGlycolysis_Mean;
    out["HUMAN_BODY"]["insulinImpactOnGlycolysis_StdDev"] = &params.humanParams.insulinImpactOnGlycolysis_StdDev;
    out["HUMAN_BODY"]["insulinImpactOnGNG_Mean"]          = &params.humanParams.insulinImpactOnGNG_Mean;
    out["HUMAN_BODY"]["insulinImpactOnGNG_StdDev"]        = &params.humanParams.insulinImpactOnGNG_StdDev;
    out["HUMAN_BODY"]["insulinImpactGlycogenBreakdownInLiver_Mean"]
        = &params.humanParams.insulinImpactGlycogenBreakdownInLiver_Mean;
    out["HUMAN_BODY"]["insulinImpactGlycogenBreakdownInLiver_StdDev"]
        = &params.humanParams.insulinImpactGlycogenBreakdownInLiver_StdDev;
    out["HUMAN_BODY"]["insulinImpactGlycogenSynthesisInLiver_Mean"]
        = &params.humanParams.insulinImpactGlycogenSynthesisInLiver_Mean;
    out["HUMAN_BODY"]["insulinImpactGlycogenSynthesisInLiver_StdDev"]
        = &params.humanParams.insulinImpactGlycogenSynthesisInLiver_StdDev;
    out["HUMAN_BODY"]["intensityPeakGlucoseProd"] = &params.humanParams.intensityPeakGlucoseProd;

    // Intestine
    out["INTESTINE"]["aminoAcidAbsorptionRate"]    = &params.intestineParams.aminoAcidsAbsorptionRate;
    out["INTESTINE"]["glutamineOxidationRate"]     = &params.intestineParams.glutamineOxidationRate;
    out["INTESTINE"]["glutamineToAlanineFraction"] = &params.intestineParams.glutamineToAlanineFraction;
    out["INTESTINE"]["Glut2VMAX_In"]               = &params.intestineParams.Glut2VMAX_In;
    out["INTESTINE"]["Glut2Km_In"]                 = &params.intestineParams.Glut2Km_In;
    out["INTESTINE"]["Glut2VMAX_Out"]              = &params.intestineParams.Glut2VMAX_Out;
    out["INTESTINE"]["Glut2Km_Out"]                = &params.intestineParams.Glut2Km_Out;
    out["INTESTINE"]["sglt1Rate"]                  = &params.intestineParams.sglt1Rate;
    out["INTESTINE"]["fluidVolumeInLumen"]         = &params.intestineParams.fluidVolumeInLumen;
    out["INTESTINE"]["fluidVolumeInEnterocytes"]   = &params.intestineParams.fluidVolumeInEnterocytes;
    out["INTESTINE"]["glycolysisMin"]              = &params.intestineParams.glycolysisMin;
    out["INTESTINE"]["glycolysisMax"]              = &params.intestineParams.glycolysisMax;
    out["INTESTINE"]["RAG_Mean"]                   = &params.intestineParams.RAG_Mean;
    out["INTESTINE"]["RAG_StdDev"]                 = &params.intestineParams.RAG_StdDev;
    out["INTESTINE"]["SAG_Mean"]                   = &params.intestineParams.SAG_Mean;
    out["INTESTINE"]["SAG_StdDev"]                 = &params.intestineParams.SAG_StdDev;

    // Kidneys
    out["KIDNEYS"]["glycolysisMin"] = &params.kidneysParams.glycolysisMin;
    out["KIDNEYS"]["glycolysisMax"] = &params.kidneysParams.glycolysisMax;
    out["KIDNEYS"]["gngKidneys"]    = &params.kidneysParams.gngKidneys;

    // Liver
    out["LIVER"]["Glycogen"]                    = &params.liverParams.glycogen;
    out["LIVER"]["MaxGlycogen"]                 = &params.liverParams.glycogenMax;
    out["LIVER"]["fluidVolume"]                 = &params.liverParams.fluidVolume;
    out["LIVER"]["Glut2Km"]                     = &params.liverParams.Glut2Km;
    out["LIVER"]["Glut2VMAX"]                   = &params.liverParams.Glut2VMAX;
    out["LIVER"]["glucoseToGlycogenInLiver"]    = &params.liverParams.glucoseToGlycogenInLiver;
    out["LIVER"]["glycogenToGlucoseInLiver"]    = &params.liverParams.glycogenToGlucoseInLiver;
    out["LIVER"]["glycolysisMin"]               = &params.liverParams.glycolysisMin;
    out["LIVER"]["glycolysisMax"]               = &params.liverParams.glycolysisMax;
    out["LIVER"]["glycolysisToLactateFraction"] = &params.liverParams.glycolysisToLactateFraction;
    out["LIVER"]["gngLiver"]                    = &params.liverParams.gngLiver;
    out["LIVER"]["glucoseToNEFA"]               = &params.liverParams.glucoseToNEFA;
    out["LIVER"]["maxLipogenesis"]              = &params.liverParams.maxLipogenesis;

    // Muscles
    out["MUSCLES"]["Glycogen"]                           = &params.musclesParams.glycogen;
    out["MUSCLES"]["MaxGlycogen"]                        = &params.musclesParams.glycogenMax;
    out["MUSCLES"]["Glut4Km"]                            = &params.musclesParams.Glut4Km;
    out["MUSCLES"]["Glut4VMAX"]                          = &params.musclesParams.Glut4VMAX;
    out["MUSCLES"]["PeakGlut4VMAX"]                      = &params.musclesParams.PeakGlut4VMAX;
    out["MUSCLES"]["maxGlucoseAbsorptionDuringExercise"] = &params.musclesParams.maxGlucoseAbsorptionDuringExercise;
    out["MUSCLES"]["basalGlucoseAbsorbed"]               = &params.musclesParams.basalGlucoseAbsorbed;
    out["MUSCLES"]["baaToGlutamine"]                     = &params.musclesParams.baaToGlutamine;
    out["MUSCLES"]["glycolysisMin"]                      = &params.musclesParams.glycolysisMin;
    out["MUSCLES"]["glycolysisMax"]                      = &params.musclesParams.glycolysisMax;
    out["MUSCLES"]["glucoseToGlycogen"]                  = &params.musclesParams.glucoseToGlycogen;
    out["MUSCLES"]["glycogenShareDuringExerciseMean"]    = &params.musclesParams.glycogenShareDuringExerciseMean;

    // Portal Vein
    out["PORTAL_VEIN"]["fluidVolume"] = &params.portalVeinParams.fluidVolume;

    // Stomach
    out["STOMACH"]["geConstant"] = &params.stomachParams.geConstant;
    out["STOMACH"]["geSlopeMin"] = &params.stomachParams.geSlopeMin;


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
bool contains(vector<T> v, T t)
{
    return find(v.begin(), v.end(), t) != v.end();
}


class CarbMetSimCLI
{
public:
    CarbMetSimCLI(const char* foodsFile, const char* exerciseFile, const char* metabolicRatesFile,
                  const char* eventsFile, const char* seedString, const char* outputFile)
        : sim{make_shared<SimCtl>(body, seedString)},
          paramMap{createMetabolicMap(metabolicParameters)},
          eventMap{createEventMap()}
    {
        // body
        readFoodFile(foodsFile);
        readExerciseFile(exerciseFile);
        readMetabolicParams(metabolicRatesFile);
        body = make_shared<HumanBody>(sim, metabolicParameters, foodTypes, exerciseTypes);

        // sim
        readEventsFile(eventsFile);

        // this
        // Get states automatically so they only need to be updated in one place
        bodyStates = get_keys(paramMap);
        bodyStates.push_back("ALL");

        output.open(outputFile);
        if (!output)
        {
            cerr << "Error opening " << outputFile << endl;
            exit(1);
        }
    }

    void run()
    {
        while (sim->runTick())    onTick();

        auto haltEvent = dynamic_pointer_cast<HaltEvent>(sim->eventsFiredThisTick().back());
        onHaltEvent(*haltEvent);
    }

private:
    shared_ptr<SimCtl>              sim;
    shared_ptr<HumanBody>           body;
    map<BodyState, MetabolicParams> metabolicParameters;
    map<unsigned, ExerciseType>     exerciseTypes;
    map<unsigned, FoodType>         foodTypes;
    ofstream                        output;

    map<string, ParamMap>  paramMap;
    map<string, EventType> eventMap;
    vector<string>         bodyStates;

    void readMetabolicParams(const char* file)
    {
        ifstream cfg{file};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
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
                cerr << "Unrecognized body state: " << bodyState;
                exit(1);
            }

            if (!paramMap[bodyStates.front()].contains(bodyOrgan))
            {
                cerr << "Unrecognized organ: " << bodyOrgan;
                exit(1);
            }

            if (!paramMap[bodyStates.front()][bodyOrgan].contains(param))
            {
                cerr << "Unrecognized parameter for organ \"" << bodyOrgan << "\": " << param;
                exit(1);
            }

            if (bodyState == "ALL")
            {
                for (auto& stateEntry : paramMap)
                    *stateEntry.second[bodyOrgan][param] = val;
            }
            else
                *paramMap[bodyState][bodyOrgan][param] = val;
        }
    }

    void readExerciseFile(const char* file)
    {
        ifstream cfg{file};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
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

    void readFoodFile(const char* file)
    {
        ifstream cfg{file};
        if (!cfg)
        {
            cerr << "Error opening " << file << endl;
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

    void readEventsFile(const char* file)
    {
        ifstream cfg{file};
        if (!cfg)
        {
            cout << "Error opening " << file << endl;
            exit(1);
        }

        string line;
        while (getline(cfg, line))
        {
            istringstream str{line};
            istringstream buf;

            // get the time stamp
            string   type;
            unsigned subtype, howmuch, days, hours, minutes;
            str >> type >> subtype;

            str.get(*buf.rdbuf(), ':');
            buf >> howmuch;

            str.get(*buf.rdbuf(), ':');
            buf >> days;

            str.get(*buf.rdbuf(), ':');
            buf >> hours;

            str >> minutes;

            unsigned fireTime = sim->timeToTicks(days, hours, minutes);
            EventType event = eventMap.at(type);

            sim->addEvent(fireTime, event, subtype, howmuch);
        }
    }

    void timeStamp()
    {
        cout << sim->elapsedDays() << ":" << sim->elapsedHours() << ":" << sim->elapsedMinutes()
             << " " << sim->ticks() << " ";
    }

    void onAddFat(const FatState& state)
    {
        // cout << "BodyWeight: addFat " << state.bodyWeight << " newfat " << state.amount << endl;
    }

    void onAddGlucose(const GlucoseState& state)
    {
        // timeStamp();
        // cout << " BGL: " << state.bloodBGL << endl;
    }

    void onRemoveGlucose(const GlucoseState& state)
    {
        // cout << "Glucose consumed " << state.amount << " ,glucose left " << state.bloodGlucose << endl;

        if (state.bloodBGL <= state.bloodMinBGL)
        {
            timeStamp();
            cerr << " bgl dips to: " << state.bloodBGL << endl;
            exit(-1);
        }
    }

    void onFireEvent(shared_ptr<Event> event)
    {
        // cout << "ticks =" << sim->tick() << ": " << sim->elapsedDays()
        //      << "::" << sim->elapsedHours() << "::" << sim->elapsedMinutes()
        //      << endl;
        // cout << "event->fireTime : " << event->fireTime << endl;
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
                // EventType::HALT is unreachable since this function is only called by CarbMetSimCLI::run()
                // case EventType::HALT:
                //     return;
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
        // cout << " BodyWeight: " << body->bodyWeight << endl;
    }

    void onBlood()
    {
        // if (body->blood->RBCsUpdated)
        // {
        //     cout << "New RBCs: " << body->blood->ageBins[blood->bin0].RBCs
        //          << " bin0 " << body->blood->bin0 << endl;
        //     cout << "Old RBCs Die\n";

        //     for (int i = 0; i <= blood->MAX_AGE - blood->HUNDRED_DAYS; ++i)
        //     {
        //         int j = body->blood->killBin[i];
        //         cout << "bin: " << j
        //              << ", RBCs " << body->blood->ageBins[j].RBCs
        //              << ", Glycated RBCs " << body->blood->ageBins[j].glycatedRBCs
        //              << " killrate " << body->blood->killRate[i]
        //              << endl;
        //     }

        //     cout << "RBCs glycate\n";
        //     for (int i = 0; i < blood->MAX_AGE + 1; ++i)
        //     {
        //         cout << "bin: " << i
        //              << ", RBCs " << body->blood->ageBins[i].RBCs
        //              << ", Glycated RBCs " << body->blood->ageBins[i].glycatedRBCs
        //              << endl;
        //     }

        //     timeStamp();
        //     cout << " New HbA1c: " << body->blood->currentHbA1c() << std::endl;

        //     StimeStamp();
        //     cout << " Blood::avgBGL " << body->blood->avgBGLOneDay << endl;
        // }

        // timeStamp();
        // cout << " Blood:: glycolysis " << body->blood->glycolysisPerTick << endl;

        // timeStamp();
        // cout << " Blood:: totalGlycolysis " << body->blood->totalGlycolysisSoFar << endl;
    }

    void onBody()
    {
        timeStamp();
        cout << " Blood:: insulinLevel " << body->blood->insulinLevel << endl;

        // cout << " lactate " << body->blood->lactate
        //      << " glutamine " << body->blood->glutamine
        //      << " alanine " << body->blood->alanine
        //      << " gngsubs " << body->blood->gngSubstrates
        //      << " bAA " << body->blood->branchedAminoAcids
        //      << " uAA " << body->blood->unbranchedAminoAcids
        //      << endl;

        timeStamp();
        cout << " " << body->blood->getBGL() << " " << body->liver->glycogen / 1000.0
             << " " << body->muscles->glycogen / 1000.0 << endl;
        timeStamp();
        cout << " HumanBody:: BGL " << body->blood->getBGL() << endl;
        // timeStamp();
        // cout << " weight " << body->bodyWeight << endl;

        timeStamp();
        cout << " HumanBody:: TotalGlycolysisPerTick " << body->totalGlycolysisPerTick << endl;
        timeStamp();
        cout << " HumanBody:: TotalGlycolysisSoFar " << body->totalGlycolysisSoFar << endl;

        timeStamp();
        cout << " HumanBody:: TotalGNGPerTick " << body->totalGNGPerTick << endl;
        timeStamp();
        cout << " HumanBody:: TotalGNGSoFar " << body->totalGNGSoFar << endl;

        timeStamp();
        cout << " HumanBody:: TotalOxidationPerTick " << body->totalOxidationPerTick << endl;
        timeStamp();
        cout << " HumanBody:: TotalOxidationSoFar " << body->totalOxidationSoFar << endl;

        timeStamp();
        cout << " HumanBody:: UseOfGlucoseOutsideLiverKidneysMuscles "
             << body->blood->glycolysisPerTick
              + body->brain->oxidationPerTick
              + body->heart->oxidationPerTick
              + body->intestine->glycolysisPerTick
             << endl;

        timeStamp();
        cout << " HumanBody:: TotalGlycogenStoragePerTick " << body->totalGlycogenStoragePerTick << endl;
        timeStamp();
        cout << " HumanBody:: TotalGlycogenStorageSoFar " <<
            body->totalLiverGlycogenStorageSoFar + body->totalMusclesGlycogenStorageSoFar
            << endl;

        timeStamp();
        cout << " HumanBody:: TotalGlycogenBreakdownPerTick "
             << body->totalGlycogenBreakdownPerTick
             << endl;
        timeStamp();
        cout << " HumanBody:: TotalGlycogenBreakdownSoFar "
            << body->totalLiverGlycogenBreakdownSoFar
             + body->totalMusclesGlycogenBreakdownSoFar
            << endl;

        timeStamp();
        cout << " HumanBody:: TotalEndogeneousGlucoseReleasePerTick "
             << body->totalEndogeneousGlucoseReleasePerTick
             << endl;
        timeStamp();
        cout << " HumanBody:: TotalEndogeneousGlucoseReleaseSoFar "
             << body->totalEndogeneousGlucoseReleaseSoFar
             << endl;

        timeStamp();
        cout << " HumanBody:: TotalGlucoseReleasePerTick " << body->totalGlucoseReleasePerTick << endl;
        timeStamp();
        cout << " HumanBody:: TotalGlucoseReleaseSoFar " << body->totalGlucoseReleaseSoFar << endl;

        if (sim->dayOver())
        {
            timeStamp();
            cout << " Totals for the day:"
                 << " " << body->dayEndTotals.totalGlycolysisSoFar
                 << " " << body->dayEndTotals.totalExcretionSoFar
                 << " " << body->dayEndTotals.totalOxidationSoFar
                 << " " << body->dayEndTotals.totalGNGSoFar
                 << " " << body->dayEndTotals.totalLiverGlycogenStorageSoFar
                 << " " << body->dayEndTotals.totalLiverGlycogenBreakdownSoFar
                 << " " << body->dayEndTotals.totalMusclesGlycogenStorageSoFar
                 << " " << body->dayEndTotals.totalMusclesGlycogenBreakdownSoFar
                 << " " << body->dayEndTotals.totalGlucoseFromIntestineSoFar
                 << endl;
        }

        // if (sim->ticks() > 600)
        //     cout << body->peakBGL << endl;

        if (sim->ticks() == 960)
        {
            cout << "Simulation Results:: GNG " << body->totalGNGSoFar - body->tempGNG
                << " glycolysis " << body->totalGlycolysisSoFar - body->tempGlycolysis
                << " oxidation " << body->totalOxidationSoFar - body->tempOxidation
                << " excretion " << body->kidneys->totalExcretion - body->tempExcretion
                << " glycogenStorage " << body->totalLiverGlycogenStorageSoFar
                                        + body->totalMusclesGlycogenStorageSoFar
                                        - body->tempGlycogenStorage
                << " glycogenBreakdown " << body->totalLiverGlycogenBreakdownSoFar
                                          + body->totalMusclesGlycogenBreakdownSoFar
                                          - body->tempGlycogenBreakdown
                << " baseBGL " << body->baseBGL
                << " peakBGL " << body->peakBGL << endl;
        }
    }

    void onBrain()
    {
        onRemoveGlucose(body->brain->glucoseRemoved);

        // timStamp();
        // cout << " Brain:: Oxidation " << body->brain->oxidationPerTick << endl;
    }

    void onHeart()
    {
        onRemoveGlucose(body->heart->basalAbsorption);
        // onRemoveGlucose(body->heart->basalAbsorption + body->heart->Glut4Absorption);

        timeStamp();
        cout << " Heart:: Oxidation " << body->heart->oxidationPerTick << endl;
    }

    void onIntestine()
    {
        for (const auto& chm : body->intestine->chymeConsumed)
        {
            timeStamp();

            cout << " Chyme:: RAG " << chm.RAG << " SAG " << chm.SAG
                 << " origRAG " << chm.origRAG << " origSAG " << chm.origSAG
                 << " RAGConsumed " << chm.RAGConsumed << " SAGConsumed " << chm.SAGConsumed
                 << endl;
        }

        timeStamp();
        cout << " Intestine:: RAGConsumed " << body->intestine->totalRAGConsumed
            << " SAGConsumed " << body->intestine->totalSAGConsumed << endl;

        // cout << " Intestine:: RAGConsumed " << body->intestine->totalRAGDigested
        //      << " SAGConsumed " << body->intestine->totalSAGDigested
        //      << " total " << body->intestine->totalRAGDigested + body->intestine->totalSAGDigested
        //      << endl;

        onRemoveGlucose(body->intestine->excessGlucoseInEnterocytes);

        // timeStamp();
        // cout << "GL in Portal Vein: " << body->intestine->glPortalVeinConcentration << endl;

        // timeStamp();
        // cout << " Intestine:: glLumen: " << body->intestine->glLumen
        //      << " glEntero " << body->intestine->glEnterocytes
        //      << " glPortal " << body->intestine->glPortalVein
        //          << ", " << body->intestine->glPortalVeinConcentration
        //      << " activeAbsorption " << intestine->activeAbsorption
        //      << " passiveAbsorption " << intestine->passiveAbsorption
        //      << endl;

        // timeStamp();
        // cout << " Intestine:: Glycolysis " << body->intestine->glycolysisPerTick << endl;

        timeStamp();
        cout << " Intestine:: ToPortalVein " << body->intestine->toPortalVeinPerTick << endl;

        // timeStamp();
        // cout << " PortalVein: bAA " << body->portalVeing->branchedAA
        //      << ", uAA " << body->portalVein->unbranchedAA << endl;
    }

    void onKidneys()
    {
        onRemoveGlucose(body->kidneys->postGlycolysis);
        onAddGlucose(body->kidneys->postGluconeogenesis);
        onRemoveGlucose(body->kidneys->postGlucoseExtraction);

        // timeStamp();
        // cout << " Kidneys:: Absorption " << body->kidneys->absorptionPerTick << endl;
        // timeStamp();
        // cout << " Kidneys:: Release " << body->kidneys->releasePerTick << endl;

        timeStamp();
        cout << " Kidneys:: Glycolysis " << body->kidneys->glycolysisPerTick << endl;
        timeStamp();
        cout << " Kidneys:: TotalExcretion " << body->kidneys->totalExcretion << endl;
        timeStamp();
        cout << " Kidneys:: GNG " << body->kidneys->gngPerTick << endl;
        timeStamp();
        cout << " Kidneys:: Excretion " << body->kidneys->excretionPerTick << endl;
    }

    void onLiver()
    {
        // timeStamp();
        // cout << "GL in Portal Vein: " << body->portalVein->getConcentration() << endl;

        // if (body->liver->excessGlucoseAbsorption > 0)
        // {
        //     cout << "Trying to absorb more glucose from portal vein than what is present there! "
        //          << body->liver->excessGlucoseAbsorption << " " << body->liver->absorptionPerTick << endl;
        // }

        // timeStamp();
        // cout << " Liver absorbs glucose from portal vein " << body->liver->absorptionPerTick << "mg" << endl;

        // if (body->liver->lipogenesisOccurred)
        // {
        //     cout << "BodyWeight: Lipogenesis " << body->liver->preLipogenesis.bodyWeight
        //          << " glucose " << body->liver->preLipogenesis.toLipogenesis
        //          << " fat " << body->liver->preLipogenesis.fat
        //          << endl;

        //     cout << "BodyWeight: Lipogenesis " << body->liver->postLipogenesis.bodyWeight
        //          << " glucose " << body->liver->postLipogenesis.toLipogenesis
        //          << " fat " << body->liver->postLipogenesis.fat
        //          << endl;

        //     timeStamp();
        //     cout << " Liver:: Lipogenesis " << body->liver->postLipogenesis.toLipogenesis << endl;
        // }

        // cout << "After glycogen synthesis in liver, liver glycogen " << body->liver->postGlycogen.glycogen
        //      << " mg, live glucose " << body->liver->postGlycogen.glucose << " mg"
        //      << endl;

        // cout << "After GNG, liver glucose " << body->liver->postGluconeogensis.glucose << " mg, "
        //      << "liver glycogen " << body->liver->postGluconeogensis.glycogen << " mg, "
        //      << "blood glucose " << body->liver->postGluconeogensis.bloodGlucose << " mg, "
        //      << "blood lactate " << body->liver->postGluconeogensis.bloodLactate << " mg"
        //      << endl;

        // if (body->liver->maxGNGDuringExercise > 0)
        // {
        //     timeStamp();
        //     cout << " Liver:: Glucose Needed " << body->liver->glucoseNeeded
        //          << " maxGNG " << body->liver->maxGNGDuringExercise << endl;
        // }

        // if (body->liver->glucoseNeeded > 0)
        // {
        //     timeStamp();
        //     cout << " Liver:: ExtraGNG " << body->liver->glucoseNeeded
        //          << " " << body->liver->gngPerTick << endl;
        // }

        timeStamp();
        cout << " Liver:: ToGlycogen " << body->liver->toGlycogenPerTick << endl;
        timeStamp();
        cout << " Liver:: glycogen " << body->liver->glycogen / 1000.0 << endl;
        timeStamp();
        cout << " Liver:: FromGlycogen " << body->liver->fromGlycogenPerTick << endl;
        timeStamp();
        cout << " Liver:: GNG " << body->liver->gngPerTick << endl;
        // timeStamp();
        // cout << " Liver:: GlucoseProduced "
        //      << body->liver->gngPerTick + body->liver->fromGlycogenPerTick
        //      << endl;

        timeStamp();
        cout << " Liver:: Absorption " << body->liver->absorptionPerTick << endl;
        timeStamp();
        cout << " Liver:: Glycolysis " << body->liver->glycolysisPerTick << endl;
        timeStamp();
        cout << " Liver:: Release " << body->liver->releasePerTick  << "mg"
             << ", gl " << body->liver->glucose / body->liver->fluidVolume
             << endl;
    }

    void onMuscles()
    {
        // timeStamp();
        // cout << "HumanBody:: Current Energy Expenditure: " << body->muscles->currEnergyNeed << endl;

        if (body->muscles->hadExercise)
        {
            if (body->muscles->absorbedFromBlood > 0)    onRemoveGlucose(body->muscles->absorptionState);

            // timeStamp();
            // cout << " Muscles: GlycogenShare " << body->muscles->glycogenShare << endl;
        }
        else
        {
            onRemoveGlucose(body->muscles->basalBase);
            if (body->muscles->basalGLUT4Occurred)    onRemoveGlucose(body->muscles->basalGLUT4);
        }

        timeStamp();
        cout << " Muscles:: GlucoseAbsorbed " << body->muscles->glucoseAbsorbedPerTick << endl;

        timeStamp();
        cout << " Muscles:: GlycogenSynthesis " << body->muscles->glycogenSynthesizedPerTick << endl;
        timeStamp();
        cout << " Muscles:: GlycogenBreakdown " << body->muscles->glycogenBreakdownPerTick << endl;

        timeStamp();
        cout << " Muscles:: glycogen " << body->muscles->glycogen / 1000.0 << endl;
        timeStamp();
        cout << " Muscles:: Oxidation " << body->muscles->oxidationPerTick << endl;
        // timeStamp();
        // cout << " Muscles:: GlycogenOxidation " << body->muscles->glycogenOxidizedPerTick << endl;
        timeStamp();
        cout << " Muscles:: Glycolysis " << body->muscles->glycolysisPerTick << endl;

        // timeStamp();
        // cout << " Muscles:: TotalGlucoseOxidized "
        //      << body->muscles->oxidationPerTick + body->muscles->glycogenOxidizedPerTick
        //      << endl;

        // timeStamp();
        // cout << " Muscles:: totalGlucoseAbsorbed " << body->muscles->totalGlucoseAbsorbed << endl;
    }

    void onPortalVein()
    {
        // timeStamp();
        // cout << " PortalVein:: " << body->portalVein->glucose
        //      << " " << body->portalVein->glucose / body->portalVein->fluidVolume
        //      << " " << body->portalVein->branchedAA
        //      << " " << body->portalVein->unbranchedAA
        //      << endl;
    }

    void onStomach()
    {
        onAddFat(body->stomach->fatInBolus);

        timeStamp();
        cout << " Gastric Emptying:: Total Food " << body->stomach->totalFood
             << " Calorific Density " << body->stomach->calorificDensity
             << " geSlope " << body->stomach->geSlope
             << " ragInBolus " << body->stomach->ragInBolus
             << " sagInBolus " << body->stomach->sagInBolus
             << endl;

        if (body->stomach->stomachEmpty)
        {
            // cout << "STOMACH EMPTY " << body->bodyState << endl;

            // if (body->bodyState != body->oldState)
            // {
            //     timeStamp();
            //     cout << "Entering State " << body->bodyState << endl;
            // }

            timeStamp();
            cout << " Stomach:: SAG " << body->stomach->SAG
                 << " RAG " << body->stomach->RAG
                 << " protein " << body->stomach->protein
                 << " fat " << body->stomach->fat
                 << endl;
        }
    }

    void onFoodEvent(const FoodEvent& event)
    {
        // timeStamp();
        // cout << "Adding " << event.quantity << " grams of "
        //      << body->foodTypes[event.foodID].name << " to stomach"
        //      << endl;

        // if (body->bodyState != body->oldState)
        // {
        //     timeStamp();
        //     cout << "Entering State " << body->bodyState << endl;
        // }
    }

    void onExerciseEvent(const ExerciseEvent& event)
    {
        timeStamp();
        cout << " Starting Exercise at " << body->percentVO2Max << " %VO2Max" << endl;

        // if (body->bodyState != body->oldState)
        // {
        //     timeStamp();
        //     cout << "Entering State " << body->bodyState << endl;
        // }

        // timeStamp();
        // cout << "Firing Exercise Event " << event.exerciseID << " for " << event.duration << " minutes" << endl;
    }

    void onHaltEvent(const HaltEvent& event)
    {
        timeStamp();
        cout << " weight " << body->bodyWeight << endl;
    }
};


int main(int argc, char *argv[])
{
    const char* foodsFile          = argv[1];
    const char* exerciseFile       = argv[2];
    const char* metabolicRatesFile = argv[3];
    const char* eventsFile         = argv[4];
    const char* seedString         = argv[5];
    const char* outputFile         = argv[6];

    cout << std::fixed;
    cout << std::setprecision(3);

    if (argc != 7)
    {
        cout << "Syntax: carbmetsim foodsfile exercisefile metabolicratesfile eventsfile seedstring outputfile\n";
        exit(1);
    }

    // redirect stdout
    int fd = open(argv[6], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1)
    {
        cout << "Failed to open " << outputFile << endl;
        return 1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        cout << "Failed to redirect standard output" << endl;
        return 1;
    }
    if (close(fd) == -1)
    {
        cout << "Failed to close the file " << outputFile << endl;
        return 1;
    }

    CarbMetSimCLI cli{foodsFile, exerciseFile, metabolicRatesFile, eventsFile, seedString, outputFile};
    cli.run();

    return 0;
}

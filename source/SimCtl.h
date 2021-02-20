#pragma once

#include <string_view>
#include <memory>
#include <random>
#include <queue>
#include <vector>
#include "HumanBody.h"


// enum class EventType {FOOD, EXERCISE, HALT, METFORMIN, INSULIN_SHORT, INSULIN_LONG};
enum class EventType {FOOD, EXERCISE, HALT};

class Event
{
public:
    Event(unsigned fireTime, EventType type);
    virtual ~Event() = default;

    unsigned fireTime;
    EventType eventType;
};

// The priority queue holds shared_ptr<Event>, so we need a custom comparator.
struct EventGreater
{
    bool operator()(const std::shared_ptr<Event>& lhs, const std::shared_ptr<Event>& rhs);
};

class FoodEvent : public Event
{
public:
    FoodEvent(unsigned fireTime, unsigned quantity, unsigned foodID);

    unsigned quantity; // in grams
    unsigned foodID;
};

class ExerciseEvent : public Event
{
public:
    ExerciseEvent(unsigned fireTime, unsigned duration, unsigned exerciseID);

    unsigned duration;
    unsigned exerciseID;
};

class HaltEvent : public Event
{
public:
    HaltEvent(unsigned fireTime);
};


class SimCtl
{
public:
    SimCtl(std::string_view seedString,
           std::map<BodyState, MetabolicParams> metabolicParameters,
           std::map<unsigned, FoodType> foodTypes,
           std::map<unsigned, ExerciseType> exerciseTypes);

    void addEvent(unsigned fireTime, EventType type, unsigned id, unsigned howmuch);
    void addEvent(std::shared_ptr<Event> event);

    bool runTick();
    void runToHalt();
    
    bool eventsWereFired() const;
    std::vector<std::shared_ptr<Event>> eventsFiredThisTick() const;

    unsigned elapsedDays() const;
    unsigned elapsedHours() const;
    unsigned elapsedMinutes() const;
    unsigned ticks() const;
    static unsigned timeToTicks(unsigned days, unsigned hours, unsigned minutes);

    bool dayOver() const;

    // Interface to body and organ parameters
    const AdiposeTissue* adiposeTissue;
    const Blood*         blood;
    const HumanBody*     body;
    const Brain*         brain;
    const Heart*         heart;
    const Intestine*     intestine;
    const Kidneys*       kidneys;
    const Liver*         liver;
    const Muscles*       muscles;
    const PortalVein*    portalVein;
    const Stomach*       stomach;

private:
    friend class HumanBody;
    HumanBody humanBody;
    std::default_random_engine generator{1};

    unsigned tick = -1; // tick count advanced at the beginning of each tick, and we want to start at 0
    static const int TICKS_PER_DAY  = 24 * 60; // Simulated time granularity
    static const int TICKS_PER_HOUR = 60;      // Simulated time granularity

    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventGreater> eventQ;
    unsigned nextFireTime = -1; // should begin at largest number

    std::vector<std::shared_ptr<Event>> currentEvents;
    bool eventsFired    = false;
    bool haltEventFired = false;

    void updateNextFireTime();
    bool eventIsReady() const;
    std::shared_ptr<Event> getNextEvent();
    bool fireEvent();
};

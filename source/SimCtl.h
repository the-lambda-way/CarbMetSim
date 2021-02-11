#pragma once

#include <string>
#include <memory>
#include <random>
#include <queue>
#include <utility> // std::forward
#include <vector>

class HumanBody;


// enum class EventType {FOOD, EXERCISE, HALT, METFORMIN, INSULIN_SHORT, INSULIN_LONG};
enum class EventType {FOOD, EXERCISE, HALT};

class Event
{
public:
    Event(unsigned fireTime, EventType type);
    virtual ~Event();

    unsigned fireTime;
    EventType eventType;

    bool operator<(const Event& other);
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
    SimCtl(std::shared_ptr<HumanBody> body, std::string seedString);

    void addEvent(unsigned fireTime, EventType type, unsigned id, unsigned howmuch);
    void addEvent(std::shared_ptr<Event> event);

    bool runTick();
    void runToHalt();
    std::vector<std::shared_ptr<Event>> eventsFiredThisTick();

    unsigned elapsedDays();
    unsigned elapsedHours();
    unsigned elapsedMinutes();
    unsigned ticks();
    static unsigned timeToTicks(unsigned days, unsigned hours, unsigned minutes);

    bool dayOver();

private:
    friend class HumanBody;

    std::shared_ptr<HumanBody> body;
    std::default_random_engine generator{1};

    unsigned tick = 0;
    std::priority_queue<std::shared_ptr<Event>> eventQ;
    std::vector<std::shared_ptr<Event>> currentEvents;
    bool haltEventFired = false;

    static const int TICKS_PER_DAY  = 24 * 60; // Simulated time granularity
    static const int TICKS_PER_HOUR = 60;      // Simulated time granularity

    bool fireEvent();
    bool eventIsReady();
};

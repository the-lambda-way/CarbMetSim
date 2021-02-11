#include "SimCtl.h"

#include "HumanBody.h"

using namespace std;


bool Event::operator<(const Event& other) {
    return fireTime < other.fireTime;
}

Event::Event(unsigned fireTime, EventType type)
    : fireTime{fireTime}, eventType{type}
{
}

FoodEvent::FoodEvent(unsigned fireTime, unsigned quantity, unsigned foodID)
    : Event{fireTime, EventType::FOOD}, quantity{quantity}, foodID{foodID}
{
}

ExerciseEvent::ExerciseEvent(unsigned fireTime, unsigned duration, unsigned exerciseID)
    : Event{fireTime, EventType::EXERCISE}, duration{duration}, exerciseID{exerciseID}
{
}

HaltEvent::HaltEvent(unsigned fireTime)
    : Event{fireTime, EventType::HALT}
{
}

SimCtl::SimCtl(shared_ptr<HumanBody> body, string seedString)
    : body{move(body)}
{
    seed_seq seed{seedString.begin(), seedString.end()};
    generator.seed(seed);
}

void SimCtl::addEvent(unsigned fireTime, EventType type, unsigned id, unsigned howmuch)
{
    switch (type)
    {
        case EventType::FOOD:
            eventQ.emplace(new FoodEvent{fireTime, howmuch, id});
            break;
        case EventType::EXERCISE:
            eventQ.emplace(new ExerciseEvent{fireTime, howmuch, id});
            break;
        case EventType::HALT:
            eventQ.emplace(new HaltEvent{fireTime});
            break;
        default:
            break;
    }
}

void SimCtl::addEvent(shared_ptr<Event> event)
{
    eventQ.push(move(event));
}

bool SimCtl::runTick()
{
    while (fireEvent());
    if (haltEventFired)    return false;

    body->processTick();
    ++tick;

    return true;
}

void SimCtl::runToHalt()
{
    while (runTick());
}

vector<shared_ptr<Event>> SimCtl::eventsFiredThisTick()
{
    return currentEvents;
}

unsigned SimCtl::elapsedDays()
{
    return tick / TICKS_PER_DAY;
}

unsigned SimCtl::elapsedHours()
{
    int x = tick % TICKS_PER_DAY;
    return x / TICKS_PER_HOUR;
}

unsigned SimCtl::elapsedMinutes()
{
    int x = tick % TICKS_PER_DAY;
    return x % TICKS_PER_HOUR;
}

unsigned SimCtl::ticks()
{
    return tick;
}

unsigned SimCtl::timeToTicks(unsigned days, unsigned hours, unsigned minutes)
{
    return days * TICKS_PER_DAY + hours * TICKS_PER_HOUR + minutes;
}

bool SimCtl::dayOver()
{
    return tick % TICKS_PER_DAY == 0 && tick != 0;
}

bool SimCtl::fireEvent()
{
    if (!eventIsReady())    return false;

    shared_ptr<Event> event = eventQ.top();
    currentEvents.push_back(event);
    eventQ.pop();

    switch (event->eventType)
    {
        case EventType::FOOD:
        {
            shared_ptr<FoodEvent> food = dynamic_pointer_cast<FoodEvent>(event);
            body->processFoodEvent(food->foodID, food->quantity);
            break;
        }
        case EventType::EXERCISE:
        {
            shared_ptr<ExerciseEvent> exercise = dynamic_pointer_cast<ExerciseEvent>(event);
            body->processExerciseEvent(exercise->exerciseID, exercise->duration);
            break;
        }
        case EventType::HALT:
            haltEventFired = true;
            return false;
        default:
            break;
    }

    return true;
}

bool SimCtl::eventIsReady()
{
    return !eventQ.empty() && eventQ.top()->fireTime <= tick;
}

#include "SimCtl.h"
#include "HumanBody.h"

using namespace std;


bool EventGreater::operator()(const std::shared_ptr<Event>& lhs, const std::shared_ptr<Event>& rhs)
{
    // Should return true if lhs comes before rhs, where the largest element becomes the top of the queue.
    // We want the smallest fireTime to be on top, therefore we invert the order. On a tie order of insertion should be
    // maintained, so we do not want to compare equality.
    return lhs->fireTime > rhs->fireTime;
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

SimCtl::SimCtl(std::string_view seedString,
               std::map<BodyState, MetabolicParams> metabolicParameters,
               std::map<unsigned, FoodType> foodTypes,
               std::map<unsigned, ExerciseType> exerciseTypes)
    : humanBody{this, move(metabolicParameters), move(foodTypes), move(exerciseTypes)}
{
    body          = &humanBody;
    adiposeTissue = &humanBody.adiposeTissue;
    blood         = &humanBody.blood;
    brain         = &humanBody.brain;
    liver         = &humanBody.liver;
    heart         = &humanBody.heart;
    intestine     = &humanBody.intestine;
    kidneys       = &humanBody.kidneys;
    muscles       = &humanBody.muscles;
    portalVein    = &humanBody.portalVein;
    stomach       = &humanBody.stomach;

    seed_seq seed(seedString.begin(), seedString.end());
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

    updateNextFireTime();
}

void SimCtl::addEvent(shared_ptr<Event> event)
{
    eventQ.push(move(event));
    updateNextFireTime();
}

bool SimCtl::runTick()
{
    // Increment the tick first so that after each runTick(), tick() will return what the organs just saw
    ++tick;

    currentEvents.clear();
    eventsFired = false;

    while (fireEvent());
    if (haltEventFired)    return false;

    humanBody.processTick();

    return true;
}

void SimCtl::runToHalt()
{
    while (runTick());
}

bool SimCtl::eventsWereFired() const
{
    return eventsFired;
}

vector<shared_ptr<Event>> SimCtl::eventsFiredThisTick() const
{
    return currentEvents;
}

unsigned SimCtl::elapsedDays() const
{
    return tick / TICKS_PER_DAY;
}

unsigned SimCtl::elapsedHours() const
{
    int x = tick % TICKS_PER_DAY;
    return x / TICKS_PER_HOUR;
}

unsigned SimCtl::elapsedMinutes() const
{
    int x = tick % TICKS_PER_DAY;
    return x % TICKS_PER_HOUR;
}

unsigned SimCtl::ticks() const
{
    return tick;
}

unsigned SimCtl::timeToTicks(unsigned days, unsigned hours, unsigned minutes)
{
    return days * TICKS_PER_DAY + hours * TICKS_PER_HOUR + minutes;
}

bool SimCtl::dayOver() const
{
    return tick % TICKS_PER_DAY == 0 && tick != 0;
}

bool SimCtl::fireEvent()
{
    if (!eventIsReady())    return false;
    eventsFired = true;

    shared_ptr<Event> event = getNextEvent();
    currentEvents.push_back(event);

    switch (event->eventType)
    {
        case EventType::FOOD:
        {
            shared_ptr<FoodEvent> food = dynamic_pointer_cast<FoodEvent>(event);
            humanBody.processFoodEvent(food->foodID, food->quantity);
            break;
        }
        case EventType::EXERCISE:
        {
            shared_ptr<ExerciseEvent> exercise = dynamic_pointer_cast<ExerciseEvent>(event);
            humanBody.processExerciseEvent(exercise->exerciseID, exercise->duration);
            break;
        }
        case EventType::HALT:
            haltEventFired = true;
            return false;
    }

    return true;
}

shared_ptr<Event> SimCtl::getNextEvent()
{
    shared_ptr<Event> out = eventQ.top();
    eventQ.pop();

    updateNextFireTime();

    return out;
}

void SimCtl::updateNextFireTime()
{
    nextFireTime = eventQ.top()->fireTime;
}

bool SimCtl::eventIsReady() const
{
    return nextFireTime <= tick;
}

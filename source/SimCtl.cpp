#include "SimCtl.h"
#include <algorithm>

using namespace std;


unsigned EventFireTime::operator()(const std::shared_ptr<Event>& event) const
{
    return event->fireTime;
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
            queue.emplace(new FoodEvent{fireTime, howmuch, id});
            break;
        case EventType::EXERCISE:
            queue.emplace(new ExerciseEvent{fireTime, howmuch, id});
            break;
        case EventType::HALT:
            queue.emplace(new HaltEvent{fireTime});
            break;
        default:
            break;
    }
}

void SimCtl::addEvent(shared_ptr<Event> event)
{
    queue.push(move(event));
}

bool SimCtl::runTick()
{
    queue.advance();

    if (queue.events_were_fired())
    {
        handleEvents();
        if (haltEventFired)    return false;
    }

    humanBody.processTick();

    return true;
}

void SimCtl::runToHalt()
{
    while (runTick());
}

bool SimCtl::eventsWereFired() const
{
    return queue.events_were_fired();
}

vector<shared_ptr<Event>> SimCtl::getFiredEvents() const
{
    return queue.get_fired_events();
}

unsigned SimCtl::elapsedDays() const
{
    return queue.ticks() / TICKS_PER_DAY;
}

unsigned SimCtl::elapsedHours() const
{
    int x = queue.ticks() % TICKS_PER_DAY;
    return x / TICKS_PER_HOUR;
}

unsigned SimCtl::elapsedMinutes() const
{
    int x = queue.ticks() % TICKS_PER_DAY;
    return x % TICKS_PER_HOUR;
}

unsigned SimCtl::ticks() const
{
    return queue.ticks();
}

unsigned SimCtl::timeToTicks(unsigned days, unsigned hours, unsigned minutes)
{
    return days * TICKS_PER_DAY + hours * TICKS_PER_HOUR + minutes;
}

bool SimCtl::dayOver() const
{
    return queue.ticks() % TICKS_PER_DAY == 0 && queue.ticks() != 0;
}

void SimCtl::handleEvents()
{
    for (const auto& event : queue.get_fired_events())
    {
        switch (event->eventType)
        {
            case EventType::FOOD:
            {
                auto food = dynamic_pointer_cast<FoodEvent>(event);
                humanBody.processFoodEvent(food->foodID, food->quantity);
                break;
            }
            case EventType::EXERCISE:
            {
                auto exercise = dynamic_pointer_cast<ExerciseEvent>(event);
                humanBody.processExerciseEvent(exercise->exerciseID, exercise->duration);
                break;
            }
            case EventType::HALT:
                haltEventFired = true;
                return;
        }
    }
}

#include "catch2/catch.h"

#include "discrete_event_queue.h"
#include <algorithm>  // std::find
#include <functional> // std::less
#include <memory>     // std::allocator
#include <vector>


#include <iostream>

struct identity
{
    template<class T>
    constexpr T&& operator()(T&& t) const noexcept
    {
        return std::forward<T>(t);
    }
};

SCENARIO("All the same constructors as std::priority_queue.")
{
    GIVEN("a type, a container, a projection, and an allocator")
    {
        using type       = unsigned;
        using container  = std::vector<type>;
        using allocator  = std::allocator<typename container::value_type>;

        container  cont;
        auto       proj = identity{};
        allocator  alloc;

        discrete_event_queue<type, identity> other_event_queue;

        std::vector<type> some_values {type{}, type{}, type{}};
        auto first = some_values.begin();
        auto last  = some_values.end();

        THEN("an event queue can be constructed the same way as every constructor of std::priority_queue.")
        {
            discrete_event_queue<type, identity>{};
            discrete_event_queue<type, identity, container>{proj};
            discrete_event_queue<type, identity>{proj};
            discrete_event_queue{proj, cont};
            discrete_event_queue{proj, container{}};
            discrete_event_queue{other_event_queue};
            discrete_event_queue{discrete_event_queue<type, identity>{}};
            discrete_event_queue<type, identity>{alloc};
            discrete_event_queue<type, identity, container>{alloc};
            discrete_event_queue<type, identity>{proj, alloc};
            discrete_event_queue<type, identity, container>{proj, alloc};
            discrete_event_queue{proj, cont, alloc};
            discrete_event_queue{proj, container{}, alloc};
            discrete_event_queue{other_event_queue, alloc};
            discrete_event_queue{discrete_event_queue<type, identity>{}, alloc};
            discrete_event_queue{first, last, proj, cont};
            discrete_event_queue{first, last, proj, container{}};
            discrete_event_queue{first, last, proj};
            discrete_event_queue<type, identity, container>{first, last, proj};
            discrete_event_queue<type, identity>{first, last};
            discrete_event_queue<type, identity, container>{first, last};
            discrete_event_queue copied_event_queue = other_event_queue;
        }
    }
}

SCENARIO("Measure and advance ticks.")
{
    GIVEN("an event queue")
    {
        discrete_event_queue<unsigned, identity> queue;

        WHEN("the queue has not advanced")
        {
            THEN("the tick count is unsigned(-1).")
            {
                REQUIRE(queue.ticks() == unsigned(-1));
            }
        }

        WHEN("the queue has advanced once")
        {
            queue.advance();

            THEN("the tick count is 0.")
            {
                REQUIRE(queue.ticks() == 0);
            }
        }

        WHEN("the queue has advanced by many ticks")
        {
            queue.advance();
            queue.advance(5);

            THEN("the tick count is increased by that amount.")
            {
                REQUIRE(queue.ticks() == 5);
            }
        }
    }
}

SCENARIO("Reset ticks.")
{
    GIVEN("A queue that has advanced some ticks")
    {
        discrete_event_queue<unsigned, identity> queue;
        queue.advance();

        WHEN("the queue is reset")
        {
            queue.reset_ticks();

            THEN("the tick count is unsigned(-1).")
            {
                REQUIRE(queue.ticks() == unsigned(-1));
            }
        }
    }
}

SCENARIO("Add and emplace events.")
{
    GIVEN("an event queue and an event")
    {
        discrete_event_queue<unsigned, identity> queue;
        unsigned event = 0;

        THEN("both an lvalue and an rvalue can be pushed to the queue.")
        {
            queue.push(event);
            queue.push(0);
        }
    }

    GIVEN("an event queue and an object")
    {
        struct Event
        {
            unsigned tick;

            // cannot implicitly construct this since the class gets transformed into a (complex) catch2 object
            Event(unsigned t) : tick{t} {}
        };

        discrete_event_queue<Event, decltype(&Event::tick)> queue{&Event::tick};

        THEN("an object can be emplaced into the queue.")
        {
            queue.emplace(0);
        }
    }
}

SCENARIO("Inspect the size of the queue.")
{
    GIVEN("an event queue")
    {
        discrete_event_queue<unsigned, identity> queue;

        WHEN("the queue is empty")
        {
            THEN("the empty function should return true and the size function should return zero.")
            {
                REQUIRE(queue.empty());
                REQUIRE(queue.size() == 0);
            }
        }

        WHEN("the queue is not empty")
        {
            queue.push(1);
            queue.push(2);

            THEN("the empty function should return false and the size function should return the number of events.")
            {
                REQUIRE(!queue.empty());
                REQUIRE(queue.size() == 2);
            }
        }
    }
}

SCENARIO("Retrieve a sorted container of events.")
{
    GIVEN("an event queue with events")
    {
        std::vector<unsigned> vals{3, 2, 1};
        discrete_event_queue queue{identity{}, vals};

        THEN("get_events should return a container of events sorted by fire time.")
        {
            REQUIRE(queue.get_events() == std::vector<unsigned>{1, 2, 3});
        }
    }
}

SCENARIO("Remove events.")
{
    GIVEN("an event queue with events")
    {
        discrete_event_queue<unsigned, identity> queue1;
        queue1.push(0);
        queue1.push(1);

        discrete_event_queue<unsigned, identity> queue2;
        queue2.push(0);
        queue2.push(1);

        THEN("an event can be removed based on value equality.")
        {
            queue1.erase(1);
        }

        THEN("an event can be removed based on a condition.")
        {
            queue2.erase_if([](const unsigned& v) { return v == 1; });
        }

        WHEN("one or more equal events exist in the queue and are removed")
        {
            queue1.push(2);
            queue1.push(2);
            auto count1 = queue1.erase(2);

            queue2.push(2);
            queue2.push(2);
            auto count2 = queue2.erase_if([](const unsigned& v) { return v == 2; });

            THEN("the function returns the number of items removed.")
            {
                REQUIRE(count1 == 2);
                REQUIRE(count2 == 2);
            }

            THEN("the container should no longer contain any event equal to the one removed.")
            {
                auto events1 = queue1.get_events();
                REQUIRE(std::find(events1.begin(), events1.end(), 2) == events1.end());

                auto events2 = queue2.get_events();
                REQUIRE(std::find(events2.begin(), events2.end(), 2) == events2.end());
            }
        }

        WHEN("an event does not exist in the queue and is asked to be removed")
        {
            auto size1  = queue1.size();
            auto count1 = queue1.erase(3);

            auto size2  = queue2.size();
            auto count2 = queue2.erase_if([](const unsigned& v) { return v == 3; });

            THEN("the function returns zero.")
            {
                REQUIRE(count1 == 0);
                REQUIRE(count2 == 0);
            }

            THEN("no events are removed from the container.")
            {
                REQUIRE(queue1.size() == size1);
                REQUIRE(queue2.size() == size2);
            }
        }
    }
}

SCENARIO("Clear all events.")
{
    GIVEN("A queue with some elements")
    {
        std::vector<unsigned> vals{3, 4, 5};
        discrete_event_queue queue{identity{}, vals};

        queue.advance();
        unsigned current_ticks = queue.ticks();

        WHEN("the queue is cleared")
        {
            queue.clear();

            THEN("the queue is empty.")
            {
                REQUIRE(queue.empty());
            }

            THEN("the ticks have not changed.")
            {
                REQUIRE(queue.ticks() == current_ticks);
            }
        }
    }
}

SCENARIO("Fire events.")
{
    GIVEN("an event queue with events")
    {
        std::vector<unsigned> vals{10, 10, 15, 20};
        discrete_event_queue queue{identity{}, vals};

        WHEN("advance is called until the tick equals the tick count of the front event")
        {
            while (queue.ticks() != vals.front())    queue.advance();

            THEN("all the events equal to that tick are removed from the queue.")
            {
                REQUIRE(queue.get_events() == std::vector<unsigned>{15, 20});
            }

            THEN("get_fired_events should return which events were just fired.")
            {
                REQUIRE(queue.get_fired_events() == std::vector<unsigned>{10, 10});
            }

            THEN("events_were_fired should return true.")
            {
                REQUIRE(queue.events_were_fired());
            }
        }

        WHEN("advance is called past an event and no more events are ready to fire")
        {
            while (queue.ticks() != vals.front())    queue.advance();
            queue.advance();

            THEN("no events should be removed from the queue.")
            {
                REQUIRE(queue.get_events() == std::vector<unsigned>{15, 20});
            }

            THEN("get_fired_events should return an empty container.")
            {
                REQUIRE(queue.get_fired_events().empty());
            }

            THEN("events_were_fired should return false.")
            {
                REQUIRE(!queue.events_were_fired());
            }
        }

        WHEN("advance(unsigned) advances the tick to equal the tick count of the front event")
        {
            queue.advance();
            queue.advance(10);

            THEN("all the events equal to that tick are removed from the queue.")
            {
                REQUIRE(queue.get_events() == std::vector<unsigned>{15, 20});
            }

            THEN("get_fired_events should return which events were just fired.")
            {
                REQUIRE(queue.get_fired_events() == std::vector<unsigned>{10, 10});
            }

            THEN("events_were_fired should return true.")
            {
                REQUIRE(queue.events_were_fired());
            }
        }

        WHEN("advance(unsigned) advances the tick to greater than the tick count of the front event")
        {
            queue.advance();
            queue.advance(11);

            THEN("all the events less than that tick are removed from the queue.")
            {
                REQUIRE(queue.get_events() == std::vector<unsigned>{15, 20});
            }

            THEN("events_fired_this_tick should return which events were just fired.")
            {
                REQUIRE(queue.get_fired_events() == std::vector<unsigned>{10, 10});
            }

            THEN("events_were_fired should return true.")
            {
                REQUIRE(queue.events_were_fired());
            }
        }

        WHEN("advance_to_next_event is called")
        {
            queue.advance_to_next_event();

            THEN("all the events equal to the front event should be removed from the queue.")
            {
                REQUIRE(queue.get_events() == std::vector<unsigned>{15, 20});
            }

            THEN("get_fired_events should return which events were just fired.")
            {
                REQUIRE(queue.get_fired_events() == std::vector<unsigned>{10, 10});
            }

            THEN("events_were_fired should return true.")
            {
                REQUIRE(queue.events_were_fired());
            }

            THEN("the tick count should compare equal the tick count of the front event just removed.")
            {
                REQUIRE(queue.ticks() == queue.get_fired_events().front());
            }
        }
    }
}

SCENARIO("Swap two queues.")
{
    GIVEN("Two queues with elements")
    {
        std::vector<unsigned> a_vals{0, 1, 2};
        std::vector<unsigned> b_vals{3, 4, 5};
        discrete_event_queue a{identity{}, a_vals};
        discrete_event_queue b{identity{}, b_vals};

        auto a_elems = a.get_events();
        auto b_elems = b.get_events();
        REQUIRE(a_elems != b_elems);

        b.advance();
        unsigned a_ticks = a.ticks();
        unsigned b_ticks = b.ticks();
        REQUIRE(a_ticks != b_ticks);

        WHEN("the contents are swapped")
        {
            a.swap(b);

            THEN("the elements of each queue have been swapped.")
            {
                REQUIRE(a.get_events() == b_elems);
                REQUIRE(b.get_events() == a_elems);
            }

            THEN("the ticks of each queue have been swapped.")
            {
                REQUIRE(a.ticks() == b_ticks);
                REQUIRE(b.ticks() == a_ticks);
            }
        }
    }
}
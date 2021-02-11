# Issues


## Examples

In all the parameter files in the examples, there is a parameter KIDNEYS. The file HumanBody.cpp processes the token KIDNEY with no S, so the KIDNEYS parameter is never processed.


## System Testing

The file tests/system/meal-event-normal.test.cpp is unable to compile because it relies on the definitions in SimCtl.cpp, which already contains a main function. For integration within larger programs, the simulation could be designed as a library that can be imported.

In tests/system/meal-event-normal.test.cpp, in theory, the call to exit(0) in Simtl::fire_event should prematurely exit the test, but due to the bug below, I haven't analyzed that issue yet.

The file tests/system/meal-event-normal.test.cpp produces a segmentation fault for unknown reasons, but contained somewhere in the call to the SimCtl::run_simulation function. This might be resolved implicitly during a future refactor, so I've abandoned debugging the issue for now.


## Integration Testing

Consider the integration test below.

```c++
#include "catch2/catch.h"
#include "AdiposeTissue.h"

// Stub HumanBody class
class HumanBody
{
public:
    double bodyWeight = 65.0;
    double fatFraction_ = 0.2;
    AdiposeTissue* adiposeTissue;

    HumanBody()
    {
        adiposeTissue = new AdiposeTissue(this);
        adiposeTissue->fat = fatFraction_*bodyWeight*1000.0;
    }

    ~HumanBody()
    {
        delete adiposeTissue;
    }

}; // class HumanBody


SCENARIO("Fat can be directly added to adipose tissue.")
{
    GIVEN("A human body with some adipose tissue")
    {
        HumanBody body{};
        double old_weight = body.bodyWeight;

        WHEN("a gram of fat is directly added")
        {
            body.adiposeTissue->addFat(1000.0);

            THEN("the body weight is increased by a gram.")
            {
                REQUIRE(body.bodyWeight == old_weight + 0.001);
            }
        }
    }
}
```

In a normal integration test, if two components are coupled, you can create a mock for one of them so that the other can be tested under controlled conditions. In this case, we want to mock HumanBody to test AdiposeTissue. Typically, a mock is created as a subclass of an interface (abstract class), so that it can be substituted wherever the original is expected. Since there is no interface for HumanBody, instead we can create a stub, a class with the minimal features necessary for testing.

However, AdiposeTissue.cpp imports the original HumanBody class, so the stub definition is never seen (the compiler finds the declaration from HumanBody.h). This leads to a test failure without a compiler error (debugging reveals the bug). This forces us to test AdiposeTissue in the full simulation environment, which is a system test. Due to this tight coupling, we are unable to effectively (at least, easily) test AdiposeTissue under controlled conditions.

One solution to this issue is to add an interface for HumanBody to eliminate the dependency on a concrete type. Another solution is to separate the logic outside of AdiposeTissue, which would allow AdiposeTissue itself to be mocked.


## Unit Testing

Unit tests are only meaningful for public members that don't interact with other parts of the program. In the AdiposeTissue class, for instance, there is nothing to unit test because none of its functions are public. It is worth considering designing features which can be unit tested if future expansion is a possibility.


## Testing Automation

Continuous integration would be helpful for future expansion of the simulation.


## Documentation

Documentation currently comes straight from the paper *CarbMetSim: A discrete-event simulator for
carbohydrate metabolism in humans*. Literature references were removed from the documentation in anticipation that the material will be simplified in the future.


## Encapsulation

Encapsulation needs to be improved across the board. Parameters should be accessed through public interfaces instead of variables or friendship. Some parameters in HumanBody are only used in a single organ, these should maybe be moved into those organs. More algorithms can be abstracted and put into "utility.h".


## Changelog
Refactored to be a library that can be integrated into other projects. Since this touches every part of the code base, I took the opportunity to do a overall refactoring as well.

* Created an independent command-line interface and moved all input and output to there.
* Replaced all output messages with diagnostic variables. Moved all the messages into the cli.
* Replaced all input strings with input parameters. Moved all the strings into the cli.
* Removed the main loop from SimCtl. The containing program runs its own main loop. On each iteration, it calls SimCtl::runTick(), then reads which events occurred as well as whatever parameters it needs. SimCtl::runTick() returns false after the HALT event has fired.
* Moved the global variables into SimCtl. As a result, multiple simulators can exist side-by-side.
* Moved externally accessed variables from private to public (encapsulation will need to be improved in the future). Moved variables only internally used from public to private.
* Changed programmer-related error checks into assertions.
* Simplified many things, including moving subprocesses into their own functions.
* There was inconsistent code formatting and style, so I picked my own and applied it everywhere.


## TODO BEFORE MERGING
* Change runtime error strategy, the sim should not exit the program.
* Get system tests to work.

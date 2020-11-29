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

Unit tests are only meaningful for public members that don't interact with other parts of the program. In the AdiposeTissue class, for instance, there is nothing to unit test because none of its state is public. It is worth considering designing features which can be unit tested if future expansion is a possibility.


## Testing Automation

Continuous integration would be helpful for future expansion of the simulation.

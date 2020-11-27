# Issues

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

        WHEN("fat is directly added")
        {
            THEN("the body weight is increased by the same amount.")
            {
                double old_weight = body.bodyWeight;

                SECTION("0 mg is added")
                {
                    body.adiposeTissue->addFat(0.0);
                    REQUIRE(body.bodyWeight == old_weight);
                }

                SECTION("1000 mg is added")
                {
                    body.adiposeTissue->addFat(1000.0);
                    REQUIRE(body.bodyWeight == old_weight + 0.001);
                }
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

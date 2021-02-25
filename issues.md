# Issues


## Unit Testing

Unit tests are only meaningful for public members that don't cause side effects to other parts of the program. It is worth considering designing features which can be unit tested if future expansion is a possibility.


## Testing Automation

Continuous integration would be helpful for future expansion of the simulation.


## Documentation

The documentation is mostly incomplete and is often copied straight from the paper *CarbMetSim: A discrete-event simulator for carbohydrate metabolism in humans*. Should be edited to an appropriate format for general consumption, as well as adding source code documentation.


## Encapsulation

Encapsulation needs to be improved across the board. Parameters should be accessed through public interfaces instead of variables or friendship. Some parameters in HumanBody are only used in a single organ, these should maybe be moved into those organs. More algorithms might be encapsulated and put into "common.h".


## Body State Parameters

I noticed that changing the metabolic parameters through HumanBody::setParams() is only done once and is commented out elsewhere, so the sim does not currently support different parameters for different body states. Should be able to just uncomment these? Were they commented out because they haven't been experimentally validated yet?


## Negative Zero

The output of stomach sometimes contains -0.000. This should be considered a bug since real metabolic quantities cannot be negative. Negative underflow is acceptable when caused by floating point imprecision, so this bug can be fixed by checking for some negative epsilon (any larger negatives that occur would be a different bug). In the meantime I've adjusted the output of the command line interface to cover up any negative underflow, but this needs to be fixed in the simulation proper. I only added the check to the stomach which was the only place the bug manifested, but it might be a concern elsewhere.
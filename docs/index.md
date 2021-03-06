# Introduction

CarbMetSim (**Carb**ohydrate **Met**abolism **Sim**ulator) is a discrete-event simulator that predicts minute by minute Blood Glucose Level (BGL) in response to a sequence of food and exercise events. It implements broader aspects of carbohydrate metabolism in human beings with the objective of capturing the average impact of various diet/exercise activities on the BGL of people with different levels of diabetes. It can be used as a stand-alone command-line application or integrated into a larger project as a library.

CarbMetSim implements key organs to the extent necessary to capture their impact on the production and consumption of glucose. Key metabolic pathways are accounted for in the operation of the various organs. The impact of insulin and insulin resistance on the operation of various organs/pathways is captured in accordance with published research. CarbMetSim provides broad flexibility to configure the insulin production ability, the average flux along various metabolic pathways and the impact of insulin resistance on different aspects of carbohydrate metabolism. Thus, it is possible to customize the simulator for a particular user by setting appropriate values to various configurable parameters.



# Getting Started

To build the command line tool, run `make` at the root folder. Run with `carbmetsim foodsfile exercisefile paramsfile eventsfile seed outputfile`. See [Running on the command line](Running on the command line.md) for an explanation of each argument.

To use as a library, copy the contents of the `source/` folder into your code base and include *SimCtl.h* in one of your source files. Instantiate an instance of `SimCtl`  with metabolic parameters, food types, and exercise types, then call `SimCtl::runTick()` on the object in a loop until it returns false. An explanation of the parameters can be found in the documentation for [SimCtl](Simulator.h).



# Contents

[Running on the command line](Running on the command line.md)

[Parameters](Parameters.md)



## Examples

Examples



## Simulator

[Simulator](Simulator.md)

[Human body](Human Body.md)



## Organs

Adipose tissue

Blood

Brain

Heart

Intestine

Kidneys

Liver

Muscles

Portal vein

Stomach



## Events

[Overview](Events.md)

[Food](Food.md)

[Exercise](Exercise.md)



## Metabolic Processes

[Gluconeogenesis](Gluconeogenesis.md)

[Glucose transport](Glucose Transport.md)

[Glycogen synthesis and breakdown](Glycogen Synthesis and Breakdown.md)

[Insuling](Insuling.md)



# Future Directions

Protein metabolism is implemented in a very simplified manner.

Lipid metabolism is implemented in a very simplified manner.

The simulator does not yet consider monosaccharides other than glucose and assumes that all dietary carbohydrate gets converted to glucose after digestion.

The impact of insulin is captured in a simplified manner and other important hormones (e.g. glucagon) are not yet directly modeled.

In CarbMetSim’s current implementation, the insulinLevel variable is tightly coupled with the BGL. A future implementation will allow the insulinLevel to vary in a configurable manner by allowing the user to specify the dosage of externally injected short term insulin. This would allow the simulator to be useful for diabetes patients dependent on short term insulin injections or insulin pumps.

The non-starch polysaccharide (also known as dietary fiber) part of the carbohydrates is currently ignored (even though the fiber contents of the food are known to have an impact on the gastric emptying).

CarbMetSim currently does not characterize protein in terms of its amino acid contents. Since only 3 of the 20 amino acids have branched chains, a general assumption is made that 85% of amino acids resulting from protein digestion have unbranched chains and the remaining have branched chains.

The simulator is not yet capable of translating a user’s diet/exercise/BGL data into the values of simulation parameters governing the behavior of different organs.

Significant additional validation against continuous blood glucose data of individuals representing different demographic groups, different types & levels of Diabetes and different lifestyles is needed before the simulator may be considered ready for use in diabetes self-management applications or for research.

The nutritional composition of leaked chyme is same as that of chyme present in the stomach. This simple model allows us to take in account the fat/protein induced slowdown of gastric emptying. There are many other factors that affect the gastric emptying process (the solid/liquid nature of food, fiber content, osmolarity, viscosity etc.) which CarbMetSim currently does not take in account. Thus, a bolus of chyme leaks from the Stomach into the Intestine every tick (i.e. every minute) until the Stomach is empty.

In CarbMetSim, the skeletal muscles are implemented as the Muscles object. Currently, the simulator implements response to the resting condition and the aerobic exercise only. Specifically, exercise with a significant anaerobic component cannot yet be simulated. Also, it is not yet possible to distinguish among different muscle groups.


The simulator currently does not support increase in gluconeogenesis flux due to increased availability of substrates.

The paper also presented a preliminary validation of CarbMetSim’s behavior in response to single meal and exercise events where the simulator’s predictions were compared against the group-level averages reported in the published literature. The simulator needs significant additional validation against continuous blood glucose data of individuals representing different demographic groups, different types & levels of Diabetes and different lifestyles before it may be considered ready for use in diabetes self-management and research-related applications.


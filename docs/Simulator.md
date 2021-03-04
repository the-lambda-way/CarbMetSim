# Simulator

## Design

CarbMetSim is implemented in an object-oriented manner. At the top level, CarbMetSim consists of a SimCtl (**SIM**ulation **C**on**T**ro**L**ler) object and a HumanBody object. The SimCtl object maintains the simulation time (in ticks, where each tick is a minute) and contains a priority queue of food/exercise events sorted in order of their firing times. At the beginning of the simulation, the SimCtl object reads all the food/exercise events into the priority queue. At each tick, the SimCtl object fires the events whose firing time has arrived (by invoking appropriate methods on the HumanBody object) and then causes each organ to do its work during that tick (again by invoking a HumanBody object method).

Default values of various parameters used by the components of the simulator were determined experimentally to provide a close match with published measurements performed on non-diabetic human subjects before and after a meal event. Table 1 in the [Glycolysis](Glycolysis.md) section shows the default values for glycolysis related parameters for different organs.

Default values of configurable parameters that determine the impact of insulinLevel on various metabolic processes are shown in Table 1 of the [HumanBody](Human-body.md) section.



## Theory





## Implementation


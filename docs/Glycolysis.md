# Glycolysis

Glucose serves as a key source of energy for various tissues, which either oxidize it completely or consume it anaerobically via glycolysis. Complete oxidation of glucose yields 15 times more energy than anaerobic glycolysis but can only be done if oxygen is available. Tissues with access to plenty of oxygen oxidize glucose for their energy needs whereas others (possibly in the same organ) use glycolysis. Glycolysis results in the generation of lactate, which serves as a key substrate for endogenous glucose production via gluconeogenesis (described later).

The following organs in CarbMetSim use anaerobic glycolysis as an energy source: Muscles, Liver, Kidneys, Intestine and Blood. The amount of glucose consumed for glycolysis increases with the glucose availability, which is signaled by the insulin level in the bloodstream. This is modeled in the simulator in the following manner. Each organ using glycolysis as an energy source has two configurable parameters: glycolysisMin and glycolysisMax (in units of mg of glucose consumed per kg of body weight per minute). At each tick, the organ generates a poisson distributed random number (min) with glycolysisMin as the mean value and glycolysisMax as the maximum value. Then, subject to the glucose availability in the organ, the amount of glucose consumed in a tick for glycolysis is given by: min + insulinImpact × (glycolysisMax - min). Here, insulinImpact is a factor (between 0 and 1) that increases in value with increase in the insulinLevel. This factor is calculated using a sigmoid function, which is currently the CDF of a normal distribution with a configurable mean and standard deviation. The simulator also uses configurable multiplicative parameters glycolysisMinImpact and glycolysisMaxImpact (with default values 1.0) to modify the values of glycolysisMin and glycolysisMax parameters associated with each organ. These parameters can be used to model the impact of diabetes on glycolysis flux. A fraction (by default 1) of the glucose consumed for glycolysis is converted to lactate, which is added to the Blood object. Table 1 shows the default values for glycolysis related parameters for different organs. The default values of various configurable parameters in CarbMetSim were determined experimentally to provide a close match with published measurements performed on non-diabetic human subjects before and after a meal event.

**Table 1. The default values for glycolysis related parameters in various organs.**

| Organ     | glycolysisMin (mg/kg/minute) | glycolysisMax (mg/kg/minute) |
| --------- | ---------------------------- | ---------------------------- |
| Blood     | 0.0315                       | 0.1135                       |
| Kidneys   | 0.0315                       | 0.1135                       |
| Liver     | 0.0630                       | 0.5675                       |
| Muscles   | 0.0630                       | 0.8512                       |
| Intestine | 0.0315                       | 0.1135                       |




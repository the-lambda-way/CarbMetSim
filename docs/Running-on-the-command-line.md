## Running on the command line

Run the simulator from the command line in the following manner:

`carbmetsim foodsfile exercisefile paramsfile eventsfile seed outputfile`



### foodsfile

The *foodsfile* contains the description of different food items. Each line in this file describes a different food using the following format:

`id food-description serving-size-in-grams RAG-per-serving SAG-per-serving protein-per-serving fat-per-serving`

**id:** An integer used to identify the food item in *eventsfile*.

**food-description:** A text string (containing no spaces) that describes the food item.

**serving-size-in-grams:** The size in grams of one serving of this food item.

**RAG-per-serving:** The amount in grams of *Rapidly Available Glucose* (carbs that will appear in the bloodstream as glucose within 20 minutes of being eaten) in one serving of the food item.

**SAG-per-serving:** The amount in grams of *Slowly Available Glucose* (carbs that will appear in the bloodstream as glucose within 120 minutes of being eaten) in one serving of the food item.

**protein-per-serving:** The amount in grams of protein in one serving of the food item.

**fat-per-serving:** The amount in grams of fat in one serving of the food item.

For example, the following line describes a *Breakfast* with serving size 100 grams, which consists of 25 grams of RAG, 30 grams of SAG, 25 grams of protein and 20 grams of fat:

`1 Breakfast 100 25 30 25 20`



### exercisefile


The *exercisefile* contains the description of different exercise types. Each line in this file describes a different exercise type using the following format:

`id exercise-description intensity-in-METs`

**id:** An integer used to identify the exercise type in *eventsfile*.

**exercise-description:** A text string (containing no spaces) that describes the exercise type.

**intensity-in-METs:** The intensity of the exercise type in units of METs.

For example, the following line describes a *SlowWalk* activity with intensity 3 METs:

`1 SlowWalk 3`



### paramsfile

The *paramsfile* describes the values of various parameters affecting the operation of different organs. The paramsfile can be used to modify the values of those parameters for which the default values are not appropriate. Each line in this file describes a different parameter type using the following format:

`body-state organ parameter value`

**body-state:** One of {ALL, FED_EXERCISING, FED_RESTING, POSTABSORPTIVE_EXERCISING, POSTABSORPTIVE_RESTING}. Must be upper case.

**organ:** One of {BLOOD, BRAIN, HUMAN_BODY, INTESTINE, KIDNEYS, LIVER, MUSCLES, PORTAL_VEIN, STOMACH}. Must be upper case.

**parameter:** A text string that must exactly match a parameter of the organ type specified by the previous argument. A list of the parameters for each organ type along with an explanation of its purpose can be found in [Parameters](Parameters.md).

**value:** A value, either an integer or a float double, to assign to the parameter specified by the previous argument. The type of each parameter can be found in [Parameters](Parameters.md). Note that doubles can be assigned an integer, but not the other way around.

For example, the following lines in the paramsfile are setting the values of a few simulation parameters in the *HumanBody* class:

```
ALL HUMAN_BODY bodyWeight 57
ALL HUMAN_BODY age 23
ALL HUMAN_BODY gender 0
ALL HUMAN_BODY fitnessLevel 50
```



### eventsfile

The *eventsfile* specifies the events that the simulator will simulate. Each line in this file specifies a different event in the following format:

`time-stamp event-type event-subtype howmuch`

**time-stamp:** The time when the event will be fired, given in the following format `day:hour:minute`.

**event-type:** One of {EXERCISE, FOOD, HALT}. If the value is HALT the simulation terminates at the event firing time.

**event-subtype:** An integer matching an id that was specified in *foodsfile* or *exercisefile*. If *event-type* is HALT a value must be provided, but it is ignored.

**howmuch:** If *event-type* is EXERCISE, the duration in minutes of the exercise activity. If *event-type* is FOOD, the amount of grams of the food eaten. If *event-type* is HALT a value must be provided, but it is ignored.

Here is a sample eventsfile:

```
0:8:0 FOOD 1 100
0:13:0 FOOD 2 135
0:16:0 FOOD 3 60
0:18:0 EXERCISE 1 30
0:20:0 FOOD 4 135
1:6:0 HALT 0 0
```



### seed

The *seed* is a string that acts as the seed for the random number generation.



### outputfile

The *outputfile* is the file where the simulation output will be stored. The simulation output is appended to any existing contents of this file.



## Examples

A few examples of different configuration files described above as well as a few python scripts to run the simulations using different seeds can be seen in the folder *tests/system/*. It contains whole-program tests used to validate the simulator.


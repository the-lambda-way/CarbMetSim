# CarbMetSim

Redesigned to be usable either as the original command-line program or as a library.


# Roadmap to version 1.0

Due to the number of changes I made, I've decided to release an upcoming commit as version 1.0.

Features of version 1.0:

* Organized the code into folders that will be useful as an open-source library.
* Moved from C++11 to C++17 since it is common and adds useful features.
* Separated the command-line interface from the simulator. This includes moving all string input and output into the cli, replacing all input strings with input parameters, and replacing all output strings with diagnostic variables. Additionally, the main loops was removed from SimCtl. The containing program should run its own main loop and call SimCtl::runTick() on each iteration. It can then read which events occurred and whatever parameters it needs. SimCtl::runTick()) returns false after the HALT event has fired.
* SimCtl is now the main library (i.e. non-textual) interface. Previously global variables were moved into SimCtl. The body and organs are accessed through public SimCtl pointers.
* Moved externally accessed variables from private to public (encapsulation will need to be improved in the future). Moved variables only internally used from public to private.
* Changed programmer-related error checks into assertions. Changed user-related errors into exceptions.
* Simplified many things, including moving subprocesses into their own functions.
* There was inconsistent code formatting and style, so I picked my own and applied it everywhere.
* Implemented a workaround for negative zero in the output. This is a bug in the simulator output that needs to be fixed.
* Added system tests corresponding to each one of the example simulations. Added catch2 as a third-party library for adding future unit and integration tests.
* Added documentation.


# Todo before version 1.0 can be released

Update documentation to reflect the new design, and update this readme.


# Original Readme

A discrete event simulator for tracking blood glucose level based on carbodydrate metabolism in human body.

Authors:
--------------
Mukul Goyal (mukul@uwm.edu)

Buket Aydas (baydas@uwm.edu)

Husam Ghazaleh (ghazaleh@uwm.edu)

Usage:
----------------
The simulator is implemented in C++. A simple makefile has been provided.

Run the simulator in the following manner:

carbmetsim foodsfile exercisefile paramsfile eventsfile seed outputfile

"foodsfile":
--------------
The "foodsfile" contains the description of different food items. Each line in this file describes a different food using
the following format:

id food-description serving-size-in-grams RAG-per-serving SAG-per-serving Protein-per-serving Fat-per-serving

The "id" is an integer id. The food item is referred to in "eventsfile" using this "id".
The "food-description" is a text string (containing no space) that describes the food item.
The "serving-size-in-grams" specifies the size in grams of one serving of this food item.
The "RAG-per-serving" specifies the amount in grams of "Rapidly Available Glucose" (carbs that will appear in the bloodstream
as glucose within 20 minutes of being eaten) in one serving of the food item.
The "SAG-per-serving" specifies the amount in grams of "Slowly Available Glucose" (carbs that will appear in the bloodstream
as glucose within 120 minutes of being eaten) in one serving of the food item.
The "Protein-per-serving" specifies the amount in grams of protein in one serving of the food item.
The "Fat-per-serving" specifies the amount in grams of fat in one serving of the food item.

For example, the following line describes a "Breakfast" with serving size 100 grams, which consists of 25 grams of RAG, 30
grams of SAG, 25 grams of protein and 20 grams of fat:

1 Breakfast 100 25 30 25 20

"exercisefile":
----------------
The "exercisefile" contains the description of different exercise types. Each line in this file describes a different exercise
type using the following format:

id exercise-description intensity-in-METs

The "id" is an integer id. The exercise type is referred to in "eventsfile" using this "id".
The "exercise-description" is a text string (containing no space) that describes the exercise type.
The "intensity-in-METs" specifies the intensity of the exercise type in units of METs.

For example, the following line describes a "SlowWalk" activity with intensity 3 METs:

1 SlowWalk 3

"paramsfile":
-------------------
The "paramsfile" describes the values of various parameters affecting the operation of different organs. The "paramsfile" can
be used to modify the values of those parameters for which the default values are not appropriate. For example, the following
lines in the "paramsfile" are setting the values of a few simulation parameters in "HumanBody" class:

ALL HUMAN_BODY bodyWeight_ 57

ALL HUMAN_BODY age_ 23

ALL HUMAN_BODY gender_ 0

ALL HUMAN_BODY fitnessLevel_ 50

"eventsfile":
-----------------
The "eventsfile" specifies the events that the simulator will simulate. Each line in this file specifies a different event in
the following format:

time-stamp event-type event-subtype howmuch

Here, "time-stamp" indicates the time when the event will be fired and is specified in the following format "day:hour:minute".

The simulator currently supports the following "event-type"s:

0: Food Event

1: Exercise Event

2: Halt Event

If the "event-type" is 2 (indicating the Halt Event), the simulation terminates at the event firing time.

The "event-subtype" is same as the "id" of a food/exercise type in "foodsfile" or "exercisefile".

For a food event, "howmuch" indicates the amount in grams of the food eaten. For an exercise event, "howmuch" indicates the duration in minutes of the exercise activity.

Here is a sample "eventsfile":

0:8:0 0 1 100

0:13:0 0 2 135

0:16:0 0 3 60

0:18:0 1 1 30

0:20:0 0 4 135

1:6:0 2 0 0

"seed":
-----------------
The "seed" is a string that acts as the seed for the random number generation.

"outputfile":
-----------------
The "outputfile" is the file where the simulation output will be stored. The simulation output is appended to any existing contents of this file.

Folder "examples":
----------------------------------
A few examples of different configuration files described above as well as a few python scripts to run the simulations using different seeds can be seen in the folder "examples".

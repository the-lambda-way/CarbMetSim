# Changelog


14 Feb 2021: Mike Castillo
------------------------------------------------------------------------------------------------------------------------

Refactored to be a library that can be integrated into other projects. Since this touches every part of the code base, I took the opportunity to do a overall refactoring as well.

* Created an independent command-line interface and moved all input and output to there.
* Replaced all output messages with diagnostic variables. Moved all the messages into the cli.
* Replaced all input strings with input parameters. Moved all the strings into the cli.
* Removed the main loop from SimCtl. The containing program runs its own main loop. On each iteration, it calls SimCtl::runTick(), then reads which events occurred as well as whatever parameters it needs. SimCtl::runTick() returns false after the HALT event has fired.
* Moved the global variables into SimCtl. As a result, multiple simulators can exist side-by-side.
* SimCtl is now the main (non-textual) interface. The body and organs are accessed through SimCtl.
* Moved externally accessed variables from private to public (encapsulation will need to be improved in the future). Moved variables only internally used from public to private.
* Changed programmer-related error checks into assertions. Changed user-related errors into exceptions.
* Simplified many things, including moving subprocesses into their own functions.
* There was inconsistent code formatting and style, so I picked my own and applied it everywhere.
* Implemented a workaround for negative zero in the output. This is a bug in the simulator output that needs to be fixed.
* Added system tests corresponding to each one of the example simulations.

#include "discrete_event_queue.h"

// Our build rules for tests expect a .cpp file
// EventQueue is header only since the class is a template
// This file is included as a workaround for the build rule since it is currently the only header-only class.
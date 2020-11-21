#
# 'make'        build executable file defined by MAIN
# 'make tests'  build test executables
# 'make clean'  removes all build and executable files
#

# define the executable file
MAIN = carbmetsim


#  -Wall turns on most, but not all, compiler warnings
CXX 	 = g++
CXXFLAGS = -std=c++11 -Wall
COMPILE  = $(CXX) $(CXXFLAGS) -O3


# Finds all source files in the source/ folder and defines object files for them.
CPP_SRCS = $(wildcard source/*.cpp)
C_SRCS   = $(wildcard source/*.c)
OBJS     = $(addsuffix .o,$(patsubst source/%,build/%,$(CPP_SRCS) $(C_SRCS)))


# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
all: $(MAIN)
	@echo "$(MAIN) has been compiled"


# ======================================================================================================================
# Main
# ======================================================================================================================
$(MAIN): $(OBJS)
	@$(COMPILE) -o $(MAIN) $(OBJS)


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
build/%.c.o: source/%.c source/%.h
	@mkdir -p build
	@echo "Building $@ ..."
	@$(COMPILE) -c $< -o $@


build/%.cpp.o: source/%.cpp source/%.h
	@mkdir -p build
	@echo "Building $@ ..."
	@$(COMPILE) -c $< -o $@


# ======================================================================================================================
# Tests
# ======================================================================================================================
# Finds all test source files in the tests/ folder and defines executables for them.
# Tests are independent from each other except for main.test.cpp, so object files aren't needed.
TEST_SRCS = $(filter-out tests/main.test.cpp,$(wildcard tests/*.test.cpp))
TEST_EXES = $(addprefix build/,$(TEST_SRCS:.cpp=.out))


tests: build/tests/main.test.o $(TEST_EXES)


build/tests/%.test.out: tests/%.test.cpp $(OBJS)
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) build/tests/main.test.o $(OBJS) $< -o $@


build/tests/main.test.o: tests/main.test.cpp
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) -c $< -o $@


# ======================================================================================================================
# Misc
# ======================================================================================================================
# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old build files:
.PHONY: clean
clean:
	@echo "Cleaning..."
	@$(RM) -f $(MAIN)
	@$(RM) -rf build/

#
# 'make'        build executable file defined by MAIN
# 'make tests'  build test executables
# 'make clean'  removes all build and executable files
#

# Define the executable file.
MAIN = carbmetsim

CXX 	 = g++
CXXFLAGS = -std=c++17 -Wall    # -Wall turns on most, but not all, compiler warnings.
CPPFLAGS = -MMD                # -MMD creates for each output file a file of makefile rules describing its dependencies
INCLUDES = -Isource/ -Iexternal/
COMPILE  = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -O3


# Finds all source files in the source/ folder and defines object files for them.
CPP_SRCS = $(wildcard source/*.cpp)
C_SRCS   = $(wildcard source/*.c)
OBJS     = $(addsuffix .o,$(patsubst source/%,build/%,$(CPP_SRCS) $(C_SRCS)))


# Typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
all: $(MAIN)
	@echo "$(MAIN) has been compiled"


# ======================================================================================================================
# Main
# ======================================================================================================================
$(MAIN): $(OBJS)
	@$(COMPILE) -o $(MAIN) $(OBJS)


# This is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule (a .c file) and $@: the name of the target of the rule (a .o file)
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
# Finds all test source files in the tests/ folder and defines executables for them. GNU Make doesn't have a recursive
#     subdirectory wildcard, so we rely on the shell.
TEST_SRCS = $(filter-out tests/main.test.cpp,$(shell find tests/ -name "*.test.cpp"))

# Tests are independent from each other except for main.test.cpp, so object files aren't needed.
TEST_EXES = $(addprefix build/,$(TEST_SRCS:.cpp=.out))


tests: build/tests/main.test.o $(TEST_EXES)


build/tests/system/%.test.out: tests/system/%.test.cpp $(OBJS)
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) -ggdb build/tests/main.test.o $^ -o $@


build/tests/integration/%.test.out: tests/integration/%.test.cpp source/%.cpp
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) -ggdb build/tests/main.test.o $^ -o $@


build/tests/unit/%.test.out: tests/unit/%.test.cpp source/%.cpp
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) -ggdb build/tests/main.test.o $^ -o $@


build/tests/main.test.o: tests/main.test.cpp
	@echo "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(COMPILE) -c $< -o $@


# Imports the makefile rules from the dependency files
TEST_DEPS := $(TEST_EXES:.out=.d)
-include $(TEST_DEPS)


# If your integration tests require another source file, add another rule below



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

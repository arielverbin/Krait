# Compiler and flags
CXX := g++
CXXFLAGS_BASE := -std=c++17 -Wall -Wextra -Iinclude -g -MMD -MP -Isrc -Itests -I.
RUN_CXXFLAGS := $(CXXFLAGS_BASE)
TEST_CXXFLAGS := $(CXXFLAGS_BASE) -DKRAIT_TESTING

# Directories
SRC_DIR := src
RUN_BUILD_DIR := build/run
TEST_BUILD_DIR := build/tests
RUN_OBJ_DIR := $(RUN_BUILD_DIR)/obj
TEST_OBJ_DIR := $(TEST_BUILD_DIR)/obj

# Prevent auto-deletion of object files
.PRECIOUS: $(TEST_OBJ_DIR)/%.o

# Executable output
RUN_BIN := $(RUN_BUILD_DIR)/krait

# Source files for run build (all files from src)
RUN_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
RUN_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(RUN_OBJ_DIR)/%.o,$(RUN_SRCS))
# The main program is in the project root
RUN_MAIN := Main.cpp
RUN_MAIN_OBJ := $(RUN_OBJ_DIR)/Main.o

# Source files for tests build (same src files as in run)
TEST_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Discover all test modules automatically (directories under tests/)
ALL_TEST_MODULES := $(filter-out lib, $(notdir $(wildcard tests/*)))
ALL_TEST_BINS  := $(foreach mod,$(ALL_TEST_MODULES),$(TEST_BUILD_DIR)/test_$(mod))

# Determine requested test modules (passed as extra targets besides the primary ones)
IGNORED_GOALS := test run clean clean_run clean_test
REQUESTED_TEST_MODULES := $(filter-out $(IGNORED_GOALS), $(MAKECMDGOALS))


# ============================================================================
# Build rules for tests
# ----------------------------------------------------------------------------
# Build object file for a given test's tests.cpp
$(TEST_OBJ_DIR)/%_test.o: tests/%/tests.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Link a test executable for a given module using the test objects (compiled from src)
# and the test module's object (compiled from tests/<module>/tests.cpp).
$(TEST_BUILD_DIR)/test_%: $(TEST_OBJS) $(TEST_OBJ_DIR)/%_test.o
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(TEST_CXXFLAGS) $^ -o $@

# ============================================================================
# Build rules for run target
# ----------------------------------------------------------------------------
# Link the main executable from Main.cpp and the sources from src.
$(RUN_BIN): $(RUN_OBJS) $(RUN_MAIN_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) $^ -o $@

# Compile .cpp files from src/ for run build
$(RUN_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) -c $< -o $@

# Compile Main.cpp for run build
$(RUN_OBJ_DIR)/Main.o: Main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) -c $< -o $@

# Compile .cpp files from src/ for tests build
$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# ============================================================================
# Phony Targets
# ----------------------------------------------------------------------------
.PHONY: run test clean clean_run clean_test

# Default target for running the main program
run: $(RUN_BIN)
	./$(RUN_BIN)

# Test target: if any modules are provided (e.g. "make test parser interpreter"),
# depend on those test executables; otherwise build all test executables.
test: $(if $(REQUESTED_TEST_MODULES), \
         $(foreach mod,$(REQUESTED_TEST_MODULES),$(TEST_BUILD_DIR)/test_$(mod)), \
         $(ALL_TEST_BINS))
	@if [ -n "$(REQUESTED_TEST_MODULES)" ]; then \
	  for mod in $(REQUESTED_TEST_MODULES); do \
	     echo "Running test for module '$$mod':"; \
	     ./$(TEST_BUILD_DIR)/test_$$mod || exit 1; \
	  done; \
	else \
	  for testexe in $(ALL_TEST_BINS); do \
	     echo "Running test '$$testexe':"; \
	     ./$$testexe || exit 1; \
	  done; \
	fi

# Cleaning targets
clean:
	rm -rf build

clean_run:
	rm -rf $(RUN_BUILD_DIR)

clean_test:
	rm -rf $(TEST_BUILD_DIR)

# Prevent make from treating extra test module names as files
%:
	@:

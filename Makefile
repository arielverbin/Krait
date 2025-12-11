# Compiler and flags
CXX := g++
PYTHON := /usr/bin/python3

CXXFLAGS_BASE := -std=c++20 -Wall -Wextra -Iinclude -MMD -MP -Isrc -Itests -I.
CXXFLAGS_DEBUG := $(CXXFLAGS_BASE) -DKRAIT_TESTING -g

RUN_CXXFLAGS := $(CXXFLAGS_BASE) -Irun
DEBUG_CXXFLAGS := $(CXXFLAGS_DEBUG) -Irun
TEST_CXXFLAGS := $(CXXFLAGS_DEBUG)

# Directories
SRC_DIR := src
RUN_DIR := run
BUILD_DIR = build

RUN_BUILD_DIR := $(BUILD_DIR)/run
DEBUG_BUILD_DIR := $(BUILD_DIR)/debug
TEST_BUILD_DIR := $(BUILD_DIR)/tests
RUN_OBJ_DIR := $(RUN_BUILD_DIR)/obj
DEBUG_OBJ_DIR := $(DEBUG_BUILD_DIR)/obj
TEST_OBJ_DIR := $(TEST_BUILD_DIR)/obj

# Prevent auto-deletion of object files
.SECONDARY:

# Executable outputs
RUN_BIN := $(RUN_BUILD_DIR)/krait
DEBUG_BIN := $(DEBUG_BUILD_DIR)/krait

# Source files for run build (all files from src)
SRCS = $(shell find $(SRC_DIR) -name '*.cpp') $(shell find $(RUN_DIR) -name '*.cpp')

RUN_SRCS := $(SRCS)
RUN_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(RUN_OBJ_DIR)/%.o,$(RUN_SRCS))
# The main program is in the project root
RUN_MAIN := Main.cpp
RUN_MAIN_OBJ := $(RUN_OBJ_DIR)/Main.o

# Source files for debug build (same as run)
DEBUG_SRCS := $(SRCS)
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/%.o,$(DEBUG_SRCS))
DEBUG_MAIN_OBJ := $(DEBUG_OBJ_DIR)/Main.o

# Source files for tests build (same src files as in run)
TEST_SRCS := $(SRCS)
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Discover all test modules automatically (directories under tests/)
ALL_TEST_MODULES := $(filter-out lib, $(notdir $(wildcard tests/*)))
ALL_TEST_BINS  := $(foreach mod,$(ALL_TEST_MODULES),$(TEST_BUILD_DIR)/test_$(mod))

# ============================================================================
# Build rules for tests
# ----------------------------------------------------------------------------
# Build object file for a given test's tests.cpp
$(TEST_OBJ_DIR)/%_test.o: tests/%/tests.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Link a test executable for a given module using the test objects (compiled from src)
# and the test module's object (compiled from tests/<module>/tests.cpp).
$(TEST_BUILD_DIR)/test_%: $(TEST_OBJS) $(TEST_OBJ_DIR)/%_test.o
	@mkdir -p $(TEST_BUILD_DIR)
	$(CXX) $(TEST_CXXFLAGS) $^ -lreadline -o $@

# Test target: runs all tests modules
test:
	@echo "[>] Running debug preprocessor..."
	@$(PYTHON) ./debug_preprocessor.py
	@echo "[>] Building with debug flags..."
	@$(MAKE) $(ALL_TEST_BINS) || (echo "[!] Build failed, reverting changes..." && $(PYTHON) ./debug_preprocessor.py revert && exit 1)
	@echo "[v] Build completed successfully, reverting changes..."
	@$(PYTHON) ./debug_preprocessor.py revert
	@find . -type f -exec touch {} +

	@for mod in $(ALL_TEST_MODULES); do \
		echo "Running test for module '$$mod':"; \
		./$(TEST_BUILD_DIR)/test_$$mod || exit 1; \
	done; \
	@$(MAKE) clean_test

# ============================================================================
# Build rules for run target
# ----------------------------------------------------------------------------
# Link the main executable from Main.cpp and the sources from src.
$(RUN_BIN): $(RUN_OBJS) $(RUN_MAIN_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) $^ -lreadline -o $@

# Compile .cpp files from src/ for run build
$(RUN_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) -c $< -o $@

# Compile Main.cpp for run build
$(RUN_OBJ_DIR)/Main.o: Main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(RUN_CXXFLAGS) -c $< -o $@

release: $(RUN_BIN)

# ============================================================================
# Build rules for debug target
# ----------------------------------------------------------------------------
# Link the debug executable from Main.cpp and the sources from src.
$(DEBUG_BIN): $(DEBUG_OBJS) $(DEBUG_MAIN_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_CXXFLAGS) $^ -lreadline -o $@

# Compile .cpp files from src/ for debug build
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_CXXFLAGS) -c $< -o $@

# Compile Main.cpp for debug build
$(DEBUG_OBJ_DIR)/Main.o: Main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_CXXFLAGS) -c $< -o $@

debug:
	@echo "[>] Running debug preprocessor..."
	@$(PYTHON) ./debug_preprocessor.py
	@echo "[>] Building with debug flags..."
	@$(MAKE) $(DEBUG_BIN) || (echo "[!] Build failed, reverting changes..." && $(PYTHON) ./debug_preprocessor.py revert && exit 1)
	@echo "[v] Build completed successfully, reverting changes..."
	@$(PYTHON) ./debug_preprocessor.py revert
	@find . -type f -exec touch {} +

# ============================================================================
# Phony Targets
# ----------------------------------------------------------------------------
.PHONY: debug test clean clean_run clean_debug clean_test

# ============================================================================
# Cleaning Targets
# ----------------------------------------------------------------------------
clean:
	@$(PYTHON) ./debug_preprocessor.py revert
	rm -rf $(BUILD_DIR)

clean_run:
	@$(PYTHON) ./debug_preprocessor.py revert
	rm -rf $(RUN_BUILD_DIR)

clean_debug:
	@$(PYTHON) ./debug_preprocessor.py revert
	rm -rf $(DEBUG_BUILD_DIR)

clean_test:
	@$(PYTHON) ./debug_preprocessor.py revert
	rm -rf $(TEST_BUILD_DIR)

# Prevent make from treating extra test module names as files
%:
	@:
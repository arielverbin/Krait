# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -g -MMD -MP -Isrc -Itests

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN := $(BUILD_DIR)/krait
MAIN_SRC := Main.cpp        # Main program entry point
TEST_DIR := tests
TEST_MODULES := interpreter parser
TEST_EXECUTABLES := $(foreach mod,$(TEST_MODULES),$(BUILD_DIR)/test_$(mod))

# Find all .cpp source files recursively in src/ (excluding Main.cpp)
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' ! -name 'Main.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN)

# Link the final main program
$(BIN): $(OBJS) $(BUILD_DIR)/Main.o
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp files to .o object files for source code (excluding Main.cpp)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Main.cpp into an object file
$(BUILD_DIR)/Main.o: $(MAIN_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === TESTS ===
# Run all tests
test: $(TEST_EXECUTABLES)
	@for t in $(TEST_EXECUTABLES); do \
		echo "Running $$t"; \
		$$t || exit 1; \
	done

# Pattern rule to build each test module executable
$(BUILD_DIR)/test_%: $(OBJS) $(TEST_DIR)/%/tests.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run a specific test module
run_test_%: $(BUILD_DIR)/test_%
	./$(BUILD_DIR)/test_$*
	rm -f $(BUILD_DIR)/test_$*


# === CLEANING ===
# Remove everything
clean:
	rm -rf $(BUILD_DIR)

# Remove only output of `run`
clean_run:
	rm -f $(BUILD_DIR)/Main.o $(BIN)

# Run the main program
run: $(BIN)
	./$(BIN)

.PHONY: all clean clean_run run test tests $(addprefix test_, $(TEST_MODULES))
.PHONY: run_test_%

# Include auto-generated dependency files
-include $(OBJS:.o=.d)

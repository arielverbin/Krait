# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -g -MMD -MP -Isrc -Itests

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN := $(BUILD_DIR)/krait
TEST_DIR := tests
MAIN_SRC := Main.cpp        # Main program entry point
TEST_SRC := $(TEST_DIR)/test.cpp  # Test program entry point

# Find all .cpp source files recursively in src/
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' ! -name 'Main.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Find all .cpp test files recursively in tests/
TEST_OBJS := $(BUILD_DIR)/test.o

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

# Link the test program
tests: $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD_DIR)/test_program

# Compile test.cpp into an object file
$(BUILD_DIR)/test.o: $(TEST_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files (both main and test)
clean:
	rm -rf $(BUILD_DIR)

# Clean only main program build files
clean_run:
	rm -rf $(BUILD_DIR)/Main.o $(BUILD_DIR)/$(notdir $(BIN)) $(BUILD_DIR)/Main.d

# Clean only test program build files
clean_test:
	rm -rf $(BUILD_DIR)/test.o $(BUILD_DIR)/test_program $(BUILD_DIR)/test.d

# Run the compiled main program
run: $(BIN)
	./$(BIN)

# Run the compiled test program
test: tests
	./$(BUILD_DIR)/test_program

.PHONY: all clean clean_run clean_test run test

# Include automatically generated dependency files
-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

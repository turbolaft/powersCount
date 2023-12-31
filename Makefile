# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++20 -Wall -g
# Source directory
SRC_DIR = src
# Object directory
OBJ_DIR = target/objs
# Executable name
EXE = target/main

# List of source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
# List of object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Main target
$(EXE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJ_FILES)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

target:
	mkdir -p target

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(EXE)
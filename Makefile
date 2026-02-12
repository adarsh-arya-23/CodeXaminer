# Mini Online Judge System - Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = judge
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJECTS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))

# Main target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p data
	@mkdir -p logs
	@mkdir -p submissions
	@mkdir -p executables

# Link object files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful! Run './$(TARGET)' to start."

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TARGET).exe
	@echo "Clean complete!"

# Clean all generated files
cleanall: clean
	rm -rf executables/* submissions/* logs/*
	@echo "All generated files cleaned!"

# Run the program
run: all
	./$(TARGET)

# Help
help:
	@echo "Mini Online Judge System - Build Commands"
	@echo "=========================================="
	@echo "make          - Build the project"
	@echo "make clean    - Remove build files"
	@echo "make cleanall - Remove all generated files"
	@echo "make run      - Build and run the program"
	@echo "make help     - Show this help message"

.PHONY: all directories clean cleanall run help

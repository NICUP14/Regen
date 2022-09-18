# Project structure
BUILD_DIR ?= bin
SOURCE_DIR ?= src

# Project parameters
PROJECT = main
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Compiler parameters
CXX = g++
CXXFLAGS ?=
CXXFLAGS += -g -Wall -I include

default: $(PROJECT)

$(PROJECT) : $(OBJECTS)
	$(CXX) $^ -o $(BUILD_DIR)/$@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	@rm -vrf $(BUILD_DIR)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -MMD -MP -fopenmp
SRC := DataSet.cpp \
       TreeNode.cpp \
       DecisionTree.cpp \
	   RandomForest.cpp \
       main.cpp

BUILD_DIR := build

OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)
DEP := $(OBJ:.o=.d)

TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

clean:
	rm -f $(BUILD_DIR)/* $(TARGET)

format:
	clang-format -i *.cpp *.h

.PHONY: all clean format

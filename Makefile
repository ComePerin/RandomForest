CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -MMD -MP -fopenmp

BUILD_DIR := build

# Sources "communes" (pas de main ici)
LIB_SRC := DataSet.cpp \
           TreeNode.cpp \
           DecisionTree.cpp \
           RandomForest.cpp

LIB_OBJ := $(LIB_SRC:%.cpp=$(BUILD_DIR)/%.o)
LIB_DEP := $(LIB_OBJ:.o=.d)

# Deux points d'entrée
IRIS_MAIN := main_iris.cpp
HEART_MAIN := main_heart.cpp

IRIS_OBJ := $(BUILD_DIR)/main_iris.o
HEART_OBJ := $(BUILD_DIR)/main_heart.o

IRIS_TARGET := iris
HEART_TARGET := heart

all: $(IRIS_TARGET) $(HEART_TARGET)

iris: $(IRIS_TARGET)
heart: $(HEART_TARGET)

$(IRIS_TARGET): $(LIB_OBJ) $(IRIS_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(HEART_TARGET): $(LIB_OBJ) $(HEART_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(LIB_DEP) $(IRIS_OBJ:.o=.d) $(HEART_OBJ:.o=.d)

clean:
	rm -f $(BUILD_DIR)/* $(IRIS_TARGET) $(HEART_TARGET)

format:
	clang-format -i *.cpp *.h

.PHONY: all clean format iris heart

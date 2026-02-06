CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -MMD -MP

SRC := DataSet.cpp \
       TreeNode.cpp \
       DecisionTree.cpp \
       main.cpp

OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)

TARGET := decision_tree

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

format:
	clang-format -i *.cpp *.h

.PHONY: all clean format

#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H

#include <memory>
#include <vector>

#include "DataSet.h"
#include "DecisionTree.h"
#include "TreeParameters.h"

class RandomForest {
 public:
  RandomForest(int nTrees, TreeParameters params);
  void fit(DataSet *dataSet);  // build the forest
  int predict(const std::vector<float> &sample)
      const;  // predict the class for a single sample

 private:
  int nTrees;             // number of trees in the forest
  TreeParameters params;  // parameters for tree construction
  std::vector<std::unique_ptr<DecisionTree>> trees;  // vector of decision trees
};

#endif  // RANDOMFOREST_H
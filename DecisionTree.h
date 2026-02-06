#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <memory>
#include <vector>

#include "DataSet.h"
#include "TreeNode.h"
#include "TreeParameters.h"
#include "stdexcept"

class DecisionTree {

  TreeParameters params;
  DataSet *dataSet;
  std::unique_ptr<TreeNode> root;

public:
  DecisionTree(DataSet *dataSet, TreeParameters params);
  void fit(); // build the tree
  int predict(const std::vector<float> &sample)
      const; // predict the class for a single sample
};

#endif // DECISIONTREE_H
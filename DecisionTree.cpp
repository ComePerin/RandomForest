#include "DecisionTree.h"

DecisionTree::DecisionTree(DataSet* dataSet, TreeParameters params)
    : params(params), dataSet(dataSet) {}

void DecisionTree::fit() {
  root = std::make_unique<TreeNode>(dataSet);
  root->split(params);
}

int DecisionTree::predict(const std::vector<float>& sample) const {
  if (!root) {
    throw std::runtime_error("The decision tree has not been fitted yet.");
  }
  return root->predict(sample);
}
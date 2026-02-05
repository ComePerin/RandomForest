#include "DecisionTree.h"

DecisionTree::DecisionTree(DataSet* dataSet, int maxDepth, int minSamplesSplit)
    : dataSet(dataSet),
      maxDepth(maxDepth),
      minSamplesSplit(minSamplesSplit)
{
}

void DecisionTree::fit() {
    root = std::make_unique<TreeNode>(dataSet);
    root->split();
}
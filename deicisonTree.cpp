#include "decisionTree.h"

DecisionTree::DecisionTree(DataSet* dataSet, int maxDepth, int minSamplesSplit) 
    : dataSet(dataSet), maxDepth(maxDepth), minSamplesSplit(minSamplesSplit), root(std::make_unique<TreeNode>(dataSet)) {
}

TreeNode::TreeNode(DataSet* dataSet, int featureIndex, float featureValue) 
    : dataSet(dataSet), featureIndex(featureIndex), featureValue(featureValue) {
}
#ifndef TREENODE_H
#define TREENODE_H

#include <algorithm>
#include <limits>
#include <memory>
#include <vector>

#include "DataSet.h"
#include "TreeParameters.h"

struct Split {
  int featureIndex;  // Index of the feature used for the split
  float threshold;   // Threshold value for the split
  float impurity;    // Impurity of the split
};

class TreeNode {
 public:
  // Construct a tree node from a subset of samples
  // dataSet : pointer to the full dataset (not owned)
  // sampleIndices : indices of samples belonging to this node
  // depth : depth of the node in the tree
  TreeNode(const DataSet* dataSet, std::vector<int> sampleIndices, int depth);

  TreeNode(const DataSet* dataSet);

  // Compute the best split for this node and create child nodes
  void split(const TreeParameters& params);

  int predict(const std::vector<float>& sample) const;

 private:
  // Left and right child nodes (binary tree)
  std::unique_ptr<TreeNode> left_;
  std::unique_ptr<TreeNode> right_;

  // Pointer to the full dataset (read-only)
  const DataSet* dataSet_;

  // Indices of samples handled by this node
  std::vector<int> sampleIndices_;

  // Split parameters (valid only if the node is not a leaf)
  int featureIndex_;  // Index of the feature used for the split
  float threshold_;   // Threshold value for the split

  // Predicted class if this node is a leaf
  int predictedClass_;

  // Depth of the node in the tree
  int depth_;

  // Check whether the node should be a leaf
  bool isLeaf(const TreeParameters& params) const;
  bool isLeaf() const;

  bool isLeaf_ = false;

  // Convert this node into a leaf
  void makeLeaf();

  // Compute impurity of the current node
  float computeImpurity() const;

  bool isPure() const;

  // Compute impurity resulting from a candidate split
  float computeSplitImpurity(std::vector<int>& leftClassCounts, int nLeft,
                             std::vector<int>& rightClassCounts,
                             int nRight) const;

  Split findBestSplit() const;
};

#endif  // TREENODE_H

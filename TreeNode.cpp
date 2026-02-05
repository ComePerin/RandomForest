#include "TreeNode.h"
#include "DataSet.h"

TreeNode::TreeNode(const DataSet* dataSet, std::vector<int> sampleIndices, int depth)
    :   dataSet_(dataSet),
        sampleIndices_(std::move(sampleIndices)),
        depth_(depth),
        featureIndex_(-1),
        threshold_(0.0f),
        predictedClass_(-1)
{
}

TreeNode::TreeNode(const DataSet* dataSet)
    : dataSet_(dataSet),
      featureIndex_(-1),
      threshold_(0.0f),
      predictedClass_(-1),
      depth_(0)
{
    // Initialize the node with all samples from the dataset
    int n = dataSet_->n_samples();
    sampleIndices_.reserve(n);

    for (int i = 0; i < n; ++i) {
        sampleIndices_.push_back(i);
    }
}

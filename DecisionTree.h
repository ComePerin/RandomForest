#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include<vector>
#include<memory>

#include "DataSet.h"
#include "TreeNode.h"

class DecisionTree {

    int maxDepth=-1; //no max depth by default
    int minSamplesSplit=1;

    DataSet* dataSet;
    std::unique_ptr<TreeNode> root;

    public:
        DecisionTree(DataSet* dataSet, int maxDepth, int minSamplesSplit);
        void fit(); //build the tree
        int predict(const std::vector<float>& sample) const; //predict the class for a single sample
};

#endif // DECISIONTREE_H
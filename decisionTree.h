#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include<vector>
#include<memory>

class DecisionTree {

    int maxDepth=-1; //no max depth by default
    int minSamplesSplit=1;

    DataSet* dataSet;
    std::unique_ptr<TreeNode> root;

    public:
        DecisionTree(DataSet* dataSet, int maxDepth, int minSamplesSplit);
        
};

class DataSet {
    //todo: implement DataSet class
};

class TreeNode {

    std::unique_ptr<TreeNode> left=nullptr;
    std::unique_ptr<TreeNode> right=nullptr;

    DataSet* dataSet;
    int featureIndex;
    float featureValue;

    public:
        TreeNode(DataSet*);
        TreeNode(DataSet* dataSet, int featureIndex, float featureValue);
        void makeSplit();
};

#endif // DECISIONTREE_H
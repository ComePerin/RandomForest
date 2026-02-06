#include "TreeNode.h"

#include <random>

#include "DataSet.h"
#include "Impurity.h"

TreeNode::TreeNode(const DataSet *dataSet, std::vector<int> sampleIndices,
                   int depth)
    : dataSet_(dataSet),
      sampleIndices_(std::move(sampleIndices)),
      depth_(depth),
      featureIndex_(-1),
      threshold_(0.0f),
      predictedClass_(-1),
      isLeaf_(false) {}

TreeNode::TreeNode(const DataSet *dataSet)
    : dataSet_(dataSet),
      depth_(0),
      featureIndex_(-1),
      threshold_(0.0f),
      predictedClass_(-1),
      isLeaf_(false) {
  // Initialize the node with all samples from the dataset
  int n = dataSet_->n_samples();
  sampleIndices_.reserve(n);

  for (int i = 0; i < n; ++i) {
    sampleIndices_.push_back(i);
  }
}

float TreeNode::computeSplitImpurity(std::vector<int> &leftClassCounts,
                                     int nLeft,
                                     std::vector<int> &rightClassCounts,
                                     int nRight) const {
  // Split dégénéré : on l'écarte
  if (nLeft == 0 || nRight == 0) {
    return std::numeric_limits<float>::max();
  }

  float leftImpurity = impurity::computeGiniImpurity(leftClassCounts);
  float rightImpurity = impurity::computeGiniImpurity(rightClassCounts);

  int n = nLeft + nRight;

  return (static_cast<float>(nLeft) / n) * leftImpurity +
         (static_cast<float>(nRight) / n) * rightImpurity;
}

float TreeNode::computeImpurity() const {
  int c = dataSet_->n_classes();
  std::vector<int> classCounts(c, 0);

  for (int sampleIndex : sampleIndices_) {
    int label = dataSet_->getLabel(sampleIndex);
    classCounts[label]++;
  }

  return impurity::computeGiniImpurity(classCounts);
}

Split TreeNode::findBestSplit(const TreeParameters &params) const {
  Split bestSplit = {-1, 0.0f, std::numeric_limits<float>::max()};

  std::vector<int> consideredFeaturesIndices(dataSet_->n_features());
  for (int i = 0; i < dataSet_->n_features(); ++i) {
    consideredFeaturesIndices[i] = i;
  }

  if (params.maxFeatures > 0 && params.maxFeatures < dataSet_->n_features()) {
    // shuffle consideredFeaturesIndices
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(consideredFeaturesIndices.begin(),
                 consideredFeaturesIndices.end(), g);

    // take only the first maxFeatures features
    consideredFeaturesIndices.resize(params.maxFeatures);
  }

  for (int featureIndex : consideredFeaturesIndices) {
    // Sort samples_indices according to feature values for each feature
    std::vector<int> samplesIndicesSortedByFeature(sampleIndices_);

    std::sort(samplesIndicesSortedByFeature.begin(),
              samplesIndicesSortedByFeature.end(),
              [this, featureIndex](int a, int b) {
                return dataSet_->getFeatureValue(a, featureIndex) <
                       dataSet_->getFeatureValue(b, featureIndex);
              });

    // Compute class counts for the left and right splits
    std::vector<int> leftClassCounts(dataSet_->n_classes(), 0);
    std::vector<int> rightClassCounts(dataSet_->n_classes(), 0);

    int nLeft = 0;
    int nRight = sampleIndices_.size();

    for (int sampleIndex : sampleIndices_) {
      int label = dataSet_->getLabel(sampleIndex);
      rightClassCounts[label]++;
    }

    if (samplesIndicesSortedByFeature.size() < 2) continue;

    for (size_t i = 0; i < samplesIndicesSortedByFeature.size() - 1; ++i) {
      int sampleIndex = samplesIndicesSortedByFeature[i];
      int label = dataSet_->getLabel(sampleIndex);

      // Move the sample from the right split to the left split
      leftClassCounts[label]++;
      rightClassCounts[label]--;
      nLeft++;
      nRight--;

      // Check if the next feature value is different to avoid redundant splits
      float currentValue = dataSet_->getFeatureValue(sampleIndex, featureIndex);
      float nextValue = dataSet_->getFeatureValue(
          samplesIndicesSortedByFeature[i + 1], featureIndex);

      if (currentValue >= nextValue) continue;  // Skip redundant split

      // Compute impurity for the current split
      float splitImpurity = computeSplitImpurity(leftClassCounts, nLeft,
                                                 rightClassCounts, nRight);

      // Update best split if the current split has lower impurity
      if (splitImpurity < bestSplit.impurity) {
        bestSplit.featureIndex = featureIndex;
        bestSplit.threshold = (currentValue + nextValue) /
                              2.0f;  // Midpoint between current and next value
        bestSplit.impurity = splitImpurity;
      }
    }
  }

  return bestSplit;
}

void TreeNode::split(const TreeParameters &params) {
  if (isLeaf(params)) {
    makeLeaf();
    return;
  }

  Split bestSplit = findBestSplit(params);

  if (bestSplit.featureIndex == -1) {
    makeLeaf();
    return;
  }

  featureIndex_ = bestSplit.featureIndex;
  threshold_ = bestSplit.threshold;

  std::vector<int> leftIndices;
  std::vector<int> rightIndices;

  for (int sampleIndex : sampleIndices_) {
    float featureValue = dataSet_->getFeatureValue(sampleIndex, featureIndex_);
    if (featureValue <= threshold_) {
      leftIndices.push_back(sampleIndex);
    } else {
      rightIndices.push_back(sampleIndex);
    }
  }

  left_ =
      std::make_unique<TreeNode>(dataSet_, std::move(leftIndices), depth_ + 1);
  right_ =
      std::make_unique<TreeNode>(dataSet_, std::move(rightIndices), depth_ + 1);

  // Recursively split the child nodes
  left_->split(params);
  right_->split(params);

  // Clear the sample indices from the current node to save memory
  sampleIndices_.clear();
}

bool TreeNode::isLeaf(const TreeParameters &params) const {
  // Check stopping criteria
  if (params.maxDepth >= 0 && depth_ >= params.maxDepth) {
    return true;
  }

  // Check minimum samples for split
  if (sampleIndices_.size() < static_cast<size_t>(params.minSamplesSplit)) {
    return true;
  }

  // Check if all samples belong to the same class (pure node)
  if (isPure()) {
    return true;
  }

  return false;
}

bool TreeNode::isLeaf() const { return isLeaf_; }

bool TreeNode::isPure() const { return computeImpurity() == 0.0f; }

void TreeNode::makeLeaf() {
  int c = dataSet_->n_classes();
  std::vector<int> classCounts(c, 0);

  // Count labels in this node
  for (int sampleIndex : sampleIndices_) {
    int label = dataSet_->getLabel(sampleIndex);
    classCounts[label]++;
  }

  // Predicted class = majority class
  auto it = std::max_element(classCounts.begin(), classCounts.end());
  predictedClass_ = static_cast<int>(std::distance(classCounts.begin(), it));

  // No split for a leaf
  featureIndex_ = -1;
  threshold_ = 0.0f;

  // No children
  left_.reset();
  right_.reset();
  isLeaf_ = true;
}

int TreeNode::predict(const std::vector<float> &sample) const {
  if (isLeaf()) {
    return predictedClass_;
  }

  if (sample[featureIndex_] <= threshold_) {
    return left_->predict(sample);
  } else {
    return right_->predict(sample);
  }
}

#include "RandomForest.h"

#include <unordered_map>

RandomForest::RandomForest(int nTrees, TreeParameters params)
    : nTrees(nTrees), params(params) {}

void RandomForest::fit(DataSet* dataSet) {
  trees.clear();
  trees.resize(nTrees);  // Pre-allocate
#pragma omp parallel for
  for (int i = 0; i < nTrees; ++i) {
    auto tree = std::make_unique<DecisionTree>(dataSet, params);
    tree->fit();
    trees[i] = std::move(tree);
  }
}

int RandomForest::predict(const std::vector<float>& sample) const {
  if (trees.empty()) {
    throw std::runtime_error("The random forest has not been fitted yet.");
  }

  // Collect predictions from all trees
  std::vector<int> predictions;
  for (const auto& tree : trees) {
    predictions.push_back(tree->predict(sample));
  }

  // Return the majority vote
  std::unordered_map<int, int> voteCount;
  for (int pred : predictions) {
    voteCount[pred]++;
  }

  int majorityClass = -1;
  int maxVotes = 0;
  for (const auto& pair : voteCount) {
    if (pair.second > maxVotes) {
      maxVotes = pair.second;
      majorityClass = pair.first;
    }
  }

  return majorityClass;
}
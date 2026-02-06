#include <iostream>

#include "DataSet.h"
#include "DecisionTree.h"
#include "TreeParameters.h"

int main() {
  // Create a simple dataset: 6 samples, 2 features, 2 classes
  DataSet dataset(6, 2, 2);

  // Set feature values and labels for a simple XOR-like problem
  dataset.setFeatureValue(0, 0, 0.0f);
  dataset.setFeatureValue(0, 1, 0.0f);
  dataset.setLabel(0, 0);
  dataset.setFeatureValue(1, 0, 0.0f);
  dataset.setFeatureValue(1, 1, 1.0f);
  dataset.setLabel(1, 1);
  dataset.setFeatureValue(2, 0, 1.0f);
  dataset.setFeatureValue(2, 1, 0.0f);
  dataset.setLabel(2, 1);
  dataset.setFeatureValue(3, 0, 1.0f);
  dataset.setFeatureValue(3, 1, 1.0f);
  dataset.setLabel(3, 0);
  dataset.setFeatureValue(4, 0, 0.5f);
  dataset.setFeatureValue(4, 1, 0.5f);
  dataset.setLabel(4, 0);
  dataset.setFeatureValue(5, 0, 0.2f);
  dataset.setFeatureValue(5, 1, 0.8f);
  dataset.setLabel(5, 1);

  // Create tree parameters
  TreeParameters params;
  params.maxDepth = 5;
  params.minSamplesSplit = 2;

  // Build the decision tree
  DecisionTree tree(&dataset, params);
  tree.fit();

  // Test predictions
  std::cout << "Decision Tree trained successfully!\n";
  std::cout << "Testing predictions:\n";

  std::cin.get();

  std::vector<float> sample1 = {0.0f, 0.0f};
  std::cout << "Sample [0.0, 0.0] -> Class " << tree.predict(sample1) << "\n";

  std::vector<float> sample2 = {1.0f, 1.0f};
  std::cout << "Sample [1.0, 1.0] -> Class " << tree.predict(sample2) << "\n";

  return 0;
}

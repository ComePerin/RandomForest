#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "DataSet.h"
#include "RandomForest.h"
#include "TreeParameters.h"

struct Sample {
  std::vector<float> features;
  int label;
};

std::vector<Sample> loadIris(const std::string& path) {
  std::ifstream file(path);
  std::vector<Sample> data;
  std::string line;

  std::getline(file, line);  // skip header

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;

    std::getline(ss, token, ',');  // skip ID

    std::vector<float> features(4);
    for (int i = 0; i < 4; ++i) {
      std::getline(ss, token, ',');
      features[i] = std::stof(token);
    }

    std::getline(ss, token, ',');
    int label = (token == "Iris-setosa")       ? 0
                : (token == "Iris-versicolor") ? 1
                                               : 2;

    data.push_back({features, label});
  }

  return data;
}

int main() {
  auto allData = loadIris("data/Iris.csv");

  // Separate by class
  std::vector<Sample> class0, class1, class2;
  for (const auto& s : allData) {
    if (s.label == 0)
      class0.push_back(s);
    else if (s.label == 1)
      class1.push_back(s);
    else
      class2.push_back(s);
  }

  // Shuffle each class
  std::mt19937 rng(42);
  std::shuffle(class0.begin(), class0.end(), rng);
  std::shuffle(class1.begin(), class1.end(), rng);
  std::shuffle(class2.begin(), class2.end(), rng);

  // 70/30 split per class
  std::vector<Sample> train, test;

  int train0 = class0.size() * 0.7;
  int train1 = class1.size() * 0.7;
  int train2 = class2.size() * 0.7;

  train.insert(train.end(), class0.begin(), class0.begin() + train0);
  test.insert(test.end(), class0.begin() + train0, class0.end());

  train.insert(train.end(), class1.begin(), class1.begin() + train1);
  test.insert(test.end(), class1.begin() + train1, class1.end());

  train.insert(train.end(), class2.begin(), class2.begin() + train2);
  test.insert(test.end(), class2.begin() + train2, class2.end());

  // Create datasets
  DataSet trainSet(train.size(), 4, 3);
  DataSet testSet(test.size(), 4, 3);

  for (int i = 0; i < train.size(); ++i) {
    for (int j = 0; j < 4; ++j) {
      trainSet.setFeatureValue(i, j, train[i].features[j]);
    }
    trainSet.setLabel(i, train[i].label);
  }

  for (int i = 0; i < test.size(); ++i) {
    for (int j = 0; j < 4; ++j) {
      testSet.setFeatureValue(i, j, test[i].features[j]);
    }
    testSet.setLabel(i, test[i].label);
  }

  // Train model
  TreeParameters params;
  params.maxDepth = 5;
  params.minSamplesSplit = 2;
  params.maxFeatures = 2;

  RandomForest forest(10, params);
  forest.fit(&trainSet);

  // Test on test dataset
  int correct = 0;
  std::cout << "Testing on " << test.size() << " samples:\n";
  std::cout << "--------------------------------------------\n";

  for (int i = 0; i < test.size(); ++i) {
    int predicted = forest.predict(test[i].features);
    int actual = test[i].label;

    std::cout << "Sample " << i << ": Predicted = " << predicted
              << ", Actual = " << actual;

    if (predicted == actual) {
      std::cout << " ✓\n";
      correct++;
    } else {
      std::cout << " ✗\n";
    }
  }

  std::cout << "--------------------------------------------\n";
  std::cout << "Accuracy: " << (100.0 * correct / test.size()) << "% "
            << "(" << correct << "/" << test.size() << ")\n";

  return 0;
}
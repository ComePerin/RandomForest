#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "DataSet.h"
#include "RandomForest.h"
#include "TreeParameters.h"

struct Sample {
  std::vector<float> features;
  int label;
};

// Helper functions to encode categorical variables
float encodeSex(const std::string& sex) { return (sex == "M") ? 1.0f : 0.0f; }

float encodeChestPainType(const std::string& type) {
  if (type == "ATA") return 0.0f;
  if (type == "NAP") return 1.0f;
  if (type == "ASY") return 2.0f;
  if (type == "TA") return 3.0f;
  return -1.0f;
}

float encodeRestingECG(const std::string& ecg) {
  if (ecg == "Normal") return 0.0f;
  if (ecg == "ST") return 1.0f;
  if (ecg == "LVH") return 2.0f;
  return -1.0f;
}

float encodeExerciseAngina(const std::string& angina) {
  return (angina == "Y") ? 1.0f : 0.0f;
}

float encodeSTSlope(const std::string& slope) {
  if (slope == "Up") return 0.0f;
  if (slope == "Flat") return 1.0f;
  if (slope == "Down") return 2.0f;
  return -1.0f;
}

std::vector<Sample> loadHeart(const std::string& path) {
  std::ifstream file(path);
  std::vector<Sample> data;
  std::string line;

  std::getline(file, line);  // skip header

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;

    std::vector<float> features;

    // Age
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // Sex
    std::getline(ss, token, ',');
    features.push_back(encodeSex(token));

    // ChestPainType
    std::getline(ss, token, ',');
    features.push_back(encodeChestPainType(token));

    // RestingBP
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // Cholesterol
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // FastingBS
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // RestingECG
    std::getline(ss, token, ',');
    features.push_back(encodeRestingECG(token));

    // MaxHR
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // ExerciseAngina
    std::getline(ss, token, ',');
    features.push_back(encodeExerciseAngina(token));

    // Oldpeak
    std::getline(ss, token, ',');
    features.push_back(std::stof(token));

    // ST_Slope
    std::getline(ss, token, ',');
    features.push_back(encodeSTSlope(token));

    // HeartDisease (label)
    std::getline(ss, token, ',');
    int label = std::stoi(token);

    data.push_back({features, label});
  }

  return data;
}

int main() {
  auto allData = loadHeart("data/heart.csv");

  std::cout << "Loaded " << allData.size() << " samples from heart.csv\n";

  // Separate by class
  std::vector<Sample> class0, class1;
  for (const auto& s : allData) {
    if (s.label == 0)
      class0.push_back(s);
    else
      class1.push_back(s);
  }

  std::cout << "Class 0 (no disease): " << class0.size() << " samples\n";
  std::cout << "Class 1 (disease): " << class1.size() << " samples\n";

  // Shuffle each class
  std::mt19937 rng(42);
  std::shuffle(class0.begin(), class0.end(), rng);
  std::shuffle(class1.begin(), class1.end(), rng);

  // 70/30 split per class
  std::vector<Sample> train, test;

  int train0 = class0.size() * 0.7;
  int train1 = class1.size() * 0.7;

  train.insert(train.end(), class0.begin(), class0.begin() + train0);
  test.insert(test.end(), class0.begin() + train0, class0.end());

  train.insert(train.end(), class1.begin(), class1.begin() + train1);
  test.insert(test.end(), class1.begin() + train1, class1.end());

  std::cout << "Training samples: " << train.size() << "\n";
  std::cout << "Test samples: " << test.size() << "\n\n";

  // Number of features: 11
  int n_features = 11;
  int n_classes = 2;

  // Create datasets
  DataSet trainSet(train.size(), n_features, n_classes);
  DataSet testSet(test.size(), n_features, n_classes);

  for (size_t i = 0; i < train.size(); ++i) {
    for (int j = 0; j < n_features; ++j) {
      trainSet.setFeatureValue(i, j, train[i].features[j]);
    }
    trainSet.setLabel(i, train[i].label);
  }

  for (size_t i = 0; i < test.size(); ++i) {
    for (int j = 0; j < n_features; ++j) {
      testSet.setFeatureValue(i, j, test[i].features[j]);
    }
    testSet.setLabel(i, test[i].label);
  }

  // Train model
  TreeParameters params;
  params.maxDepth = 5;
  params.minSamplesSplit = 2;
  params.maxFeatures = 3;

  std::cout << "Training Random Forest with " << 500 << " trees...\n";
  RandomForest forest(500, params);
  forest.fit(&trainSet);
  std::cout << "Training complete!\n\n";

  // Test on test dataset
  int correct = 0;
  int truePositives = 0, trueNegatives = 0;
  int falsePositives = 0, falseNegatives = 0;

  std::cout << "Testing on " << test.size() << " samples:\n";
  std::cout << "--------------------------------------------\n";

  for (size_t i = 0; i < test.size(); ++i) {
    int predicted = forest.predict(test[i].features);
    int actual = test[i].label;

    std::cout << "Sample " << i << ": Predicted = " << predicted
              << ", Actual = " << actual;

    if (predicted == actual) {
      std::cout << " ✓\n";
      correct++;
      if (actual == 1)
        truePositives++;
      else
        trueNegatives++;
    } else {
      std::cout << " ✗\n";
      if (predicted == 1)
        falsePositives++;
      else
        falseNegatives++;
    }
  }

  std::cout << "--------------------------------------------\n";
  std::cout << "Accuracy: " << (100.0 * correct / test.size()) << "% "
            << "(" << correct << "/" << test.size() << ")\n";

  std::cout << "\nConfusion Matrix:\n";
  std::cout << "True Positives: " << truePositives << "\n";
  std::cout << "True Negatives: " << trueNegatives << "\n";
  std::cout << "False Positives: " << falsePositives << "\n";
  std::cout << "False Negatives: " << falseNegatives << "\n";

  if (truePositives + falseNegatives > 0) {
    float sensitivity =
        100.0 * truePositives / (truePositives + falseNegatives);
    std::cout << "Sensitivity (Recall): " << sensitivity << "%\n";
  }

  if (trueNegatives + falsePositives > 0) {
    float specificity =
        100.0 * trueNegatives / (trueNegatives + falsePositives);
    std::cout << "Specificity: " << specificity << "%\n";
  }

  if (truePositives + falsePositives > 0) {
    float precision = 100.0 * truePositives / (truePositives + falsePositives);
    std::cout << "Precision: " << precision << "%\n";
  }

  return 0;
}
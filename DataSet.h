#ifndef DATASET_H
#define DATASET_H

#include <vector>

class DataSet {
 public:
  DataSet(int n, int d, int c);

  // Read accesors
  float getFeatureValue(int sampleIndex, int featureIndex) const;
  int getLabel(int sampleIndex) const;

  // Write accessors
  void setFeatureValue(int sampleIndex, int featureIndex, float value);
  void setLabel(int sampleIndex, int label);

  // getters
  int n_samples() const;
  int n_features() const;
  int n_classes() const;

 private:
  int n_;  // number of samples
  int d_;  // number of features
  int c_;  // number of classes

  // Contiguous data values for each feature, stored in a 2D vector (n_ x d_)
  // X_[i*d_ + j] gives the value of the j-th feature for the i-th sample
  std::vector<float> X_;
  std::vector<int>
      y_;  // Class labels for each sample, stored in a 1D vector (n_)
};

#endif  // DATASET_H
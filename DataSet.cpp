#include "DataSet.h"

DataSet::DataSet(int n, int d, int c)
    : n_(n), d_(d), c_(c) {
    X_.resize(n_ * d_, 0.0f);
    y_.resize(n_, 0);
}

float DataSet::getFeatureValue(int sampleIndex, int featureIndex) const {
    return X_[sampleIndex * d_ + featureIndex];
}

int DataSet::getLabel(int sampleIndex) const {
    return y_[sampleIndex];
}

void DataSet::setFeatureValue(int sampleIndex, int featureIndex, float value) {
    X_[sampleIndex * d_ + featureIndex] = value;
}

void DataSet::setLabel(int sampleIndex, int label) {
    y_[sampleIndex] = label;
}

int DataSet::n_samples() const {
    return n_;
}

int DataSet::n_features() const {
    return d_;
}

int DataSet::n_classes() const {
    return c_;
}

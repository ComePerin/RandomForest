#ifndef TREEPARAMETERS_H
#define TREEPARAMETERS_H

struct TreeParameters {
    int maxDepth = -1;          // -1 => no limit
    int minSamplesSplit = 2;
    int minSamplesLeaf = 1;
    int maxFeatures = -1;       // -1 => use all features
};

#endif // TREEPARAMETERS_H
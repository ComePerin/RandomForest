#ifndef IMPURITY_H
#define IMPURITY_H

#include <vector>

namespace impurity {

    float computeGiniImpurity(const std::vector<int>& classCounts) {
        int totalSamples = 0;
        for (int count : classCounts) {
            totalSamples += count;
        }
    
        if (totalSamples == 0) {
            return 0.0f; // No samples, impurity is zero
        }
    
        float impurity = 1.0f;
        for (int count : classCounts) {
            float proportion = static_cast<float>(count) / totalSamples;
            impurity -= proportion * proportion; // Gini impurity formula
        }

        return impurity;
    }

}

#endif // IMPURITY_H
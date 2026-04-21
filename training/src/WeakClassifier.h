#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H
#include <vector>
#include "HaarFeature.h"
class WeakClassifier{
public:
    WeakClassifier(HaarFeature f);
    WeakClassifier(HaarFeature f, float threshold, int polarity, float error);
    void trainWeakClass(const std::vector<std::vector<std::vector<int>>> &images, 
        const std::vector<int> &labels,
        const std::vector<float> &weights);
    int predictFace(const std::vector<std::vector<int>> &intImage);
    float getThresholdValue();
    int getPolarityValue();
    float getErrorValue();
private:
    HaarFeature feature;
    float threshold;
    int polarity;
    float error;
};
#endif

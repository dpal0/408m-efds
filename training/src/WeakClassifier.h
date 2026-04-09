#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H
#include <vector>
#include "HaarFeature.h"
class WeakClassifier{
public:
    WeakClassifier(HaarFeature f);
    void trainWeakClass(const std::vector<int*> &images, const std::vector<int>
        const std::vector<float> &weights);
    int predictFace(int *intImage);
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

#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H
#include <vector>
#include "HaarFeature.h"

class WeakClassifier{
public:

    // constructors
    WeakClassifier (HaarFeature f);
    WeakClassifier (HaarFeature f, float thresh, int pol, float err);
    
    // accessors
    HaarFeature* getHaarFeature() { return &feature; }
    float getThreshold() { return threshold; }
    int getPolarity() { return polarity; }
    float getError() { return error; }

    void train(const std::vector<int*> &images, const std::vector<int>
        const std::vector<float> &weights);
    int predictFace(int *intImage);

private:
    HaarFeature feature;
    float threshold;
    int polarity;
    float error;
};

#endif

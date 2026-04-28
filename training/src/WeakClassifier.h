<<<<<<< HEAD
#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H

#include <vector>
#include "HaarFeature.h"

class WeakClassifier{

public:
    WeakClassifier(HaarFeature f);
    WeakClassifier(HaarFeature f, float threshold, int polarity, float error);
    
    float train(const std::vector<std::vector<std::vector<int>>> &images, 
        const std::vector<int> &labels,
        const std::vector<float> &weights);
    int classify(const std::vector<std::vector<int>> &intImage);
    
    // accessors
    HaarFeature* getHaarFeature() { return &feature; }
    float getThreshold() { return threshold; }
    int getPolarity() { return polarity; }
    float getError() { return error; }

private:
    HaarFeature feature;
    float threshold;
    int polarity;
    float error;
};

#endif
=======
#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H
#include <vector>
#include "HaarFeature.h"
class WeakClassifier{
public:
    WeakClassifier(HaarFeature f);
    void train(const std::vector<int*> &images, const std::vector<int>
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
>>>>>>> 4045f82620771edf1fde6ce2322a25c9c4104f4f

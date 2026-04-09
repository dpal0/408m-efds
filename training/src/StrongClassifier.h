
#include "WeakClassifier.h"
#include <vector>

struct wc_weight_pair
{
    WeakClassifier *wc;
    float weight;
};

class StrongClassifier
{
public:
    StrongClassifier();
    StrongClassifier(std::vector<WeakClassifier*> wc, float *weight);
    StrongClassifier(std::vector<WeakClassifier*> wc, float *weight, float threshold);
    int classify(const std::vector<std::vector<int>> &integralImage) const;
    void train(std::vector<WeakClassifier *> wc_given, std::vector<std::vector<int>> &positive_set, std::vector<std::vector<int>> &negative_set, int rounds);

protected:
    std::vector<wc_weight_pair> weak_classifiers;
    float threshold;
};

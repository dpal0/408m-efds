
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
    StrongClassifier(std::vector<wc_weight_pair> wc);
    StrongClassifier(std::vector<wc_weight_pair> wc, float threshold);
    int classify(const std::vector<std::vector<int>> &integralImage) const;
    void train(std::vector<HaarFeature *> features, std::vector<std::vector<int>> &positive_set, std::vector<std::vector<int>> &negative_set, int rounds);
    void set_threshold(float threshold);
    void set_weak_classifiers(std::vector<wc_weight_pair> weak_classifiers);
    std::vector<wc_weight_pair> get_weak_classifiers() const;
    float get_threshold() const;

protected:
    std::vector<wc_weight_pair> weak_classifiers;
    float threshold;
};

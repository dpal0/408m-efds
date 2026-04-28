
#include "WeakClassifier.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

class StrongClassifier
{
public:
    struct wc_weight_pair
    {
        WeakClassifier wc;
        float weight;
    };

    // constructors
    StrongClassifier();
    StrongClassifier(std::vector<wc_weight_pair> wc);
    StrongClassifier(std::vector<wc_weight_pair> wc, float threshold);
<<<<<<< HEAD

    // accessors
    std::vector<wc_weight_pair> getWeakClassifiers() { return weak_classifiers; }
    float getThreshold() { return threshold; }

    // mutators
    void set_threshold(float threshold) { this->threshold = threshold; }
    void set_weak_classifiers(std::vector<wc_weight_pair> weak_classifiers) { this->weak_classifiers = weak_classifiers; }

    int classify(const std::vector<std::vector<int>> &integralImage);
    void train(std::vector<HaarFeature> features, std::vector<std::vector<std::vector<int>>> &positive_set, std::vector<std::vector<std::vector<int>>> &negative_set, int rounds);
=======
    int classify(const std::vector<std::vector<int>> &integralImage) const;
    void train(std::vector<HaarFeature *> features, std::vector<std::vector<int>> &positive_set, std::vector<std::vector<int>> &negative_set, int rounds);
    void set_threshold(float threshold);
    void set_weak_classifiers(std::vector<wc_weight_pair> weak_classifiers);
    std::vector<wc_weight_pair> get_weak_classifiers() const;
    float get_threshold() const;
>>>>>>> 4045f82620771edf1fde6ce2322a25c9c4104f4f

protected:
    std::vector<wc_weight_pair> weak_classifiers;
    float threshold;
};

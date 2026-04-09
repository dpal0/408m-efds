
#include "StrongClassifier.h"

/** Constructor for training */
StrongClassifier::StrongClassifier() {
  this->threshold = 0.0;
}

/** Constructor for classification with selected weak classifiers and weights, no threshold given */
StrongClassifier::StrongClassifier(std::vector<WeakClassifier*> wc, float *weight) {
  this->wc = wc;
  this->threshold = 0.0;
  for (int i=0; i<wc.size(); i++) this->weight.push_back(weight[i]);
}

/** Constructor for classification with selected weak classifiers, weights, and threshold */
StrongClassifier::StrongClassifier(std::vector<WeakClassifier*> wc, float *weight, float threshold) {
  for (int i=0; i<wc.size(); i++) {
    this->weak_classifiers.push_back({wc[i], weight[i]});
  }
  this->threshold = threshold;
}

/** Classify method, takes in an integral image and returns 1 if it is a face and 0 if it is not */
int StrongClassifier::classify(const std::vector<std::vector<int>> &integralImage) const
{
  float sum = 0.0;

  // Sum the weighted outputs of each weak classifier, assuming Erica has classify method returning 0 or 1
  for (size_t i = 0; i < this->weak_classifiers.size(); i++)
  {
    int h = this->weak_classifiers[i].wc->classify(integralImage);
    sum += this->weak_classifiers[i].weight * h;
  }

  return (sum >= threshold) ? 1 : 0;
}

/** Training method, generates a linear combination of weak classifiers using AdaBoost algorithm 
  and saves it in the strong classifier. */
void StrongClassifier::train(std::vector<WeakClassifier *> wc_given, std::vector<std::vector<int>> &positive_set, std::vector<std::vector<int>> &negative_set, int rounds)
{
  int num_classifiers = wc_given.size();
  int num_positive = positive_set.size();
  int num_negative = negative_set.size();

  // Initialize strong classifier with empty weak classifiers and weights
  this->weak_classifiers = std::vector<wc_weight_pair>(rounds);
  this->threshold = 0.0;

  // Initialize sample weights
  std::vector<float> sample_weights(num_positive + num_negative);
  for (size_t i = 0; i < num_positive; i++)
  {
    sample_weights[i] = 1.0 / (2 * num_positive);
  }
  for (size_t i = num_positive; i < (num_positive + num_negative); i++)
  {
    sample_weights[i] = 1.0 / (2 * num_negative);
  }

  // Iterate through boosting rounds
  for (size_t t = 0; t < rounds; t++)
  {
    // Train weak classifiers using sample weights and record error for each, assuming Erica has train method returning error rate
    std::vector<float> wc_errors(num_classifiers);
    for (size_t i = 0; i < num_classifiers; i++)
    {
      wc_errors[i] = wc_given[i]->train(positive_set, negative_set, sample_weights);
    }

    // Step 2: Choose the best weak classifier
    size_t best_index = std::distance(wc_errors.begin(), std::min_element(wc_errors.begin(), wc_errors.end()));
    float best_error = wc_errors[best_index];
    float beta = best_error / (1 - best_error);

    // Step 4: Update sample weights for the next round
    for (size_t i = 0; i < num_positive; i++)
    {
      if (wc_given[best_index]->classify(positive_set[i]) == 1)
      {
        sample_weights[i] *= std::pow(beta, 1);
      }
      else
      {
        sample_weights[i] *= std::pow(beta, 0);
      }
    }
    for (size_t i = num_positive; i < (num_positive + num_negative); i++)
    {
      if (wc_given[best_index]->classify(negative_set[i - num_positive]) == 0)
      {
        sample_weights[i] *= std::pow(beta, 1);
      }
      else
      {
        sample_weights[i] *= std::pow(beta, 0);
      }
    }

    // Step 1: Normalize sample weights
    float sum_weights = std::accumulate(sample_weights.begin(), sample_weights.end(), 0.0f);
    for (size_t i = 0; i < sample_weights.size(); i++)
    {
      sample_weights[i] /= sum_weights;
    }

    // Calculate coefficient (alpha) for the chosen weak classifier, log base should be experimented with to find best performance
    float alpha = std::log(1 / beta);

    // Final step: Store the selected weak classifier and its coefficient in the strong classifier
    this->weak_classifiers.push_back({wc_given[best_index], alpha});
  }
}

#include <iostream>
#include <typeinfo>
#include "StrongClassifier.h"
using namespace std;

/** Constructor for training */
StrongClassifier::StrongClassifier()
{
  this->threshold = 0.0;
}

/** Constructor for classification with selected weak classifiers and weights, no threshold given */
StrongClassifier::StrongClassifier(std::vector<wc_weight_pair> wc)
{
  this->weak_classifiers = wc;
  this->threshold = 0.0;
}

/** Constructor for classification with selected weak classifiers, weights, and threshold */
StrongClassifier::StrongClassifier(std::vector<wc_weight_pair> wc, float threshold)
{
  this->weak_classifiers = wc;
  this->threshold = threshold;
}

/** Classify method, takes in an integral image and returns 1 if it is a face and 0 if it is not */
int StrongClassifier::classify(const std::vector<std::vector<int>> &integralImage)
{
  float sum = 0.0;

  // Sum the weighted outputs of each weak classifier
  for (size_t i = 0; i < this->weak_classifiers.size(); i++)
  {
    int h = this->weak_classifiers[i].wc.classify(integralImage);

    sum += this->weak_classifiers[i].weight * h;
  }

  return (sum >= threshold) ? 1 : 0;
}

/** Training method, generates a linear combination of weak classifiers using AdaBoost algorithm and saves it in the strong classifier. */
void StrongClassifier::train(std::vector<HaarFeature> features, std::vector<std::vector<std::vector<int>>> &positive_set, std::vector<std::vector<std::vector<int>>> &negative_set, int rounds)
{
  int num_features = features.size();
  int num_positive = positive_set.size();
  int num_negative = negative_set.size();

  // Concatenate positive and negative sets for weak classifier training
  vector<vector<vector<int>>> images = positive_set;
  images.insert(images.end(), negative_set.begin(), negative_set.end());

  vector<int> labels(num_positive, 1);
  vector<int> neg_labels(num_negative, 0);
  labels.insert(labels.end(), neg_labels.begin(), neg_labels.end());

  // Initialize weak classifiers from the given features
  std::vector<WeakClassifier> wc;
  for (int i = 0; i < features.size(); i++)
  {
    wc.push_back(WeakClassifier(features[i]));
  }

  // Initialize strong classifier with empty weak classifiers and weights
  this->weak_classifiers.clear();
  this->weak_classifiers.reserve(rounds);
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
    cout << "Training round " << t + 1 << " / " << rounds << endl;
    // Train weak classifiers using sample weights and record error for each
    std::vector<float> wc_errors(num_features);
    time_t start = time(NULL);
    for (size_t i = 0; i < num_features; i++)
    {
      wc_errors[i] = wc[i].train(images, labels, sample_weights);
      if (i % 1000 == 0) {
        time_t now = time(NULL);
        cout <<  i << " Weak classifiers trained in " << difftime(now, start) << "s, round " << t + 1 << " is appx " << (i * 100.0 / num_features) << "\% complete" << endl;
      }
      if (wc_errors[i] == 0) {
        // Avoid division by zero in later calculations
        wc_errors[i] = 1e-10; 
      }
    }

    // Choose the best weak classifier (step 2 in AdaBoost algorithm)
    size_t best_index = std::distance(wc_errors.begin(), std::min_element(wc_errors.begin(), wc_errors.end()));
    float best_error = wc_errors[best_index];
    float beta = best_error / (1 - best_error);

    // Update sample weights for the next round (step 4 in AdaBoost algorithm)
    for (size_t i = 0; i < num_positive; i++)
    {
      if (wc[best_index].classify(positive_set[i]) == 1)
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
      if (wc[best_index].classify(negative_set[i - num_positive]) == 0)
      {
        sample_weights[i] *= std::pow(beta, 1);
      }
      else
      {
        sample_weights[i] *= std::pow(beta, 0);
      }
    }

    // Normalize sample weights (step 1 in AdaBoost algorithm)
    float sum_weights = std::accumulate(sample_weights.begin(), sample_weights.end(), 0.0f);
    for (size_t i = 0; i < sample_weights.size(); i++)
    {
      sample_weights[i] /= sum_weights;
    }

    // Calculate coefficient (alpha) for the chosen weak classifier, log base should be experimented with to find best performance
    float alpha = std::log(1 / beta);

    cout << "Round " << t + 1 << ": Selected weak classifier " << best_index << " with error " << best_error << " and alpha " << alpha << endl;
    // Store the selected weak classifier and its coefficient in the strong classifier (final step in AdaBoost algorithm)
    this->weak_classifiers.push_back({wc[best_index], alpha});
    // Update threshold based on the error of the selected weak classifier, the constant 0.5 can be experimented with to find best performance
    this->threshold += 0.5 * std::log(1 / beta);
  }
}

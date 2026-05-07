#include <iostream>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <atomic>
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
#ifdef TRAINING
void StrongClassifier::train(std::vector<HaarFeature> features, std::vector<std::vector<std::vector<int>>> &positive_set, std::vector<std::vector<std::vector<int>>> &negative_set, int rounds, int stage, int stages, float targetTPR)
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

  // Define number of threads, default to 4 if hardware_concurrency cannot determine
  unsigned int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
  {
    num_threads = 4;
  }

  cout << "Training strong classifier with " << rounds << " round(s) and " << num_threads << " threads for stage " << stage << endl;

  // Iterate through boosting rounds
  for (size_t t = 0; t < rounds; t++)
  {
    cout << "\nBegin training round " << t + 1 << " / " << rounds << " of stage " << stage << "/" << stages << endl;

    // Train weak classifiers using sample weights and record error for each
    std::vector<float> wc_errors(num_features);
    time_t start = time(NULL);

    // Variables to manage multithreading of wc training
    std::vector<std::thread> threads;
    std::mutex log_mutex;
    std::atomic<int> features_done{0};

    // Divide features into chunks, one chunk per thread
    auto worker = [&](int thread_id)
    {
      int chunk = (num_features + num_threads - 1) / num_threads;
      int begin = thread_id * chunk;
      int end = std::min(begin + chunk, num_features);

      for (int i = begin; i < end; i++)
      {
        wc_errors[i] = wc[i].train(images, labels, sample_weights);

        if (wc_errors[i] == 0)
        {
          wc_errors[i] = 1e-10f;
        }

        // Log progress every ~10%
        int divisor = ((num_features / num_threads) / 10) + 1;
        int done = features_done++;

        if (thread_id == 0 && i % divisor == 0)
        {
          std::lock_guard<std::mutex> lock(log_mutex);
          time_t now = time(NULL);
          cout << done << " WC trained in " << difftime(now, start)
               << "s, round " << t + 1 << "/" << rounds << " of stage " << stage << "/" << stages << " is ~" << ((done * 100.0) / num_features) << "\% complete" << endl;
        }
      }
    };

    // Start worker threads for parallel training of weak classifiers
    for (unsigned int i = 0; i < num_threads; i++)
    {
      threads.emplace_back(worker, i);
    }

    // Wait for all threads before proceeding
    for (auto &th : threads)
    {
      th.join();
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

    cout << "Stage " << stage << "/" << stages << " Round " << t + 1 << "/" << rounds << ": Selected WC " << best_index << " w/ error " << best_error << " and alpha " << alpha << " in " << difftime(time(NULL), start) << "s" << endl;

    // Store the selected weak classifier and its coefficient in the strong classifier (final step in AdaBoost algorithm)
    this->weak_classifiers.push_back({wc[best_index], alpha});
    // Update threshold based on the error of the selected weak classifier, the constant 0.5 can be experimented with to find best performance
    this->threshold += 0.5 * std::log(1 / beta);
  }
  // Sweep threshold down until target TPR is met
  while (true)
  {
    int tp = 0;
    for (vector<vector<int>> img : positive_set)
    {
      if (this->classify(img) == 1)
      {
        tp++;
      }
    }

    float tpr = (float)tp / num_positive;
    if (tpr >= targetTPR)
      break;

    this->threshold -= 0.005f;
  }
}
#endif
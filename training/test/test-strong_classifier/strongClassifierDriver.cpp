#define UNIT_TESTING
#include <iostream>
#include <vector>
#include "../../src/main.cpp"

using namespace std;

int main() {

    int numStages = 3;
    
    vector<vector<vector<int>>> face_training    = loadImages("data/face");
    vector<vector<vector<int>>> nonface_training = loadImages("data/non-face");
    vector<vector<vector<int>>> currentNegatives = nonface_training;

    vector<HaarFeature> features = generateFeatures();

    // Cascade training loop 
    for (int stage = 0; stage < numStages; stage++) {

        cout << "\n=== Training stage " << stage + 1 << " / " << numStages
             << " ===" << endl;
        cout << "Positives: " << face_training.size()
             << "  Negatives: " << currentNegatives.size() << endl;


        // Construct and train the strong classifier
        StrongClassifier sc;
        int rounds = stage + 1; // number of weak classifiers to use in this stage
        sc.train(features, face_training, currentNegatives, rounds);

        // nonface_training shrinks each stage: only false positives from 
        // the previous stage are passed to the next stage as negatives.
        for (size_t i = 0; i < currentNegatives.size(); i++) {
            if (!sc.classify(currentNegatives[i])) {
                currentNegatives.erase(currentNegatives.begin() + i);
            }
        }
    }
    
    cout << "\n=== Training complete ===" << endl;
    cout << "Positives: " << face_training.size()
            << "  Negatives: " << currentNegatives.size() << endl << endl;

    return 0;
}

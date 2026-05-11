#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/train/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    // stage 1 (2 weak classifiers)
    HaarFeature *hf1 = new HaarFeature(1, 2, 3, 4, TWO_RECT_VERTICAL);
    WeakClassifier *wc1 = new WeakClassifier(*hf1, 5.5, 6, 7.7);
    StrongClassifier::wc_weight_pair pair1{*wc1, 8.8};

    HaarFeature *hf2 = new HaarFeature(4, 4, 10, 6, THREE_RECT_HORIZONTAL);
    WeakClassifier *wc2 = new WeakClassifier(*hf2, 50.3, 62.1, 77.12);
    StrongClassifier::wc_weight_pair pair2{*wc2, 89.8};

    vector<StrongClassifier::wc_weight_pair> wc_vector1;
    wc_vector1.push_back(pair1);
    wc_vector1.push_back(pair2);

    StrongClassifier *sc1 = new StrongClassifier(wc_vector1, 9.9);

    makeConfigFile (sc1, 1, ".");

    // stage 2 (1 weak classifiers)
    HaarFeature *hf3 = new HaarFeature(1, 2, 3, 4, TWO_RECT_VERTICAL);
    WeakClassifier *wc3 = new WeakClassifier(*hf3, 5.5, 6, 7.7);
    StrongClassifier::wc_weight_pair pair3{*wc3, 8.8};

    vector<StrongClassifier::wc_weight_pair> wc_vector2;
    wc_vector2.push_back(pair3);

    StrongClassifier *sc2 = new StrongClassifier(wc_vector2, 9.9);

    makeConfigFile (sc2, 2, ".");
}
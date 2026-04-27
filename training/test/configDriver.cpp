#include "HaarFeature.h"
#include "WeakClassifier.h"
#include "StrongClassifier.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

void makeConfigFile (StrongClassifier* sc, int stage) {
    ofstream configFile ("config" + to_string(stage) + "txt");

    configFile << sc->getThreshold() << endl;

    vector<StrongClassifier::wc_weight_pair> wc_vector = sc->getWeakClassifiers();
    
    for (StrongClassifier::wc_weight_pair it : wc_vector) {
        WeakClassifier* wc = it.wc;
        float weight = it.weight;

        HaarFeature* feature = wc->getHaarFeature();
        float threshold = wc->getThreshold();
        int polarity = wc->getPolarity();
        float error = wc->getError();

        int x = feature->getX();
        int y = feature->getY();
        int w = feature->getWidth();
        int h = feature->getHeight();
        int type = feature->getType();

        configFile << x << " "
                   << y << " "
                   << w << " "
                   << h << " "
                   << type << " "
                   << threshold << " "
                   << polarity << " "
                   << error << " "
                   << weight << endl;
    }
    
    configFile.close();
}

int main(int argc, char** argv) {
    HaarFeature *hf1 = new HaarFeature(1, 2, 3, 4, TWO_RECT_VERTICAL);
    WeakClassifier *wc1 = new WeakClassifier(*hf1, 5.5, 6, 7.7);
    StrongClassifier::wc_weight_pair pair1 = {wc1, 8.8};

    HaarFeature *hf2 = new HaarFeature(10, 20, 30, 40, THREE_RECT_HORIZONTAL);
    WeakClassifier *wc2 = new WeakClassifier(*hf2, 81.2, 39, 0421.2);
    StrongClassifier::wc_weight_pair pair2 = {wc2, 102.4};

    vector<StrongClassifier::wc_weight_pair> wc_vector = {pair, pair2};
    StrongClassifier *sc = new StrongClassifier(wc_vector, 9.9);

    makeConfigFile(sc, 1);
    
    return 0;
}
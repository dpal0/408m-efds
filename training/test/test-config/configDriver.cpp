#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/StrongClassifier.h"
#include "../../src/WeakClassifier.h"
#include "../../src/HaarFeature.h"
using namespace std;

/*******************************************************************************
 * generate config file for one strong classifier
 * For each strong classifier, pulls all the information related to the strong
 classifier and writes it to a file.
 The file is setup as follows:
    1 line with strong classifier threshold
    1 line for each weak classifier with x, y, width, height, feature type, 
        wc_threshold, polarity, error, and weight
 ******************************************************************************/
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

/* Tests HaarFeater.h functionality with respect to computing 
the value of different featires. Uses a 6x6 array as the tester
integral image. 
Note: make sure when using this tester, IMG_DIM = 6 in HaarFeature.h */
int main (int argc, char** argv) {

    HaarFeature *hf = new HaarFeature(1, 2, 3, 4, TWO_RECT_VERTICAL);
    WeakClassifier *wc = new WeakClassifier(*hf, 5.5, 6, 7.7);
    StrongClassifier::wc_weight_pair pair;
    pair.wc = wc;
    pair.weight = 8.8;
    vector<StrongClassifier::wc_weight_pair> wc_vector;
    wc_vector.push_back(pair);
    StrongClassifier *sc = new StrongClassifier(wc_vector, 9.9);

    makeConfigFile (sc, 1);
}
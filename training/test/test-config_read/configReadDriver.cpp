#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/validate/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    StrongClassifier sc = readConfigFile(1, ".");
    
    ofstream out("out.txt");

    out << sc.getThreshold() << endl;

    vector<StrongClassifier::wc_weight_pair> wc_vector = sc.getWeakClassifiers();

    for (StrongClassifier::wc_weight_pair it : wc_vector)
    {
        WeakClassifier wc = it.wc;
        float weight = it.weight;

        HaarFeature *feature = wc.getHaarFeature();
        float threshold = wc.getThreshold();
        int polarity = wc.getPolarity();
        float error = wc.getError();

        int x = feature->getX();
        int y = feature->getY();
        int w = feature->getWidth();
        int h = feature->getHeight();
        int type = feature->getType();

        out << x << " "
                   << y << " "
                   << w << " "
                   << h << " "
                   << type << " "
                   << threshold << " "
                   << polarity << " "
                   << error << " "
                   << weight << endl;
    }

    out.close();

}
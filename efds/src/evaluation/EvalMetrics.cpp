#include "EvalMetrics.h"
using namespace std;

EvalMetrics::EvalMetrics (int cascades, int faces, int nonfaces) {
    this->cascades = cascades;
    this->faces = faces;
    this->nonfaces = nonfaces;
    groundTruth();
}

EvalMetrics::~EvalMetrics () {
    delete[] truth;
}

void EvalMetrics::calculateCascade (char* out_filename) {
    
    // vars used within the loop
    ifstream real;
    ofstream out(out_filename);
    int TP = 0, FP = 0, TN = 0, FN = 0;
    double TPR, FPR, overall_TPR = 1, overall_FPR = 1;
    int ind;
    char classification;

    for (int iter = 1; iter <= cascades; iter++) {

        // open current cascade's real data
        real.open("out" + to_string(iter) + ".txt");

        // count TP, FP, TN, FN for current cascade
        while (real >> ind) {

            real >> classification;

            if (classification == 'F') {
                if (truth[ind-1]) {
                    TP++;
                } else {
                    FP++;
                }
            } else {
                if (truth[ind-1]) {
                    FN++;
                } else {
                    TN++;
                }
                real >> classification;
            }
        }

        // calculate TPR & FPR
        TPR = (double)TP / (TP + FN);
        FPR = (double)FP / (FP + TN);
        overall_TPR *= TPR;
        overall_FPR *= FPR;

        // write the current cascade's evaluation metrics to the output file
        out << "(cascade " << iter << ") TP: " << TP << ", FP: " 
            << FP << ", TN: " << TN << ", FN: " << FN 
            << ", TPR: " << TPR << ", FPR: " << FPR <<'\n'; 

        // reset variables for next iteration
        TP = 0;
        FP = 0;
        TN = 0;
        FN = 0;

        // close ifstream
        real.close();
    }

    // write overall evaluation metrics to the output file
    out << "\nOverall TPR = " << overall_TPR 
        << ", Overall FPR = " << overall_FPR << endl; 
    
    out.close();

}

void EvalMetrics::groundTruth () {
    truth = new bool[faces + nonfaces];
    fill(truth, truth + faces, true);
    fill(truth + faces + 1, truth + faces + nonfaces, false);
}

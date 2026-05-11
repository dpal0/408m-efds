#include "EvalMetrics.h"
using namespace std;

EvalMetrics::EvalMetrics (int cascades, int faces, int nonfaces) {
    this->cascades = cascades;
    this->faces = faces;
    this->nonfaces = nonfaces;
     // Running confusion-matrix totals across accumulate() calls.
    int TP_total;
    int FP_total;
    int TN_total;
    int FN_total;

    groundTruth();
}

EvalMetrics::~EvalMetrics () {
    delete[] truth;
}

void EvalMetrics::calculateCascade (const char* out_filename) {
    
    // vars used within the loop
    ifstream real(out_filename);
    int TP = 0, FP = 0, TN = 0, FN = 0;
    double TPR, FPR;
    int ind;
    string classification;
    if(!real.is_open()){
        cout<<"could not open file" <<endl;
        return;
    }
    while (real >>ind >>classification) {
        if(ind<=0 || ind>faces+nonfaces){
            cout<<"invalid idx: " <<ind<<endl;
            continue;
        }
        if (classification == "F") {
            if (truth[ind-1]) {
                TP++;
            } else {
                FP++;
            }
        } else if (classification=="NF"){
            if (truth[ind-1]) {
                FN++;
            } else {
                TN++;
            }
        }else{
            cout<<"invalid classification"<<endl;
        }
    }
    // calculate TPR & FPR
    TPR = (TP+FN==0)? 0 : (double)TP / (TP + FN);
    FPR = (FP+TN==0)? 0 :(double)FP / (FP + TN);
    double precision=(TP+FP==0)? 0: (double)TP/(TP+FP);
    double F1=(precision+TPR==0)? 0: 2*((precision*TPR)/(precision+TPR));
    cout<<"TP: "<<TP<<endl;
    cout<<"FP: "<<FP<<endl;
    cout<<"TN: "<<TN<<endl;
    cout<<"FN: "<<FN<<endl;
    cout<<"FPR: "<<FPR<<endl;
    cout<<"TPR/Recall: "<<TPR<<endl;
    cout<<"Precision: "<<precision<<endl;
    cout<<"F1-Score: "<<F1<<endl;
    real.close();

}

void EvalMetrics::accumulate (const char* out_filename, bool is_face_file) {
    ifstream real(out_filename);
    if (!real.is_open()) {
        cout << "could not open file: " << out_filename << endl;
        return;
    }
 
    int ind;
    string classification;
    while (real >> ind >> classification) {
        // We don't trust the index against the combined truth[] array here,
        // because each file's indices are local to its own graph run
        // (face.txt has indices 1..faces, non-face.txt has indices 1..nonfaces).
        // The ground truth for the entire file is determined by is_face_file.
        if (classification == "F") {
            if (is_face_file) {
                TP_total++;
            } else {
                FP_total++;
            }
        } else if (classification == "NF") {
            if (is_face_file) {
                FN_total++;
            } else {
                TN_total++;
            }
        } else {
            cout << "invalid classification: " << classification << endl;
        }
    }
    real.close();
}
 

void EvalMetrics::report () {
    double TPR = (TP_total + FN_total == 0) ? 0
                 : (double)TP_total / (TP_total + FN_total);
    double FPR = (FP_total + TN_total == 0) ? 0
                 : (double)FP_total / (FP_total + TN_total);
    double precision = (TP_total + FP_total == 0) ? 0
                       : (double)TP_total / (TP_total + FP_total);
    double F1 = (precision + TPR == 0) ? 0
                : 2 * ((precision * TPR) / (precision + TPR));
 
    cout << "TP: " << TP_total << endl;
    cout << "FP: " << FP_total << endl;
    cout << "TN: " << TN_total << endl;
    cout << "FN: " << FN_total << endl;
    cout << "FPR: " << FPR << endl;
    cout << "TPR/Recall: " << TPR << endl;
    cout << "Precision: " << precision << endl;
    cout << "F1-Score: " << F1 << endl;
}

void EvalMetrics::groundTruth () {
    truth = new bool[faces + nonfaces];
    fill(truth, truth + faces, true);
    fill(truth + faces, truth + faces + nonfaces, false);
}

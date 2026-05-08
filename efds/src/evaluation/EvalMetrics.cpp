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

void EvalMetrics::groundTruth () {
    truth = new bool[faces + nonfaces];
    fill(truth, truth + faces, true);
    fill(truth + faces, truth + faces + nonfaces, false);
}

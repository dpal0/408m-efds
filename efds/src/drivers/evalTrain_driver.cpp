#include "../../src/evaluation/EvalMetrics.h"
#include <iostream>
int main(){
    //EvalMetrics evalFace(1,485,0);
    // //ground truth has faces=485, and nonface=0
    // cout<<"_____________All Face_______"<<endl;
    // evalFace.calculateCascade("face.txt");
    // cout<<"_____________All No Face_______"<<endl;
    // EvalMetrics evalNoFace(1,0,909);
    // //ground truth has faces=485, and nonface=0
    // evalNoFace.calculateCascade("non-face.txt");
    EvalMetrics eval(1, 485, 909);
    eval.accumulate("face.txt", true);
    eval.accumulate("non-face.txt", false);
    eval.report();
    return 0;
}
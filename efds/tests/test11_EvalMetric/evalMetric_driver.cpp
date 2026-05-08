#include "../../src/evaluation/EvalMetrics.h"
#include <iostream>
int main(){
    EvalMetrics evaluate(1,3,3);
    //ground truth has faces=3 and nonfaces=3
//   So actual truth is 1F, 2F, 3F, 4NF, 5NF, 6NF
    // test normal output
    cout<<"_____test normal output_____"<<endl;
    evaluate.calculateCascade("test1.txt");
    //test correct output
    cout<<"_____test correct output_____"<<endl;
    evaluate.calculateCascade("test2.txt");
    // test none correct output
    cout<<"_____test none correct output_____"<<endl;
    evaluate.calculateCascade("test3.txt");
    // test incorrect order output
    cout<<"_____test incorrect order output_____"<<endl;
    evaluate.calculateCascade("test4.txt");
    //test incorrect classification outputs
    cout<<"_____test incorrect classification outputs_____"<<endl;
    evaluate.calculateCascade("test5.txt");
    //test missing outputs
    cout<<"_____test missing outputs_____"<<endl;
    evaluate.calculateCascade("test6.txt");
    //test duplicate outputs
    cout<<"_____test duplicate outputs_____"<<endl;
    evaluate.calculateCascade("test7.txt");
    EvalMetrics eval(1,4,0); //test ground truth are all true
    cout<<"_____test all true output_____"<<endl;
    eval.calculateCascade("test8.txt");
    EvalMetrics eval1(1,0,4); // test ground true are all false
    cout<<"_____test all false output_____"<<endl;
    eval1.calculateCascade("test8.txt");
    
    return 0;
}
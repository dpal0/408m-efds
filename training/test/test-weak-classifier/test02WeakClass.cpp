#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "HaarFeature.h"
#include "WeakClassifier.h"
using namespace std;
int* loadIntImage(const string &filename, int &rows, int &cols){
    ifstream file(filename);
    vector<int> values;
    string line;
    rows=0;
    cols=0;
    if(!file.is_open()){
        cerr<<"Error: could not open file "<<filename<<endl;
        return nullptr;
    }
    while(getline(file,line)){
        stringstream ss(line);
        string val;
        int count=0;
        while(getline(ss,val,',')){
            values.push_back(stoi(val));
            count++;
        }
        if(cols==0){
            cols=count;
        }
        rows++;
    }
    int* img=new int[values.size()];
    for(int i=0; i<values.size(); i++){
        img[i]=values[i];
    }
    file.close();
    return img;
}
int main(){
    int rows,cols;
    int* img1=loadIntImage("Face0001.txt",rows,cols);
    int* img2=loadIntImage("Face0002.txt",rows,cols);
    int* img3=loadIntImage("Not0001.txt",rows,cols);
    cout<<"Loaded image: "<<rows<<"x"<<cols<<endl;
    HaarFeature feature(0,0,4,4,TWO_RECT_VERTICAL);
    WeakClassifier wc(feature);
    vector<int*> images;
    vector<int> labels;
    vector<float> weights;
    images.push_back(img1);
    images.push_back(img2);
    images.push_back(img3);
    labels.push_back(1);
    labels.push_back(1);
    labels.push_back(0);
    weights.push_back(1.0f);
    weights.push_back(5.0f);
    weights.push_back(3.0f);
    wc.trainWeakClass(images,labels, weights);
    cout<<"Threshold: "<<wc.getThresholdValue()<<endl;
    cout<<"Polarity: "<<wc.getPolarityValue()<<endl;
    cout<<"Error: "<<wc.getErrorValue()<<endl;
    int result=wc.predictFace(img1);
    cout<<"Prediction for img1: "<<result<<endl;
    result=wc.predictFace(img2);
    cout<<"Prediction for img2: "<<result<<endl;
    result=wc.predictFace(img3);
    cout<<"Prediction for img3: "<<result<<endl;
    delete[] img1;
    delete[] img2;
    delete[] img3;
    return 0;
}



#include <iostream>
#include <vector>
#include "../../src/WeakClassifier.h"
#include "../../src/HaarFeature.h"

using namespace std;

int main(){

    vector<vector<int>> img1 = {{1,2,3,4,5},
                                {2,4,6,8,10},
                                {3,6,9,12,15},
                                {4,8,12,16,20},
                                {5,10,15,20,25}};

    vector<vector<int>> img2 = {{2,2,2,2,2},
                                {2,2,2,2,2},
                                {2,2,2,2,2},
                                {2,2,2,2,2},
                                {2,2,2,2,2}};

    vector<vector<int>> img3 = {{5,5,5,5,5},
                                {5,5,5,5,5},
                                {5,5,5,5,5},
                                {5,5,5,5,5},
                                {5,5,5,5,5}};

    vector<vector<vector<int>>> images;
    images.push_back(img1);
    images.push_back(img2);
    images.push_back(img3);

    std::vector<int> labels={0,0,1};
    std::vector<float> weights={0.33f,0.33f, 0.34f};
    HaarFeature feature(0,0,2,2,TWO_RECT_VERTICAL);

    WeakClassifier wc(feature);

    wc.train(images,labels,weights);

    std::cout<<"Training Values: "<<std::endl;
    std::cout<<"Threshold: "<<wc.getThreshold()<<std::endl;
    std::cout<<"Polarity: "<<wc.getPolarity()<<std::endl;
    std::cout<<"Error: "<<wc.getError()<<std::endl;
    std::cout<<"Prediction Values: "<<std::endl;

    for(int i=0; i<images.size(); i++){
        int pred=wc.classify(images[i]);
        std::cout<<"Image "<<i
                  <<" | Label: "<<labels[i]
                  <<" | Prediction: "<< pred<<std::endl;
    }
    
    return 0;
}

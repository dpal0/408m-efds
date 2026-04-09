#include <vector>
#include <limits>
#include <iostream>
#include "WeakClassifier.h"

WeakClassifier::WeakClassifier(HaarFeature f)
    :feature(f), threshold(0), polarity(1), error(0){}

float WeakClassifier::getThresholdValue(){
    return this->threshold;
}

int WeakClassifier::getPolarityValue(){
    return this->polarity;
}
float WeakClassifier::getErrorValue(){
    return this->error;
}

void WeakClassifier::trainWeakClass(const std::vector<int*> &images, const std>
        const std::vector<float> &weights){
    //labels=list of 1's and 0's where 1=face, 0=no face of the ith image.
    //images=list of integral images.
    // output error if input sizes are different
    if(images.size() !=labels.size() || images.size() !=weights.size() || labe>
        std::cerr<<"Error in input sizes. Sizes must all be the same size.\n";
        return;
    }
    size_t lenImage=images.size(); //get size of the images array
    float lowestError=std::numeric_limits<float>::max(); //gets max integer nu>
    std::vector<int> featValArr(lenImage); //initialize array for getting the >
    //used to determine polarity
    int polarPlus=1;
    int polarMinus=-1;
    // used to get the final polarity and final threshold
    int foundPolar=1;
    int foundThreshold=0;
    // put in values into feature value array before finding best threshold an>
    for(size_t i=0; i<lenImage; i++){
        featValArr[i]=feature.computeFeatureVal(images[i]);
    }
    //loop through images
    for(size_t i=0; i<lenImage; i++){
        //set current feature value to predict threshold.
        int predictThreshold=featValArr[i];
        //initialize errors to see if polarity +1 errors are higher than
        // polarity -1 errors.
        float errors1=0.0f; //errors for polarity +1
        float errors2=0.0f; //errors for polarity -1
        for(size_t j=0; j<lenImage; j++){ //check for polarity +1
            int predict=0; //predict=face or no face
            if(polarPlus*featValArr[j]<polarPlus*predictThreshold){
                predict=1; //face
            }else{
                predict=0; //no face
            }
            if(predict!=labels[j]){ //when predict does not match expected
                errors1 +=weights[j]; //increase weight.
            }
        }
        for(size_t j=0; j<lenImage; j++){ //check for polarity -1
            int predict=0;
            if(polarMinus*featValArr[j]<polarMinus*predictThreshold){
                predict=1;
            }else{
                predict=0;
            }
            if(predict!=labels[j]){
                errors2 +=weights[j];
            }
        }
        std::cerr<<"Threshold predict: "<<predictThreshold<<std::endl;
        std::cerr<<"Threshold found: "<<foundThreshold<<std::endl;
        std::cerr<<"errors1: "<<errors1<<std::endl;
        std::cerr<<"errors2: "<<errors2<<std::endl;
        if(errors1<=errors2 && errors1<lowestError){ //when errors1 has less e>
            lowestError=errors1;
            foundPolar=polarPlus;
            foundThreshold=predictThreshold;
        }else if(errors2<errors1 && errors2<lowestError){ //vice versa
            lowestError=errors2;
            foundPolar=polarMinus;
            foundThreshold=predictThreshold;
        }
    }
    //set values after finding the best threshold, polarity, and lowest error.
    this->error=lowestError;
    this->polarity=foundPolar;
    this->threshold=foundThreshold;
}
int WeakClassifier::predictFace(int *intImage){ //input integral image.
    float featVal=feature.computeFeatureVal(intImage); //used HaarFeature and >
    if(polarity*featVal<polarity*threshold){ //use trained threshold and polar>
        return 1; //face
    }
    return 0; //not face
}




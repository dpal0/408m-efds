#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "WeakClassifier.h"


WeakClassifier::WeakClassifier(HaarFeature f)
    :feature(f), threshold(0), polarity(1), error(0){}
WeakClassifier::WeakClassifier(HaarFeature f, float threshold, int polarity, float error)
    :feature(f), threshold(threshold), polarity(polarity), error(error){}

float WeakClassifier::train(const std::vector<std::vector<std::vector<int>>> &images, const std::vector<int> &labels,
        const std::vector<float> &weights){
    //labels=list of 1's and 0's where 1=face, 0=no face of the ith image.
    //images=list of integral images.
    // output error if input sizes are different
    if(images.size() !=labels.size() || images.size() !=weights.size() || labels.size() !=weights.size()){
        std::cerr<<"Error in input sizes. Sizes must all be the same size.\n";
        return -1;
    }
    size_t lenImage=images.size(); //get size of the images array
    float lowestError=std::numeric_limits<float>::max(); //gets max integer number
    std::vector<int> featValArr(lenImage); //initialize array with the number of images
    //used to determine polarity
    int polarPlus=1;
    int polarMinus=-1;
    // used to get the final polarity and final threshold
    int foundPolar=1;
    int foundThreshold=0;
    // put in values into feature value array before finding best threshold and best polarity
    for(size_t i=0; i<lenImage; i++){
        featValArr[i]=feature.computeFeatureVal(images[i]);
    }

    // sort indices of feature value array by feature value
    std::vector<size_t> sortedIndices(lenImage);
    std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&](size_t a, size_t b) {
        return featValArr[a] < featValArr[b];
    });

    // compute total weights for positive and negative samples
    float totalPosWeight = 0.0f;
    float totalNegWeight = 0.0f;
    for (size_t i = 0; i < lenImage; i++) {
        if (labels[i] == 1) {
            totalPosWeight += weights[i];
        } else {
            totalNegWeight += weights[i];
        }
    }
 
    // cumulative weights of positive/negative samples to the left/right of the threshold respectively
    float leftPosWeight = 0.0f;
    float leftNegWeight = 0.0f;

    //loop through images
    for(size_t i=0; i<lenImage; i++){
        //get index of the image with the ith smallest feature value
        size_t currInd = sortedIndices[i];
        //set current feature value to predict threshold.
        int predictThreshold=featValArr[currInd];
        //initialize errors to see if polarity +1 errors are higher than
        // polarity -1 errors.
        float errors1 = leftNegWeight + (totalPosWeight - leftPosWeight); //errors for polarity +1
        float errors2 = leftPosWeight + (totalNegWeight - leftNegWeight); //errors for polarity -1
        
        if(errors1<=errors2 && errors1<lowestError){ //when errors1 has less than lowest error
            lowestError=errors1;
            foundPolar=polarPlus;
            foundThreshold=predictThreshold;
        }else if(errors2<errors1 && errors2<lowestError){ //vice versa
            lowestError=errors2;
            foundPolar=polarMinus;
            foundThreshold=predictThreshold;
        }

        // accumulate weights after evaulating threshold at this point
        if (labels[currInd] == 1) {
            leftPosWeight += weights[currInd];
        } else {
            leftNegWeight += weights[currInd];
        }
    }
    //set values after finding the best threshold, polarity, and lowest error.
    this->error=lowestError;
    this->polarity=foundPolar;
    this->threshold=foundThreshold;

    return lowestError;
}

int WeakClassifier::classify(const std::vector<std::vector<int>> &intImage){ //input integral image.
    float featVal=feature.computeFeatureVal(intImage); //find feature value based on input integral image

    if(polarity*featVal<polarity*threshold){ //use trained threshold and polarity to determine face or no face.
        return 1; //face
    }
    return 0; //not face
}

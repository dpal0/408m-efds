#include <iostream>
#include <vector>
#include "../../src/HaarFeature.h"
using namespace std;

/* Tests HaarFeater.h functionality with respect to computing 
the value of different featires. Uses a 6x6 array as the tester
integral image. 
Note: make sure when using this tester, IMG_DIM = 6 in HaarFeature.h */
int main (int argc, char** argv) {

    // 6x6 array acting as the integral image
    vector<vector<int>> ii = {{127,154,172,175,177,178},
                              {155,185,200,195,189,181},
                              {173,197,204,192,176,151},
                              {174,189,192,180,166,137},
                              {170,185,191,189,186,179},
                              {170,186,192,186,179,173}};

    // test vertical 2-rect feature
    HaarFeature *hf = new HaarFeature(1, 2, 3, 4, TWO_RECT_VERTICAL);
    int actual = (180+155-195-174) - (186+174-180-170);
    cout << "2 rect vertical -- calculated: " << hf->computeFeatureVal(ii) 
            << ", actual: " << actual << endl;

    // test horizontal 2-rect feature
    *hf = HaarFeature(0, 2, 6, 1, TWO_RECT_HORIZONTAL);
    actual = (204-200) - (151+200-181-204);
    cout << "2 rect horizontal -- calculated: " << hf->computeFeatureVal(ii) 
            << ", actual: " << actual << endl;

    // test vertical 3-rect feature
    *hf = HaarFeature(4, 3, 2, 3, THREE_RECT_VERTICAL);
    actual = (137+192-151-180) - (179+180-137-189) + (173+189-179-186);
    cout << "3 rect vertical -- calculated: " << hf->computeFeatureVal(ii) 
            << ", actual: " << actual << endl;

    // test horizontal 3-rect feature
    *hf = HaarFeature(0, 1, 6, 4, THREE_RECT_HORIZONTAL);
    actual = (185-154) - (189+154-175-185) + (179+175-178-189);
    cout << "3 rect horizontal -- calculated: " << hf->computeFeatureVal(ii) 
            << ", actual: " << actual << endl;

    // test 4-rect feature
    *hf = HaarFeature(0, 0, 6, 6, FOUR_RECT);
    actual = (204) - (151-204) + (192-204) - (173+204-151-192);
    cout << "4 rect -- calculated: " << hf->computeFeatureVal(ii) 
            << ", actual: " << actual << endl;
}
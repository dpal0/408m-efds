#include <iostream>
#include "HaarFeature.h"
using namespace std;

/* Constructor. creates a new HaarFeature object.
@param int x = x location of the first pixel in the rect feature starting 
    from the left of the image. Leftmost column is 0.
@param int y = y location of the first pixel in the rect feature starting 
    from the top of the image. Topmost row is 0.
@param int w = width of rect feature. must abide by the rect feature's 
    limitations. Limitations occur for 4-rect and horizontal features. In 
    those cases, it must be a multiple of the number of rectangles.
@param int y = height of rect feature. must abide by the rect feature's 
    limitations. Limitations occur for 4-rect and vertical features. In 
    those cases, it must be a multiple of the number of rectangles.
@param Feature type = the type of rect feature. Defined in enum. */
HaarFeature::HaarFeature (int x, int y, int w, int h, Feature type) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
}

/* Computes the haar feature's value. The value is the difference between the 
rectangles defined by the haar feature. There are 5 types of haar features.
Each feature has its own case statement. The value within each rectangular
region is computed using, at most, 4 array accesses since the computation is
being done on the integral image. 
@param int* integralImg = array containing the precomputed integral image
@return int = difference computed between the rectangles according to that
    specific haar feature. */
int HaarFeature::computeFeatureVal (vector<vector<int>> integralImg) {
    switch (type) {
        case TWO_RECT_VERTICAL: {
            int rect1 = compute (integralImg, x-1, y-1, x+w-1, y+h/2-1);
            int rect2 = compute (integralImg, x-1, y+h/2-1, x+w-1, y+h-1);
            return rect1 - rect2;
        } case TWO_RECT_HORIZONTAL: {
            int rect1 = compute (integralImg, x-1, y-1, x+w/2-1, y+h-1);
            int rect2 = compute (integralImg, x+w/2-1, y-1, x+w-1, y+h-1);
            return rect1 - rect2;
        } case THREE_RECT_VERTICAL: {
            int rect1 = compute (integralImg, x-1, y-1, x+w-1, y+h/3-1);
            int rect2 = compute (integralImg, x-1, y+h/3-1, x+w-1, y+h*2/3-1);
            int rect3 = compute (integralImg, x-1, y+h*2/3-1, x+w-1, y+h-1);
            return rect1 - rect2 + rect3;
        } case THREE_RECT_HORIZONTAL: {
            int rect1 = compute (integralImg, x-1, y-1, x+w/3-1, y+h-1);
            int rect2 = compute (integralImg, x+w/3-1, y-1, x+w*2/3-1, y+h-1);
            int rect3 = compute (integralImg, x+w*2/3-1, y-1, x+w-1, y+h-1);
            return rect1 - rect2 + rect3;
        } case FOUR_RECT: {
            int rect1 = compute (integralImg, x-1, y-1, x+w/2-1, y+h/2-1);
            int rect2 = compute (integralImg, x+w/2-1, y-1, x+w-1, y+h/2-1);
            int rect3 = compute (integralImg, x-1, y+h/2-1, x+w/2-1, y+h-1);
            int rect4 = compute (integralImg, x+w/2-1, y+h/2-1, x+w-1, y+h-1);
            return rect1 - rect2 + rect3 - rect4;
        } default: {
            cerr << "HaarFeature.cpp: feature type not found" << endl;
            exit(-1);
        }
    }
}

/* Helper function for the computeFeatureVal() function. Computes the area of
a predefined area on an integral image. 
@param int* integralImg = array containing the precomputed integral image
@param int x_start = row to the left of the interest area (not in interest area)
@param int y_start = col above the interest area (not in interest area)
@param int x_end = row at the right of the interest area (in the interest area)
@param int y_end = col at the bottom of the interest area (in the interest area)
@return int = difference computed between the rectangles according to that
    specific haar feature. */
int HaarFeature::compute (vector<vector<int>> integralImg, int x_start, int y_start, 
    int x_end, int y_end) {

    int retVal = integralImg[y_end][x_end];
    if (y_start >= 0 && x_start >= 0)
        retVal += integralImg[y_start][x_start];
    if (y_start >= 0)
        retVal -= integralImg[y_start][x_end];
    if (x_start >= 0)
        retVal -= integralImg[y_end][x_start];

    return retVal;
}

/* Scales the feature. This is used when computing an exhaustive list of 
all features. Instead of scaling the image, it is much quicker and easier
to scale the feature. */
void HaarFeature::scale (float factor) {
    x *= factor;
    y *= factor;
    w *= factor;
    h *= factor;
}

/* Prints the feature's values to the console. Used for debugging. */
void HaarFeature::toString () {
    cout << "x: " << x << ", y: " << y << ", width: " 
        << w << ", height: " << h << endl;
}
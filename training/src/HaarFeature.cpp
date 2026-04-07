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
@param int h = height of rect feature. must abide by the rect feature's
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

/* Computes the haar feature's value using a precomputed IntegralImage.
The value is the difference between white and black rectangular regions.
rectangleSum(r, c, h, w) returns the sum of pixels in the rectangle with
top-left corner (r, c), height h, and width w.
@param ii = precomputed integral image
@return int = difference computed between the rectangles */
int HaarFeature::computeFeatureVal (const IntegralImage& ii) const {
    switch (type) {
        case TWO_RECT_VERTICAL: {
            int rect1 = ii.rectangleSum(y,       x, h/2, w);
            int rect2 = ii.rectangleSum(y + h/2, x, h/2, w);
            return rect1 - rect2;
        } case TWO_RECT_HORIZONTAL: {
            int rect1 = ii.rectangleSum(y, x,       h, w/2);
            int rect2 = ii.rectangleSum(y, x + w/2, h, w/2);
            return rect1 - rect2;
        } case THREE_RECT_VERTICAL: {
            int rect1 = ii.rectangleSum(y,         x, h/3, w);
            int rect2 = ii.rectangleSum(y + h/3,   x, h/3, w);
            int rect3 = ii.rectangleSum(y + h*2/3, x, h/3, w);
            return rect1 - rect2 + rect3;
        } case THREE_RECT_HORIZONTAL: {
            int rect1 = ii.rectangleSum(y, x,         h, w/3);
            int rect2 = ii.rectangleSum(y, x + w/3,   h, w/3);
            int rect3 = ii.rectangleSum(y, x + w*2/3, h, w/3);
            return rect1 - rect2 + rect3;
        } case FOUR_RECT: {
            int rect1 = ii.rectangleSum(y,       x,       h/2, w/2);
            int rect2 = ii.rectangleSum(y,       x + w/2, h/2, w/2);
            int rect3 = ii.rectangleSum(y + h/2, x,       h/2, w/2);
            int rect4 = ii.rectangleSum(y + h/2, x + w/2, h/2, w/2);
            return rect1 - rect2 + rect3 - rect4;
        } default: {
            cerr << "HaarFeature.cpp: feature type not found" << endl;
            exit(-1);
        }
    }
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
void HaarFeature::toString () const {
    cout << "x: " << x << ", y: " << y << ", width: "
        << w << ", height: " << h << endl;
}

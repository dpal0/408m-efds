#ifndef haar_features_h
#define haar_features_h

/*******************************************************************************
* haar_features.h
*
* Defines Haar-like feature types and computes feature values using an
* integral image. Each feature is a weighted difference between sums of
* rectangular regions (white region sum minus black region sum).
*
* Supported feature types (as in Viola-Jones):
*   TWO_RECT_H  - two horizontally adjacent rectangles (left white, right black)
*   TWO_RECT_V  - two vertically adjacent rectangles (top white, bottom black)
*   THREE_RECT_H - three horizontal rectangles (white, black, white)
*   FOUR_RECT   - 2x2 arrangement of rectangles (diagonal pairs share sign)
*
* All features are defined by their position (row, col), width, and height
* within the 24x24 image window.
*******************************************************************************/

#include "integral_image.h"

enum HaarFeatureType {
    TWO_RECT_H,
    TWO_RECT_V,
    THREE_RECT_H,
    FOUR_RECT
};

struct HaarFeature {
    HaarFeatureType type;
    int row;    /* top-left row of the feature */
    int col;    /* top-left col of the feature */
    int width;  /* total width of the feature region */
    int height; /* total height of the feature region */
};

/*******************************************************************************
* Compute the value of a single Haar feature given an integral image.
* Returns white_region_sum - black_region_sum for the given feature type
* and position.
*******************************************************************************/
int computeHaarFeature(const IntegralImage& ii, const HaarFeature& f);

#endif
#ifndef HAAR_FEATURE_H
#define HAAR_FEATURE_H

#include "integral_image.h"

/*******************************************************************************
 * HaarFeature
 *
 * Represents one of five Haar-like rectangle feature types from Viola-Jones:
 *   TWO_RECT_VERTICAL   - two rectangles stacked vertically
 *   TWO_RECT_HORIZONTAL - two rectangles side by side horizontally
 *   THREE_RECT_VERTICAL - three rectangles stacked vertically
 *   THREE_RECT_HORIZONTAL - three rectangles side by side horizontally
 *   FOUR_RECT           - 2x2 checkerboard of four rectangles
 *
 * computeFeatureVal() returns (white region sum) - (black region sum).
 ******************************************************************************/

enum Feature {
    TWO_RECT_VERTICAL,
    TWO_RECT_HORIZONTAL,
    THREE_RECT_VERTICAL,
    THREE_RECT_HORIZONTAL,
    FOUR_RECT
};

class HaarFeature {
public:
    /* x, y: top-left corner (x = col, y = row)
     * w, h: width and height of the feature window */
    HaarFeature(int x, int y, int w, int h, Feature type);

    /* Compute the feature value using a precomputed integral image. */
    int computeFeatureVal(const IntegralImage& ii) const;

    void scale(float factor);
    void toString() const;

    int getX() const { return x; }
    int getY() const { return y; }
    int getW() const { return w; }
    int getH() const { return h; }
    Feature getType() const { return type; }

private:
    int x, y, w, h;
    Feature type;
};

#endif

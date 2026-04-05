#ifndef features_h
#define features_h

#include "integral_image.h"

/*******************************************************************************
 * HaarFeature
 *
 * Represents one of four Haar feature types from the Viola-Jones paper:
 *   TYPE_2H - two rectangles side by side (horizontal)
 *   TYPE_2V - two rectangles stacked (vertical)
 *   TYPE_3H - three rectangles side by side (horizontal)
 *   TYPE_4  - four rectangles (2x2 checkerboard)
 *
 * compute() returns (white region sum) - (black region sum).
 * The sign convention follows the VJ paper: white regions are positive,
 * black regions are negative.
 ******************************************************************************/
class HaarFeature {
public:
    enum Type { TYPE_2H, TYPE_2V, TYPE_3H, TYPE_4 };

    /* r, c: top-left corner of the feature window
     * h, w: height and width of the feature window */
    HaarFeature(Type type, int r, int c, int h, int w);

    /* Compute the feature value using the precomputed integral image. */
    int compute(const IntegralImage &ii) const;

    Type getType() const { return type; }
    int getR() const { return r; }
    int getC() const { return c; }
    int getH() const { return h; }
    int getW() const { return w; }

private:
    Type type;
    int r, c, h, w;
};

#endif
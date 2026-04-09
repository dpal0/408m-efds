#include "features.h"

HaarFeature::HaarFeature(Type type, int r, int c, int h, int w)
    : type(type), r(r), c(c), h(h), w(w) {}

int HaarFeature::compute(const IntegralImage &ii) const {
    int half_w = w / 2;
    int half_h = h / 2;
    int third_w = w / 3;
    int white, black;

    switch (type) {
        case TYPE_2H:
            /* [ black | white ] side by side */
            black = ii.rectangleSum(r, c,          h, half_w);
            white = ii.rectangleSum(r, c + half_w, h, half_w);
            return white - black;

        case TYPE_2V:
            /* [ white ] on top, [ black ] on bottom */
            white = ii.rectangleSum(r,          c, half_h, w);
            black = ii.rectangleSum(r + half_h, c, half_h, w);
            return white - black;

        case TYPE_3H:
            /* [ white | black | white ] side by side */
            white  = ii.rectangleSum(r, c,             h, third_w);
            black  = ii.rectangleSum(r, c + third_w,   h, third_w);
            white += ii.rectangleSum(r, c + 2*third_w, h, third_w);
            return white - black;

        case TYPE_4:
            /* [ white | black ]
               [ black | white ] checkerboard */
            white  = ii.rectangleSum(r,          c + half_w, half_h, half_w);
            white += ii.rectangleSum(r + half_h, c,          half_h, half_w);
            black  = ii.rectangleSum(r,          c,          half_h, half_w);
            black += ii.rectangleSum(r + half_h, c + half_w, half_h, half_w);
            return white - black;

        default:
            return 0;
    }
}
#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

extern "C"{
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include welt_cpp_actor.h"
#include welt_cpp_graph.h"
#include "iis.h"

/*******************************************************************************
* integral_image.h
*
* Computes the integral image (summed area table) from a 2D grayscale image
* represented as a vector<vector<int>>. The integral image allows any
* rectangular region sum to be computed in O(1) time using four lookups.
*
* Image dimensions are assumed to be 24x24 (per the EFDS project dataset).
*******************************************************************************/

#include <vector>
using std::vector;

class IntegralImage {
public:
    /* Construct and compute the integral image from the given grayscale image.
     * img is assumed to be a nRows x nCols 2D vector of pixel values (0-255). */
    IntegralImage(const vector<vector<int>>* img, int nRows, int nCols);

    /* Return the sum of pixel values in the rectangle defined by
     * top-left (r1, c1) and bottom-right (r2, c2), inclusive. */
    int rectSum(int r1, int c1, int r2, int c2) const;

    /* Return the value at position (row, col) in the integral image. */
    int get(int row, int col) const;

    int getNRows() const;
    int getNCols() const;

private:
    vector<vector<int>> table;
    int nRows;
    int nCols;
};

#endif


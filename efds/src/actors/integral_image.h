#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

#include <vector>
using std::vector;

/*******************************************************************************
 * IntegralImage
 *
 * Computes the integral image (summed area table) from a 2D grayscale image.
 * Allows any rectangular region sum to be computed in O(1) time using four
 * lookups.
 *
 * Coordinate convention: row 0 is the top, col 0 is the left.
 ******************************************************************************/
class IntegralImage {
public:
    /* Construct and compute the integral image from the given grayscale image.
     * img[r][c] is the pixel value at row r, column c. */
    IntegralImage(const vector<vector<int>>& img);

    /* Return the sum of pixel values in the rectangle with top-left corner
     * (r, c), height h, and width w. */
    int rectangleSum(int r, int c, int h, int w) const;
    const vector<vector<int>> &getImage() const{
        return ii;
    }
    int getNRows() const { return nRows; }
    int getNCols() const { return nCols; }

private:
    vector<vector<int>> ii;
    int nRows;
    int nCols;
};

#endif

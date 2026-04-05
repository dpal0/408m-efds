#ifndef integral_image_h
#define integral_image_h

#include <vector>
using namespace std;

/*******************************************************************************
 * IntegralImage
 *
 * Computes and stores the integral image of a given grayscale image.
 * The integral image ii satisfies:
 *   ii[r][c] = sum of all pixels in the rectangle from (0,0) to (r,c)
 *              inclusive.
 *
 * Once constructed, rectangleSum(r, c, h, w) returns the sum of pixels
 * in any rectangle in O(1) using four lookups.
 ******************************************************************************/
class IntegralImage {
public:
    /* Compute the integral image from the given grayscale image grid. */
    IntegralImage(const vector<vector<int>> &img);

    /* Return the sum of pixels in the rectangle with top-left corner (r, c),
     * height h, and width w. */
    int rectangleSum(int r, int c, int h, int w) const;

    int rows() const { return nRows; }
    int cols() const { return nCols; }

private:
    int nRows;
    int nCols;
    vector<vector<int>> ii; /* integral image storage */
};

#endif
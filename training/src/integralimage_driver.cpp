#include <iostream>
#include "integral_image.h"
using namespace std;

/* Tests IntegralImage construction and rectangleSum() using a 6x6 image
 * where p[r][c] = r + c + 2:
 *
 *   2  3  4  5  6  7
 *   3  4  5  6  7  8
 *   4  5  6  7  8  9
 *   5  6  7  8  9 10
 *   6  7  8  9 10 11
 *   7  8  9 10 11 12
 *
 * Resulting integral image:
 *    2   5   9  14  20  27
 *    5  12  21  32  45  60
 *    9  21  36  54  75  99
 *   14  32  54  80 110 144
 *   20  45  75 110 150 195
 *   27  60  99 144 195 252
 */
int main (int argc, char** argv) {

    int nRows = 6, nCols = 6;
    vector<vector<int>> img(nRows, vector<int>(nCols));
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            img[r][c] = r + c + 2;

    IntegralImage ii(img);

    // single pixel at top-left corner
    // sum = p[0][0] = 2
    cout << "single pixel (0,0)       -- computed: " << ii.rectangleSum(0, 0, 1, 1)
         << ", expected: " << 2 << endl;

    // single pixel in the middle
    // sum = p[2][3] = 7
    cout << "single pixel (2,3)       -- computed: " << ii.rectangleSum(2, 3, 1, 1)
         << ", expected: " << 7 << endl;

    // single pixel at bottom-right corner
    // sum = p[5][5] = 12
    cout << "single pixel (5,5)       -- computed: " << ii.rectangleSum(5, 5, 1, 1)
         << ", expected: " << 12 << endl;

    // full top row: sum = 2+3+4+5+6+7 = 27
    cout << "full top row             -- computed: " << ii.rectangleSum(0, 0, 1, 6)
         << ", expected: " << 27 << endl;

    // full left column: sum = 2+3+4+5+6+7 = 27
    cout << "full left col            -- computed: " << ii.rectangleSum(0, 0, 6, 1)
         << ", expected: " << 27 << endl;

    // full image: sum = 252 (from integral image bottom-right corner)
    cout << "full image               -- computed: " << ii.rectangleSum(0, 0, 6, 6)
         << ", expected: " << 252 << endl;

    // interior rectangle: rows 1-3, cols 1-3
    // (4+5+6) + (5+6+7) + (6+7+8) = 15+18+21 = 54
    cout << "interior (1,1) 3x3       -- computed: " << ii.rectangleSum(1, 1, 3, 3)
         << ", expected: " << 54 << endl;

    // bottom-right 2x2: rows 4-5, cols 4-5
    // (10+11) + (11+12) = 21+23 = 44
    cout << "bottom-right (4,4) 2x2   -- computed: " << ii.rectangleSum(4, 4, 2, 2)
         << ", expected: " << 44 << endl;

    return 0;
}

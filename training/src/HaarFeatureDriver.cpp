#include <iostream>
#include "HaarFeature.h"
#include "integral_image.h"
using namespace std;

/* Tests HaarFeature functionality using an IntegralImage.
 * Uses a 6x6 pixel image where p[r][c] = r + c + 2:
 *
 *   2  3  4  5  6  7
 *   3  4  5  6  7  8
 *   4  5  6  7  8  9
 *   5  6  7  8  9 10
 *   6  7  8  9 10 11
 *   7  8  9 10 11 12
 */
int main (int argc, char** argv) {

    // build the 6x6 pixel image
    int nRows = 6, nCols = 6;
    vector<vector<int>> img(nRows, vector<int>(nCols));
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            img[r][c] = r + c + 2;

    IntegralImage ii(img);

    HaarFeature *hf;

    // TWO_RECT_VERTICAL: x=1, y=0, w=4, h=4
    // rect1 = rows 0-1, cols 1-4 = (3+4+5+6) + (4+5+6+7) = 40
    // rect2 = rows 2-3, cols 1-4 = (5+6+7+8) + (6+7+8+9) = 56
    // expected = 40 - 56 = -16
    hf = new HaarFeature(1, 0, 4, 4, TWO_RECT_VERTICAL);
    cout << "2 rect vertical    -- computed: " << hf->computeFeatureVal(ii)
         << ", expected: " << -16 << endl;

    // TWO_RECT_HORIZONTAL: x=0, y=1, w=6, h=2
    // rect1 = rows 1-2, cols 0-2 = (3+4+5) + (4+5+6) = 27
    // rect2 = rows 1-2, cols 3-5 = (6+7+8) + (7+8+9) = 45
    // expected = 27 - 45 = -18
    *hf = HaarFeature(0, 1, 6, 2, TWO_RECT_HORIZONTAL);
    cout << "2 rect horizontal  -- computed: " << hf->computeFeatureVal(ii)
         << ", expected: " << -18 << endl;

    // THREE_RECT_VERTICAL: x=0, y=0, w=3, h=6
    // rect1 = rows 0-1, cols 0-2 = (2+3+4) + (3+4+5) = 21
    // rect2 = rows 2-3, cols 0-2 = (4+5+6) + (5+6+7) = 33
    // rect3 = rows 4-5, cols 0-2 = (6+7+8) + (7+8+9) = 45
    // expected = 21 - 33 + 45 = 33
    *hf = HaarFeature(0, 0, 3, 6, THREE_RECT_VERTICAL);
    cout << "3 rect vertical    -- computed: " << hf->computeFeatureVal(ii)
         << ", expected: " << 33 << endl;

    // THREE_RECT_HORIZONTAL: x=0, y=0, w=6, h=3
    // rect1 = rows 0-2, cols 0-1 = (2+3)+(3+4)+(4+5) = 21
    // rect2 = rows 0-2, cols 2-3 = (4+5)+(5+6)+(6+7) = 33
    // rect3 = rows 0-2, cols 4-5 = (6+7)+(7+8)+(8+9) = 45
    // expected = 21 - 33 + 45 = 33
    *hf = HaarFeature(0, 0, 6, 3, THREE_RECT_HORIZONTAL);
    cout << "3 rect horizontal  -- computed: " << hf->computeFeatureVal(ii)
         << ", expected: " << 33 << endl;

    // FOUR_RECT: x=0, y=0, w=4, h=4
    // rect1 (top-left)     = rows 0-1, cols 0-1 = 2+3+3+4 = 12
    // rect2 (top-right)    = rows 0-1, cols 2-3 = 4+5+5+6 = 20
    // rect3 (bottom-left)  = rows 2-3, cols 0-1 = 4+5+5+6 = 20
    // rect4 (bottom-right) = rows 2-3, cols 2-3 = 6+7+7+8 = 28
    // expected = 12 - 20 + 20 - 28 = -16
    *hf = HaarFeature(0, 0, 4, 4, FOUR_RECT);
    cout << "4 rect             -- computed: " << hf->computeFeatureVal(ii)
         << ", expected: " << -16 << endl;

    delete hf;
    return 0;
}

#include "integral_image.h"

IntegralImage::IntegralImage(const vector<vector<int>> &img) {
    nRows = img.size();
    nCols = img.empty() ? 0 : img[0].size();
    ii.assign(nRows, vector<int>(nCols, 0));

    for (int r = 0; r < nRows; r++) {
        for (int c = 0; c < nCols; c++) {
            if (r == 0 && c == 0) 
                ii[r][c] = img[r][c];
            else if (r == 0)
                ii[r][c] = img[r][c] + ii[r][c-1];
            else if (c == 0)
                ii[r][c] = img[r][c] + ii[r-1][c];
            else 
                ii[r][c] = img[r][c] - img[r-1][c-1] + img[r-1][c] + img[r][c-1];
        }
    }
    
}

int IntegralImage::rectangleSum(int r, int c, int h, int w) const {
    /* Bottom-right corner of the rectangle */
    int r2 = r + h - 1;
    int c2 = c + w - 1;

    int br = ii[r2][c2];
    int above = (r > 0) ? ii[r-1][c2] : 0;
    int left  = (c > 0) ? ii[r2][c-1] : 0;
    int diag  = (r > 0 && c > 0) ? ii[r-1][c-1] : 0;

    return br - above - left + diag;
}
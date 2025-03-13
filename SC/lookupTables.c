#include <stdio.h>
#include <stdint.h>

#define NUM_ROWS 3
#define NUM_COLS 3

const uint8_t dataMatrix[NUM_ROWS][NUM_COLS] = {
    { 5, 10, 15},
    {10, 15, 20},
    {15, 20, 25}
};

const uint16_t rowBrkPts[NUM_ROWS] = {3, 6, 9};
const uint16_t colBrkPts[NUM_COLS] = {100, 200, 300};

uint8_t Two_D_Lookup(const uint16_t* row_brk_pts, const uint16_t* col_brk_pts, 
                     const uint8_t dataMatrix[NUM_ROWS][NUM_COLS], 
                     const uint8_t numRows, const uint8_t numCols, 
                     uint16_t rowInput, uint16_t colInput)
{
    if (!row_brk_pts || !col_brk_pts || !dataMatrix || numRows <= 0 || numCols <= 0) {
        printf("Invalid Table\n");
        return 0;
    }

    uint8_t rowBaseInd = 0, colBaseInd = 0;

    // Row index
    if (rowInput <= row_brk_pts[0]) {
        rowBaseInd = 0;
    } else if (rowInput >= row_brk_pts[numRows-1]) {
        rowBaseInd = numRows-2;  // Last segment
    } else {
        for (uint8_t i = 0; i < numRows-1; i++) {
            if (rowInput <= row_brk_pts[i+1]) {
                rowBaseInd = i;
                break;
            }
        }
    }

    // Column index
    if (colInput <= col_brk_pts[0]) {
        colBaseInd = 0;
    } else if (colInput >= col_brk_pts[numCols-1]) {
        colBaseInd = numCols-2;  // Last segment
    } else {
        for (uint8_t i = 0; i < numCols-1; i++) {
            if (colInput <= col_brk_pts[i+1]) {
                colBaseInd = i;
                break;
            }
        }
    }

    // Bilinear interpolation (integer math)
    uint16_t x0 = row_brk_pts[rowBaseInd], x1 = row_brk_pts[rowBaseInd+1];
    uint16_t y0 = col_brk_pts[colBaseInd], y1 = col_brk_pts[colBaseInd+1];
    uint8_t f00 = dataMatrix[rowBaseInd][colBaseInd];
    uint8_t f01 = dataMatrix[rowBaseInd][colBaseInd+1];
    uint8_t f10 = dataMatrix[rowBaseInd+1][colBaseInd];
    uint8_t f11 = dataMatrix[rowBaseInd+1][colBaseInd+1];

    // Scale to avoid float (x100)
    uint16_t dx = (rowInput - x0) * 100 / (x1 - x0);
    uint16_t dy = (colInput - y0) * 100 / (y1 - y0);
    uint16_t retVal = f00 * (100 - dx) * (100 - dy) + 
                      f10 * dx * (100 - dy) + 
                      f01 * (100 - dx) * dy + 
                      f11 * dx * dy;
    retVal /= 10000;  // Back to original scale

    return (uint8_t)retVal;
}
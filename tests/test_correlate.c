#include "cvl.h"
#include "netpbm.h"
#include <assert.h>
#include <stdio.h>

void test_1(void);
void test_2(void);

void test_correlate(void) {
    test_1();
    test_2();

    printf("  cvl_correlate - passed\n");
}

void test_1(void) {
    int vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    Matrix src = createMatrix(3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            src.map[i][j] = vals[i][j];
        }
    }

    Matrix kernel = createMatrix(3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            kernel.map[i][j] = 1.0 / 9.0;
        }
    }

    Matrix dst = createMatrix(3, 3);

    cvl_correlate(&src, &dst, &kernel);

    double exp[3][3] = {
        {12.0 / 9.0, 21.0 / 9.0, 16.0 / 9.0},
        {27.0 / 9.0, 45.0 / 9.0, 33.0 / 9.0},
        {24.0 / 9.0, 39.0 / 9.0, 28.0 / 9.0},
    };

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(dst.map[i][j] == exp[i][j]);
        }
    }

    deleteMatrix(src);
    deleteMatrix(kernel);
    deleteMatrix(dst);
}

void test_2(void) {
    int vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    Matrix src = createMatrix(3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            src.map[i][j] = vals[i][j];
        }
    }

    Matrix kernel = createMatrix(2, 2);
    kernel.map[0][0] = 1 / 4.0;
    kernel.map[0][1] = 0 / 4.0;
    kernel.map[1][0] = 0 / 4.0;
    kernel.map[1][1] = 1 / 4.0;

    Matrix dst = createMatrix(3, 3);

    cvl_correlate(&src, &dst, &kernel);

    double exp[3][3] = {
        {1 / 4.0, 2 / 4.0, 3 / 4.0},
        {4 / 4.0, 6 / 4.0, 8 / 4.0},
        {7 / 4.0, 12 / 4.0, 14 / 4.0},
    };

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(dst.map[i][j] == exp[i][j]);
        }
    }

    deleteMatrix(src);
    deleteMatrix(kernel);
    deleteMatrix(dst);
}

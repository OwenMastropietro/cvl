#include <cvl/cvl.h>
#include <gtest/gtest.h>

TEST(ImgProcTest, Correlate) {
    Matrix src;
    Matrix dst;
    Matrix kernel;
    double exp[3][3] = {0};

    // ----------
    // Test 1
    // ----------
    double vals[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };

    src = cvl_mat_create_from(vals, 3, 3);

    kernel = cvl_mat_create(3, 3); // todo: cvl_mat_create_fill
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            kernel.map[i][j] = 1.0 / 9.0;
        }
    }

    dst = cvl_correlate_new(&src, &kernel);

    exp[0][0] = 12.0 / 9.0; exp[0][1] = 21.0 / 9.0; exp[0][2] = 16.0 / 9.0;
    exp[1][0] = 27.0 / 9.0; exp[1][1] = 45.0 / 9.0; exp[1][2] = 33.0 / 9.0;
    exp[2][0] = 24.0 / 9.0; exp[2][1] = 39.0 / 9.0; exp[2][2] = 28.0 / 9.0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(dst.map[i][j], exp[i][j], 1e-6);
        }
    }

    // ----------
    // Test 2
    // ----------
    kernel = cvl_mat_create(2, 2);
    kernel.map[0][0] = 1 / 4.0; kernel.map[0][1] = 0 / 4.0;
    kernel.map[1][0] = 0 / 4.0; kernel.map[1][1] = 1 / 4.0;

    dst = cvl_correlate_new(&src, &kernel);

    exp[0][0] = 1 / 4.0; exp[0][1] = 2 / 4.0; exp[0][2] = 3 / 4.0;
    exp[1][0] = 4 / 4.0; exp[1][1] = 6 / 4.0; exp[1][2] = 8 / 4.0;
    exp[2][0] = 7 / 4.0; exp[2][1] = 12 / 4.0; exp[2][2] = 14 / 4.0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(dst.map[i][j], exp[i][j], 1e-6);
        }
    }

    cvl_mat_free(src);
    cvl_mat_free(kernel);
    cvl_mat_free(dst);
}

TEST(ImgProcTest, Convolve) {
    Matrix src;
    Matrix dst;
    Matrix kernel;
    double exp[3][3] = {0};

    // ============
    // Test 1
    // ============
    double vals[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };

    src = cvl_mat_create_from(vals, 3, 3);

    kernel = cvl_mat_create(3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            kernel.map[i][j] = 1.0 / 9.0;
        }
    }

    dst = cvl_convolve_new(&src, &kernel);

    exp[0][0] = 12.0 / 9.0; exp[0][1] = 21.0 / 9.0; exp[0][2] = 16.0 / 9.0;
    exp[1][0] = 27.0 / 9.0; exp[1][1] = 45.0 / 9.0; exp[1][2] = 33.0 / 9.0;
    exp[2][0] = 24.0 / 9.0; exp[2][1] = 39.0 / 9.0; exp[2][2] = 28.0 / 9.0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(dst.map[i][j], exp[i][j], 1e-6);
        }
    }

    // ============
    // Test 2
    // ============
    kernel = cvl_mat_create(2, 2);
    kernel.map[0][0] = 1 / 4.0; kernel.map[0][1] = 0 / 4.0;
    kernel.map[1][0] = 0 / 4.0; kernel.map[1][1] = 1 / 4.0;

    dst = cvl_convolve_new(&src, &kernel);

    exp[0][0] = 1 / 4.0; exp[0][1] = 2 / 4.0; exp[0][2] = 3 / 4.0;
    exp[1][0] = 4 / 4.0; exp[1][1] = 6 / 4.0; exp[1][2] = 8 / 4.0;
    exp[2][0] = 7 / 4.0; exp[2][1] = 12 / 4.0; exp[2][2] = 14 / 4.0;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(dst.map[i][j], exp[i][j], 1e-6);
        }
    }

    // ============
    // Test 3 - Asymetric Kernel
    // ============

    kernel = cvl_mat_create(3, 3);
    kernel.map[0][0] = -1; kernel.map[0][1] = 0; kernel.map[0][2] = 1;
    kernel.map[1][0] = -2; kernel.map[1][1] = 0; kernel.map[1][2] = 2;
    kernel.map[2][0] = -1; kernel.map[2][1] = 0; kernel.map[2][2] = 1;

    dst = cvl_convolve_new(&src, &kernel);

    exp[0][0] = -9; exp[0][1] = -6; exp[0][2] = 9;
    exp[1][0] = -20; exp[1][1] = -8 ; exp[1][2] = 20;
    exp[2][0] = -21; exp[2][1] = -6 ; exp[2][2] = 21;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(dst.map[i][j], exp[i][j], 1e-6);
        }
    }

    cvl_mat_free(src);
    cvl_mat_free(kernel);
    cvl_mat_free(dst);
}

TEST(ImgProcTest, Outer) {
    {
        double vals[] = {1, 4, 6, 4, 1};

        Matrix a = cvl_mat_create_from(vals, 5, 1);
        Matrix b = cvl_mat_create_from(vals, 5, 1);
        Matrix dst = cvl_outer_new(&a, &b);

        double exp[5][5] = {
            { 1,  4,  6,  4,  1},
            { 4, 16, 24, 16,  4},
            { 6, 24, 36, 24,  6},
            { 4, 16, 24, 16,  4},
            { 1,  4,  6,  4,  1},
        };

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                EXPECT_EQ(dst.map[i][j], exp[i][j]);
            }
        }

        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(dst);
    }

    {
        double avals[] = { 1,  4,  6,  4,  1};
        double bvals[] = {-1, -2,  0,  2,  1};

        Matrix a = cvl_mat_create_from(avals, 5, 1);
        Matrix b = cvl_mat_create_from(bvals, 5, 1);
        Matrix dst = cvl_outer_new(&a, &b);

        double exp[5][5] = {
            {-1,  -2,   0,   2,   1},
            {-4,  -8,   0,   8,   4},
            {-6, -12,   0,  12,   6},
            {-4,  -8,   0,   8,   4},
            {-1,  -2,   0,   2,   1},
        };

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                EXPECT_EQ(dst.map[i][j], exp[i][j]);
            }
        }

        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(dst);
    }
    {
        double avals[] = { 1,  4,  6,  4,  1};
        double bvals[] = {-1,  0,  2,  0, -1};

        Matrix a = cvl_mat_create_from(avals, 5, 1);
        Matrix b = cvl_mat_create_from(bvals, 5, 1);
        Matrix dst = cvl_outer_new(&a, &b);

        double exp[5][5] = {
            {-1,  0,  2,  0, -1},
            {-4,  0,  8,  0, -4},
            {-6,  0, 12,  0, -6},
            {-4,  0,  8,  0, -4},
            {-1,  0,  2,  0, -1},
        };

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                EXPECT_EQ(dst.map[i][j], exp[i][j]);
            }
        }

        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(dst);
    }
    {
        double avals[] = {1,  4,  6,  4,  1};
        double bvals[] = {1, -4,  6, -4,  1};

        Matrix a = cvl_mat_create_from(avals, 5, 1);
        Matrix b = cvl_mat_create_from(bvals, 5, 1);
        Matrix dst = cvl_outer_new(&a, &b);

        double exp[5][5] = {
            {1,  -4,   6,  -4,   1},
            {4, -16,  24, -16,   4},
            {6, -24,  36, -24,   6},
            {4, -16,  24, -16,   4},
            {1,  -4,   6,  -4,   1},
        };

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                EXPECT_EQ(dst.map[i][j], exp[i][j]);
            }
        }

        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(dst);
    }
    {
        double avals[] = {-1,  2,  0, -2,  1};
        double bvals[] = {-1, -2,  0,  2,  1};

        Matrix a = cvl_mat_create_from(avals, 5, 1);
        Matrix b = cvl_mat_create_from(bvals, 5, 1);
        Matrix dst = cvl_outer_new(&a, &b);

        double exp[5][5] = {
            { 1,  2,  0, -2, -1},
            {-2, -4,  0,  4,  2},
            { 0,  0,  0,  0,  0},
            { 2,  4,  0, -4, -2},
            {-1, -2,  0,  2,  1},
        };

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                EXPECT_EQ(dst.map[i][j], exp[i][j]);
            }
        }

        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(dst);
    }
}

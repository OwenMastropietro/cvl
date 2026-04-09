#include <cvl/cvl.h>
#include <gtest/gtest.h>

TEST(ImgProcTest, Correlate) {
    // Test 1
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double kval = 1.0 / 9.0;

        double exp[3][3] = {
            {12.0 / 9.0, 21.0 / 9.0, 16.0 / 9.0},
            {27.0 / 9.0, 45.0 / 9.0, 33.0 / 9.0},
            {24.0 / 9.0, 39.0 / 9.0, 28.0 / 9.0},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat kernel = cvl_mat_create_fill(3, 3, 1, CVL_FLOAT64, &kval);
        cvl_Mat dst = cvl_correlate_new(&src, &kernel);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&kernel);
        cvl_mat_free(&dst);
    }

    // Test 2
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double kvals[2][2] = {
            {1.0 / 4.0, 0.0 / 4.0},
            {0.0 / 4.0, 1.0 / 4.0},
        };

        double exp[3][3] = {
            {1.0 / 4.0, 2.0  / 4.0, 3.0  / 4.0},
            {4.0 / 4.0, 6.0  / 4.0, 8.0  / 4.0},
            {7.0 / 4.0, 12.0 / 4.0, 14.0 / 4.0},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat kernel = cvl_mat_create_from(2, 2, 1, CVL_FLOAT64, kvals);
        cvl_Mat dst = cvl_correlate_new(&src, &kernel);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&kernel);
        cvl_mat_free(&dst);
    }
}

TEST(ImgProcTest, Convolve) {
    // Test 1
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double kval = 1.0 / 9.0;

        double exp[3][3] = {
            {12.0 / 9.0, 21.0 / 9.0, 16.0 / 9.0},
            {27.0 / 9.0, 45.0 / 9.0, 33.0 / 9.0},
            {24.0 / 9.0, 39.0 / 9.0, 28.0 / 9.0},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat kernel = cvl_mat_create_fill(3, 3, 1, CVL_FLOAT64, &kval);
        cvl_Mat dst = cvl_convolve_new(&src, &kernel);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&kernel);
        cvl_mat_free(&dst);
    }

    // Test 2
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double kvals[2][2] = {
            {1.0 / 4.0, 0.0 / 4.0},
            {0.0 / 4.0, 1.0 / 4.0},
        };

        double exp[3][3] = {
            {1.0 / 4.0, 2.0  / 4.0, 3.0  / 4.0},
            {4.0 / 4.0, 6.0  / 4.0, 8.0  / 4.0},
            {7.0 / 4.0, 12.0 / 4.0, 14.0 / 4.0},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat kernel = cvl_mat_create_from(2, 2, 1, CVL_FLOAT64, kvals);
        cvl_Mat dst = cvl_convolve_new(&src, &kernel);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&kernel);
        cvl_mat_free(&dst);
    }

    // Test 3 (asymmetric kernel)
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double kvals[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1},
        };

        double exp[3][3] = {
            {-9,  -6, 9},
            {-20, -8, 20},
            {-21, -6, 21},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat kernel = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, kvals);
        cvl_Mat dst = cvl_convolve_new(&src, &kernel);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&kernel);
        cvl_mat_free(&dst);
    }
}

TEST(ImgProcTest, BlurMean) {
    // Test 1
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        double exp[3][3] = {
            {12.0 / 9.0, 21.0 / 9.0, 16.0 / 9.0},
            {27.0 / 9.0, 45.0 / 9.0, 33.0 / 9.0},
            {24.0 / 9.0, 39.0 / 9.0, 28.0 / 9.0},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat dst = cvl_blur_mean_new(&src, 3);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }
}

TEST(ImgProcTest, BlurMedian) {
    // Test 1
    {
        double vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        // double exp[3][3] = { // oops, this is for BOARDER_CONSTANT
        //     {0, 2, 0},
        //     {2, 5 ,0},
        //     {0, 5, 0},
        // };
        double exp[3][3] = {
            {2, 3, 3},
            {4, 5, 6},
            {7, 7, 8},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
        cvl_Mat dst = cvl_blur_median_new(&src, 3);

        for (int i = 0; i < dst.height; ++i) {
            double *row = cvl_row_f64(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_EQ(row[j], expected);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }
}

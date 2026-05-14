#include <cvl/cvl.h>
#include <gtest/gtest.h>


static void assert_mat_equal_u8(cvl_Mat *a, cvl_Mat *b) {
    ASSERT_EQ(a->height, b->height);
    ASSERT_EQ(a->width, b->width);
    ASSERT_EQ(a->channels, b->channels);

    for (int i = 0; i < a->height; ++i) {
        uint8_t *row_a = cvl_row_u8(a, i);
        uint8_t *row_b = cvl_row_u8(b, i);

        for (int j = 0; j < a->width; ++j) {
            EXPECT_EQ(row_a[j], row_b[j]);
        }
    }
}

static void assert_mat_equal_f64(cvl_Mat *a, cvl_Mat *b) {
    ASSERT_EQ(a->height, b->height);
    ASSERT_EQ(a->width, b->width);
    ASSERT_EQ(a->channels, b->channels);

    for (int i = 0; i < a->height; ++i) {
        double *row_a = cvl_row_f64(a, i);
        double *row_b = cvl_row_f64(b, i);

        for (int j = 0; j < a->width; ++j) {
            EXPECT_EQ(row_a[j], row_b[j]);
        }
    }
}

// ==============================================
// U8 Threshold Tests
// ==============================================

TEST(ThresholdU8, Binary) {
    int thresh = 5;
    int maxval = 255;

    uint8_t vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    uint8_t exp_vals[3][3] = {
        {0,   0,   0},
        {0,   0,   255},
        {255, 255, 255},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_BINARY);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_UINT8, exp_vals);

    assert_mat_equal_u8(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdU8, BinaryInv) {
    int thresh = 5;
    int maxval = 255;

    uint8_t vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    uint8_t exp_vals[3][3] = {
        {255, 255, 255},
        {255, 255, 0},
        {0,   0,   0},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_BINARY_INV);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_UINT8, exp_vals);

    assert_mat_equal_u8(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdU8, Trunc) {
    int thresh = 5;
    int maxval = 255;

    uint8_t vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    uint8_t exp_vals[3][3] = {
        {1, 2, 3},
        {4, 5, 5},
        {5, 5, 5},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TRUNC);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_UINT8, exp_vals);

    assert_mat_equal_u8(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdU8, ToZero) {
    int thresh = 5;
    int maxval = 255;

    uint8_t vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    uint8_t exp_vals[3][3] = {
        {0, 0, 0},
        {0, 0, 6},
        {7, 8, 9},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TOZERO);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_UINT8, exp_vals);

    assert_mat_equal_u8(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdU8, ToZeroInv) {
    int thresh = 5;
    int maxval = 255;

    uint8_t vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    uint8_t exp_vals[3][3] = {
        {1, 2, 3},
        {4, 5, 0},
        {0, 0, 0},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TOZERO_INV);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_UINT8, exp_vals);

    assert_mat_equal_u8(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

// ==============================================
// F64 Threshold Tests
// ==============================================

TEST(ThresholdF64, Binary) {
    int thresh = 5;
    int maxval = 255;

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    double exp_vals[3][3] = {
        {0,   0,   0},
        {0,   0,   255},
        {255, 255, 255},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_BINARY);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, exp_vals);

    assert_mat_equal_f64(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdF64, BinaryInv) {
    int thresh = 5;
    int maxval = 255;

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    double exp_vals[3][3] = {
        {255, 255, 255},
        {255, 255, 0},
        {0,   0,   0},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_BINARY_INV);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, exp_vals);

    assert_mat_equal_f64(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdF64, Trunc) {
    int thresh = 5;
    int maxval = 255;

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    double exp_vals[3][3] = {
        {1, 2, 3},
        {4, 5, 5},
        {5, 5, 5},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TRUNC);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, exp_vals);

    assert_mat_equal_f64(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdF64, ToZero) {
    int thresh = 5;
    int maxval = 255;

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    double exp_vals[3][3] = {
        {0, 0, 0},
        {0, 0, 6},
        {7, 8, 9},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TOZERO);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, exp_vals);

    assert_mat_equal_f64(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

TEST(ThresholdF64, ToZeroInv) {
    int thresh = 5;
    int maxval = 255;

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };
    double exp_vals[3][3] = {
        {1, 2, 3},
        {4, 5, 0},
        {0, 0, 0},
    };

    cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    cvl_Mat res = cvl_threshold_new(&src, thresh, maxval, CVL_THRESH_TOZERO_INV);
    cvl_Mat exp = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, exp_vals);

    assert_mat_equal_f64(&res, &exp);

    cvl_mat_free(&src);
    cvl_mat_free(&res);
    cvl_mat_free(&exp);
}

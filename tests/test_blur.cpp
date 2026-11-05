/**
 * @file test_blur.cpp
 * @brief Tests for box (mean), Gaussian and median blur.
 */

#include <cvl/cvl.h>
#include <gtest/gtest.h>
#include <cmath>

/** cvl_blur_mean() */

TEST(MeanBlurTest, MatchesExpectedOutput) {
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
            double *row = (double *)cvl_mat_row(&dst, i);

            for (int j = 0; j < dst.width; ++j) {
                double expected = exp[i][j];
                EXPECT_NEAR(row[j], expected, 1e-6);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }
}

/** cvl_blur_gauss() */

class GaussianBlurTest : public ::testing::Test {
protected:
    cvl_Mat src = {};
    cvl_Mat dst = {};

    void SetUp() override {
        src = cvl_mat_create(16, 16, 1, CVL_UINT8);
        dst = cvl_mat_create(16, 16, 1, CVL_UINT8);

        ASSERT_NE(src.data, nullptr);
        ASSERT_NE(dst.data, nullptr);
    }

    void TearDown() override {
        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }
};

TEST_F(GaussianBlurTest, RejectsNegativeKernelSize) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, -1, 1.0),
        CVL_ERR_RANGE
    );
}

TEST_F(GaussianBlurTest, RejectsEvenKernelSize) {
    for (size_t ksize = 2; ksize < 10; ksize+=2) {
        EXPECT_EQ(
            cvl_blur_gauss(&src, &dst, ksize, 1.0),
            CVL_ERR_RANGE
        );
    }
}

TEST_F(GaussianBlurTest, RejectsZeroKernelAndSigma) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 0, 0.0),
        CVL_ERR_RANGE
    );
}

TEST_F(GaussianBlurTest, RejectsNegativeSigma) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, -1.0),
        CVL_ERR_RANGE
    );
}

TEST_F(GaussianBlurTest, RejectsInfiniteSigma) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, INFINITY),
        CVL_ERR_RANGE
    );
}

TEST_F(GaussianBlurTest, RejectsNanSigma) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, NAN),
        CVL_ERR_RANGE
    );
}

TEST_F(GaussianBlurTest, RejectsNullSource) {
    EXPECT_EQ(
        cvl_blur_gauss(nullptr, &dst, 3, 1.0),
        CVL_ERR_NULL
    );
}

TEST_F(GaussianBlurTest, RejectsNullDestination) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, nullptr, 3, 1.0),
        CVL_ERR_NULL
    );
}

TEST_F(GaussianBlurTest, RejectsDifferentDimensions) {
    cvl_Mat other = cvl_mat_create(8, 16, 1, CVL_UINT8);

    EXPECT_EQ(
        cvl_blur_gauss(&src, &other, 3, 1.0),
        CVL_ERR_STATE
    );

    cvl_mat_free(&other);
}

TEST_F(GaussianBlurTest, RejectsDifferentChannels) {
    cvl_Mat other = cvl_mat_create(16, 16, 3, CVL_UINT8);

    EXPECT_EQ(
        cvl_blur_gauss(&src, &other, 3, 1.0),
        CVL_ERR_STATE
    );

    cvl_mat_free(&other);
}

TEST_F(GaussianBlurTest, SupportsU8) {
    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, 1.0),
        CVL_OK
    );
}

TEST_F(GaussianBlurTest, SupportsF64) {
    cvl_mat_free(&src);
    cvl_mat_free(&dst);

    src = cvl_mat_create(16, 16, 1, CVL_FLOAT64);
    dst = cvl_mat_create(16, 16, 1, CVL_FLOAT64);

    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, 1.0),
        CVL_OK
    );
}

TEST_F(GaussianBlurTest, SupportsDifferentDestinationDepth) {
    cvl_mat_free(&dst);
    dst = cvl_mat_create(16, 16, 1, CVL_FLOAT64);

    EXPECT_EQ(
        cvl_blur_gauss(&src, &dst, 3, 1.0),
        CVL_OK
    );
}

TEST_F(GaussianBlurTest, SupportsDerivedSigma) {
    int ksize = 5;
    double sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8;

    cvl_Mat a = cvl_blur_gauss_new(&src, ksize, 0.0);   // sigma derived
    cvl_Mat b = cvl_blur_gauss_new(&src, ksize, sigma); // sigma directly

    for(int y = 0; y < a.height; ++y) {
        for (int x = 0; x < a.width; ++x) {
            EXPECT_NEAR(CVL_AT_U8(&a, y, x, 0), CVL_AT_U8(&b, y, x, 0), 1e-6);
        }
    }

    cvl_mat_free(&b);
    cvl_mat_free(&a);
}

TEST_F(GaussianBlurTest, SupportsDerivedKernelSize) {
    double sigma = 1.0;
    int ksize = 2 * ceil(3.0 * sigma) + 1;

    cvl_Mat a = cvl_blur_gauss_new(&src, 0, sigma);     // ksize derived
    cvl_Mat b = cvl_blur_gauss_new(&src, ksize, sigma); // ksize directly

    for(int y = 0; y < a.height; ++y) {
        for (int x = 0; x < a.width; ++x) {
            EXPECT_NEAR(CVL_AT_U8(&a, y, x, 0), CVL_AT_U8(&b, y, x, 0), 1e-6);
        }
    }

    cvl_mat_free(&b);
    cvl_mat_free(&a);
}

TEST_F(GaussianBlurTest, PreservesConstantImage) {
    const uint8_t v = 128;

    cvl_mat_free(&src);
    src = cvl_mat_create_fill(16, 16, 1, CVL_UINT8, &v);

    ASSERT_EQ(
        cvl_blur_gauss(&src, &dst, 3, 1.0),
        CVL_OK
    );

    for(int y = 0; y < dst.height; ++y) {
        for (int x = 0; x < dst.width; ++x) {
            EXPECT_NEAR(CVL_AT_U8(&dst, y, x, 0), v, 1);
        }
    }
}

TEST_F(GaussianBlurTest, ProducesGaussianImpulseResponse) {
    CVL_AT_U8(&src, 8, 8, 0) = 255;

    cvl_blur_gauss(&src, &dst, 5, 1.0);

    // Center should have highest response.
    EXPECT_GT(CVL_AT_U8(&dst, 8, 8, 0), CVL_AT_U8(&dst, 8, 7, 0));

    // Response should be symmetric.
    EXPECT_NEAR(CVL_AT_U8(&dst, 8, 7, 0), CVL_AT_U8(&dst, 8, 9, 0), 1);
    EXPECT_NEAR(CVL_AT_U8(&dst, 7, 8, 0), CVL_AT_U8(&dst, 9, 8, 0), 1);
    EXPECT_NEAR(CVL_AT_U8(&dst, 7, 7, 0), CVL_AT_U8(&dst, 9, 9, 0), 1);
}

TEST_F(GaussianBlurTest, MatchesExpectedOutput) {
    cvl_mat_free(&src);
    cvl_mat_free(&dst);

    double vals[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    src = cvl_mat_create_from(3, 3, 1, CVL_FLOAT64, vals);
    dst = cvl_blur_gauss_new(&src, 3, 1.0);

    ASSERT_NE(src.data, nullptr);
    ASSERT_NE(dst.data, nullptr);

    const double center = 1.0;
    const double side = exp(-0.5);
    const double norm = center + 2.0 * side;
    const double k0 = side / norm;
    const double k1 = center / norm;

    auto at = [](double m[3][3], int y, int x) {
        if (y < 0) y = 0;
        if (y > 2) y = 2;
        if (x < 0) x = 0;
        if (x > 2) x = 2;
        return m[y][x];
    };

    double horizontal[3][3];
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            horizontal[y][x] = k0 * at(vals, y, x - 1) + k1 * at(vals, y, x) + k0 * at(vals, y, x + 1);
        }
    }
    double expected[3][3];
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            expected[y][x] = k0 * at(horizontal, y - 1, x) + k1 * at(horizontal, y, x) + k0 * at(horizontal, y + 1, x);
        }
    }

    for (int y = 0; y < 3; ++y) {
        double *row = (double *)cvl_mat_row(&dst, y);
        for (int x = 0; x < 3; ++x) {
            EXPECT_NEAR(row[x], expected[y][x], 1e-9);
        }
    }
}

/** cvl_blur_median() */

TEST(MedianBlurTest, MatchesExpectedOutput) {
    // Test: Replicate Border
    {
        uint8_t vals[3][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
        };

        // uint8_t exp[3][3] = { // oops, this is for BOARDER_CONSTANT
        //     {0, 2, 0},
        //     {2, 5 ,0},
        //     {0, 5, 0},
        // };
        uint8_t exp[3][3] = {
            {2, 3, 3},
            {4, 5, 6},
            {7, 7, 8},
        };

        cvl_Mat src = cvl_mat_create_from(3, 3, 1, CVL_UINT8, vals);
        cvl_Mat dst = cvl_blur_median_new(&src, 3);

        ASSERT_NE(dst.data, nullptr);
        EXPECT_EQ(memcmp(dst.data, exp, sizeof(exp)), 0);

        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }

    // Test: RGB
    {
        uint8_t vals[3][3][3] = {{
            {10, 20, 30},
            {10, 20, 30},
            {10, 20, 30},
        }, {
            {10, 20, 30},
            {255, 20, 30},
            {10, 20, 30},
        }, {
            {10, 20, 30},
            {10, 20, 30},
            {10, 20, 30},
        }};

        cvl_Mat src = cvl_mat_create_from(3, 3, 3, CVL_UINT8, vals);
        cvl_Mat dst = cvl_blur_median_new(&src, 3);

        ASSERT_NE(dst.data, nullptr);

        for (int y = 0; y < dst.height; ++y) {
            for (int x = 0; x < dst.width; ++x) {
                EXPECT_EQ(CVL_AT_U8(&dst, y, x, 0), 10);
                EXPECT_EQ(CVL_AT_U8(&dst, y, x, 1), 20);
                EXPECT_EQ(CVL_AT_U8(&dst, y, x, 2), 30);
            }
        }

        cvl_mat_free(&src);
        cvl_mat_free(&dst);
    }
}

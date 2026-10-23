#include <cvl/cvl.h>
#include <gtest/gtest.h>
#include <cstring>
#include <float.h>
#include <stdint.h>

// ================
// cvl_mat_create
// ================

struct MatCreateCase {
    int height;
    int width;
    int channels;
    cvl_depth_t depth;
};

class MatCreateTest : public ::testing::TestWithParam<MatCreateCase> {};

TEST_P(MatCreateTest, MatCreate) {
    MatCreateCase tc = GetParam();

    size_t elem_size = cvl_elem_size(tc.depth);
    ASSERT_GT(elem_size, 0u);

    size_t tc_stride = tc.width * tc.channels * elem_size;

    cvl_Mat mat = cvl_mat_create(tc.height, tc.width, tc.channels, tc.depth);

    ASSERT_EQ(mat.height, tc.height);
    ASSERT_EQ(mat.width, tc.width);
    ASSERT_EQ(mat.channels, tc.channels);
    ASSERT_EQ(mat.depth, tc.depth);
    ASSERT_EQ(mat.stride, tc_stride);
    ASSERT_NE(mat.data, nullptr);

    for (int i = 0; i < tc.height; ++i) {
        for (int j = 0; j < tc.width; ++j) {
            for (int c = 0; c < tc.channels; ++c) {
                if (tc.depth == CVL_UINT8) {
                        const uint8_t *data = static_cast<const uint8_t *>(mat.data);
                        const uint8_t v = data[i * tc_stride + j * tc.channels + c];
                        EXPECT_EQ(v, 0);
                } else if (tc.depth == CVL_FLOAT64) {
                        const double *data = static_cast<const double *>(mat.data);
                        const double v = data[i * tc_stride + j * tc.channels + c];
                        EXPECT_EQ(v, 0);
                } else {
                    ASSERT_FALSE(true);
                }
            }
        }
    }

    cvl_mat_free(&mat);

    ASSERT_EQ(mat.data, nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    MatTests,
    MatCreateTest,
    ::testing::Values(
        MatCreateCase{3, 3, 3, CVL_UINT8},
        MatCreateCase{1, 1, 1, CVL_UINT8},
        MatCreateCase{10, 10, 1, CVL_UINT8},
        MatCreateCase{5, 7, 4, CVL_UINT8},
        
        MatCreateCase{3, 3, 3, CVL_FLOAT64},
        MatCreateCase{1, 1, 1, CVL_FLOAT64},
        MatCreateCase{10, 10, 1, CVL_FLOAT64},
        MatCreateCase{5, 7, 4, CVL_FLOAT64}
    )
);

// ================
// cvl_mat_create_fill
// ================

TEST(MatCreateFillTest, MatCreateFill) {
    cvl_Mat mat;
    uint8_t *data;

    // U8
    {
        uint8_t fv;

        // Test 1: fill value = 0
        fv = 0;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_UINT8, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_UINT8);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    uint8_t v = data[i * mat.stride + j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 2: fill value = 128
        fv = 128;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_UINT8, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_UINT8);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    uint8_t v = data[i * mat.stride + j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 3: fill value = 255
        fv = 255;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_UINT8, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_UINT8);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    uint8_t v = data[i * mat.stride + j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }
    }

    // F64
    {
        double fv;

        // Test 1: fill value = 0
        fv = 0.0;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_FLOAT64, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const double *row = (double *)(data + i * mat.stride);
        
            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    double v = row[j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 2: fill value = 128
        fv = 128.0;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_FLOAT64, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const double *row = (double *)(data + i * mat.stride);

            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    double v = row[j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 3: fill value = 255
        fv = 255.0;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_FLOAT64, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const double *row = (double *)(data + i * mat.stride);

            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    double v = row[j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 4: fill value = max f64
        fv = DBL_MAX;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_FLOAT64, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const double *row = (double *)(data + i * mat.stride);

            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    double v = row[j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }

        // Test 5: fill value = min f64
        fv = DBL_MIN;
        mat = cvl_mat_create_fill(3, 3, 3, CVL_FLOAT64, &fv);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const double *row = (double *)(data + i * mat.stride);

            for (int j = 0; j < mat.width; ++j) {
                for (int c = 0; c < mat.channels; ++c) {
                    double v = row[j * mat.channels + c];
                    EXPECT_EQ(v, fv);
                }
            }
        }
    }

    cvl_mat_free(&mat);
    ASSERT_EQ(mat.data, nullptr);
}

// ================
// cvl_mat_create_from
// ================

TEST(MatCreateFromTest, MatCreateFrom) {
    cvl_Mat mat;
    uint8_t *data;

    // U8
    {
        const uint8_t values_1[2][3] = {
            {0, 100, 128},
            {150, 200, 255},
        };

        mat = cvl_mat_create_from(2, 3, 1, CVL_UINT8, values_1);

        ASSERT_EQ(mat.height, 2);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 1);
        ASSERT_EQ(mat.depth, CVL_UINT8);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const uint8_t *row = cvl_mat_row(&mat, i);

            for (int j = 0; j < mat.width; ++j) {
                    EXPECT_EQ(row[j], values_1[i][j]);
            }
        }

        // ------------

        const uint8_t values_2[] = {
            255, 0,   0,   // red
            0,   255, 0,   // green
            0,   0,   255, // blue
            0,   0,   0,   // black
            128, 128, 128, // grey
            255, 255, 255  // white
        };

        mat = cvl_mat_create_from(3, 2, 3, CVL_UINT8, values_2);

        ASSERT_EQ(mat.height, 3);
        ASSERT_EQ(mat.width, 2);
        ASSERT_EQ(mat.channels, 3);
        ASSERT_EQ(mat.depth, CVL_UINT8);
        ASSERT_NE(mat.data, nullptr);

        data = static_cast<uint8_t *>(mat.data);

        for (int i = 0; i < mat.height; ++i) {
            const uint8_t *row = cvl_mat_row(&mat, i);

            for (int j = 0; j < mat.width; ++j) {
                uint8_t r = row[j * mat.channels + 0];
                uint8_t g = row[j * mat.channels + 1];
                uint8_t b = row[j * mat.channels + 2];

                int exp_idx = (i * mat.width + j);

                EXPECT_EQ(r, values_2[exp_idx * mat.channels + 0]);
                EXPECT_EQ(g, values_2[exp_idx * mat.channels + 1]);
                EXPECT_EQ(b, values_2[exp_idx * mat.channels + 2]);
            }
        }
    }

    // F64
    {
        const double values_3[] = {
            0.0,   0.25,  0.5,
            0.75,  1.0,   1.5
        };

        mat = cvl_mat_create_from(2, 3, 1, CVL_FLOAT64, values_3);

        ASSERT_EQ(mat.height, 2);
        ASSERT_EQ(mat.width, 3);
        ASSERT_EQ(mat.channels, 1);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        for (int i = 0; i < mat.height; ++i) {
            double *row = (double *)cvl_mat_row(&mat, i);

            for (int j = 0; j < mat.width; ++j) {
                double expected = values_3[i * mat.width + j];
                EXPECT_DOUBLE_EQ(row[j], expected); // todo: near?
            }
        }

        const double values_4[] = {
            8.0,    16.0,
            32.0,   64.0,
            128.0,  512.0,
            1024.0, 2048.0
        };

        mat = cvl_mat_create_from(4, 2, 1, CVL_FLOAT64, values_4);

        ASSERT_EQ(mat.height, 4);
        ASSERT_EQ(mat.width, 2);
        ASSERT_EQ(mat.channels, 1);
        ASSERT_EQ(mat.depth, CVL_FLOAT64);
        ASSERT_NE(mat.data, nullptr);

        for (int i = 0; i < mat.height; ++i) {
            double* row = (double *)cvl_mat_row(&mat, i);

            for (int j = 0; j < mat.width; ++j) {
                double expected = values_4[i * mat.width + j];
                EXPECT_DOUBLE_EQ(row[j], expected); // todo: near?
            }
        }
    }

    cvl_mat_free(&mat);
    ASSERT_EQ(mat.data, nullptr);
}

// ================
// cvl_mat_free
// ================

TEST(MatTest, MatFree) {
    cvl_Mat mat = cvl_mat_create(3, 3, 3, CVL_UINT8);

    ASSERT_TRUE(mat.data != NULL);

    cvl_mat_free(&mat);

    ASSERT_EQ(mat.data, nullptr);
}

static void expect_empty(const cvl_Mat &mat) {
    EXPECT_EQ(mat.data, nullptr);
    EXPECT_EQ(mat.height, 0);
    EXPECT_EQ(mat.width, 0);
    EXPECT_EQ(mat.channels, 0);
    EXPECT_EQ(mat.stride, 0u);
}

TEST(MatCreateInvalid, NonPositiveAndOverflow) {
    expect_empty(cvl_mat_create(0, 2, 1, CVL_UINT8));
    expect_empty(cvl_mat_create(2, 0, 1, CVL_UINT8));
    expect_empty(cvl_mat_create(2, 2, 0, CVL_UINT8));
    expect_empty(cvl_mat_create(-1, 2, 1, CVL_UINT8));
    expect_empty(cvl_mat_create(2, -1, 1, CVL_UINT8));
    expect_empty(cvl_mat_create(2, 2, -1, CVL_UINT8));

    // width * channels * elem_size does not fit in size_t.
    expect_empty(cvl_mat_create(1, INT32_MAX, INT32_MAX, CVL_FLOAT64));
    // stride fits, height * stride does not.
    expect_empty(cvl_mat_create(INT32_MAX, INT32_MAX, 2, CVL_UINT8));

    uint8_t fill = 7;
    expect_empty(cvl_mat_create_fill(0, 2, 1, CVL_UINT8, &fill));
    expect_empty(cvl_mat_create_fill(2, 2, 1, CVL_UINT8, nullptr));

    uint8_t pixel = 9;
    expect_empty(cvl_mat_create_from(0, 2, 1, CVL_UINT8, &pixel));
    expect_empty(cvl_mat_create_from(1, 1, 1, CVL_UINT8, nullptr));
    expect_empty(cvl_mat_create_from(INT32_MAX, INT32_MAX, 2, CVL_UINT8, &pixel));
}

TEST(MatCopy, CopiesPixelsAndRejectsEmpty) {
    uint8_t values[] = {1, 2, 3, 4};
    cvl_Mat src = cvl_mat_create_from(2, 2, 1, CVL_UINT8, values);
    ASSERT_NE(src.data, nullptr);

    cvl_Mat dst = cvl_mat_copy(&src);
    ASSERT_NE(dst.data, nullptr);
    EXPECT_NE(dst.data, src.data);
    EXPECT_EQ(dst.height, 2);
    EXPECT_EQ(dst.width, 2);
    EXPECT_EQ(dst.channels, 1);
    EXPECT_EQ(dst.stride, src.stride);
    EXPECT_EQ(memcmp(dst.data, values, 4), 0);

    static_cast<uint8_t *>(dst.data)[0] = 9;
    EXPECT_EQ(static_cast<uint8_t *>(src.data)[0], 1);

    cvl_Mat empty = {0};
    expect_empty(cvl_mat_copy(&empty));
    expect_empty(cvl_mat_copy(nullptr));

    src.height = 0;
    expect_empty(cvl_mat_copy(&src));
    src.height = 2;

    cvl_mat_free(&src);
    cvl_mat_free(&dst);
}

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
            EXPECT_EQ(dst.map[i][j], exp[i][j]);
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
            EXPECT_EQ(dst.map[i][j], exp[i][j]);
        }
    }

    cvl_mat_free(src);
    cvl_mat_free(kernel);
    cvl_mat_free(dst);
}

#include <cvl/cvl.h>
#include <gtest/gtest.h>

// cvl_mat_create

TEST(MatTest, Create) {
    // Define Test Cases.
    struct { int h; int w; } test_cases[] = {
        { 0, 3 },
        { 3, 0 },
        { 2, 3 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix m = cvl_mat_create(tc.h, tc.w);

        if (tc.h == 0 || tc.w == 0) {
            EXPECT_EQ(m.height, 0);
            EXPECT_EQ(m.width,  0);
            EXPECT_EQ(m.map,    nullptr);
        } else {
            EXPECT_EQ(m.height, tc.h);
            EXPECT_EQ(m.width,  tc.w);
            for (int i = 0; i < m.height; ++i) {
                for (int j = 0; j < m.width; ++j) {
                    EXPECT_DOUBLE_EQ(m.map[i][j], 0.0);
                }
            }
            cvl_mat_free(m);
        }
    }
}

// cvl_mat_create_fill

TEST(MatTest, CreateFill) {
    // Test Cases.
    struct { int h; int w; double fv; } test_cases[] = {
        { 0,  3,  0.0  },
        { 3,  0,  0.0  },
        { 2,  3,  42.0 },
        { 3,  2, -69.0 },
        { 3,  3,  3.14 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix m = cvl_mat_create_fill(tc.h, tc.w, tc.fv);

        if (tc.h == 0 || tc.w == 0) {
            EXPECT_EQ(m.height, 0);
            EXPECT_EQ(m.width,  0);
            EXPECT_EQ(m.map,    nullptr);
        } else {
            EXPECT_EQ(m.height, tc.h);
            EXPECT_EQ(m.width,  tc.w);
            for (int i = 0; i < m.height; ++i) {
                for (int j = 0; j < m.width; ++j) {
                    EXPECT_DOUBLE_EQ(m.map[i][j], tc.fv);
                }
            }
            cvl_mat_free(m);
        }
    }
}

// cvl_mat_create_from

TEST(MatTest, CreateFrom) {
    static double t1[] = {1, 2, 3, 4, 5, 6};
    static double t2[] = {-1, -2, -3, -4, -5, -6};
    static double t3[] = {3.14, 2.71, 1.41, 0.0, -1.0, -3.14, 42.0, -69.0, 100.0};

    // Test Cases.
    struct { int h; int w; double *entry; } test_cases[] = {
        { 0, 3, nullptr },
        { 3, 0, nullptr },
        { 2, 3, t1 },
        { 3, 2, t2 },
        { 3, 3, t3 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix m = cvl_mat_create_from(tc.entry, tc.h, tc.w);

        if (tc.h == 0 || tc.w == 0) {
            EXPECT_EQ(m.height, 0);
            EXPECT_EQ(m.width,  0);
            EXPECT_EQ(m.map,    nullptr);
        } else {
            EXPECT_EQ(m.height, tc.h);
            EXPECT_EQ(m.width,  tc.w);
            for (int i = 0; i < m.height; ++i) {
                for (int j = 0; j < m.width; ++j) {
                    EXPECT_DOUBLE_EQ(m.map[i][j], tc.entry[i * tc.w + j]);
                }
            }
            cvl_mat_free(m);
        }
    }
}

// todo: cvl_mat_copy
TEST(MatTest, Copy) {
    // Test Cases.
    static double t1[] = {1, 2, 3, 4, 5, 6};
    static double t2[] = {-1, -2, -3, -4, -5, -6};
    static double t3[] = {3.14, 2.71, 1.41, 0.0, -1.0, -3.14, 42.0, -69.0, 100.0};
    struct { int h; int w; double *entry; } test_cases[] = {
        { 2, 3, t1 },
        { 3, 2, t2 },
        { 3, 3, t3 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix src = cvl_mat_create_from(tc.entry, tc.h, tc.w);
        Matrix dst = cvl_mat_copy(&src);

        EXPECT_EQ(dst.height, tc.h);
        EXPECT_EQ(dst.width,  tc.w);
        for (int i = 0; i < dst.height; ++i) {
            for (int j = 0; j < dst.width; ++j) {
                double expected = tc.entry[i * tc.w + j];
                EXPECT_DOUBLE_EQ(dst.map[i][j], expected);

                src.map[i][j] += 1.0;
                EXPECT_DOUBLE_EQ(dst.map[i][j], expected);
                src.map[i][j] -= 1.0;

                dst.map[i][j] += 10.0;
                EXPECT_DOUBLE_EQ(src.map[i][j], expected);
                dst.map[i][j] -= 10.0;
            }

        }
        cvl_mat_free(src);
        cvl_mat_free(dst);
    }

}

/*
// todo: cvl_mat_free

TEST(MatTest, Free) {
    // todo: cvl_mat_free(Matrix *mat)

    Matrix m = cvl_mat_create(2, 3);
    cvl_mat_free(m);

    EXPECT_EQ(m.height, 0);
    EXPECT_EQ(m.width,  0);
    EXPECT_EQ(m.map,    nullptr);
}
*/

// todo: cvl_mat_add
TEST(MatTest, Add) {
    double v1[] = {1, 2, 3};
    double v2[] = {4, 5, 6};
    double v3[] = {1, 2, 3, 4, 5, 6};
    double v4[] = {-1, -2, -3, -4, -5, -6};
    double v5[] = {3.14, 2.71, 1.41};
    double v6[] = {0.0, -1.0, -3.14};

    // Test Cases.
    struct { int h; int w; double *a; double *b; } test_cases[] = {
        { 1, 3, v1, v2 },
        { 2, 3, v3, v4 },
        { 3, 1, v5, v6 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix a = cvl_mat_create_from(tc.a, tc.h, tc.w);
        Matrix b = cvl_mat_create_from(tc.b, tc.h, tc.w);
        Matrix res = cvl_mat_add_new(&a, &b);

        ASSERT_EQ(res.height, tc.h);
        ASSERT_EQ(res.width,  tc.w);

        for (int i = 0; i < res.height; ++i) {
            for (int j = 0; j < res.width; ++j) {
                double expected = tc.a[i * tc.w + j] + tc.b[i * tc.w + j];
                EXPECT_DOUBLE_EQ(res.map[i][j], expected);
            }
        }
        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(res);
    }
}

// todo: cvl_mat_sub
TEST(MatTest, Sub) {
    double v1[] = {1, 2, 3};
    double v2[] = {4, 5, 6};
    double v3[] = {1, 2, 3, 4, 5, 6};
    double v4[] = {-1, -2, -3, -4, -5, -6};
    double v5[] = {3.14, 2.71, 1.41};
    double v6[] = {0.0, -1.0, -3.14};

    // Test Cases.
    struct { int h; int w; double *a; double *b; } test_cases[] = {
        { 1, 3, v1, v2 },
        { 2, 3, v3, v4 },
        { 3, 1, v5, v6 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix a = cvl_mat_create_from(tc.a, tc.h, tc.w);
        Matrix b = cvl_mat_create_from(tc.b, tc.h, tc.w);
        Matrix res = cvl_mat_sub_new(&a, &b);

        ASSERT_EQ(res.height, tc.h);
        ASSERT_EQ(res.width,  tc.w);

        for (int i = 0; i < res.height; ++i) {
            for (int j = 0; j < res.width; ++j) {
                double expected = tc.a[i * tc.w + j] - tc.b[i * tc.w + j];
                EXPECT_DOUBLE_EQ(res.map[i][j], expected);
            }
        }
        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(res);
    }
}

// cvl_mat_mul

TEST(MatTest, Mul) {
    // tc1: (2x3) * (3x2) = (2x2)
    double a1[] = {1, 2, 3, 4, 5, 6};
    double b1[] = {7, 8, 9, 10, 11, 12};

    // tc2: (3x2) * (2x2) = (3x2)
    double a2[] = {1, 2, 3, 4, 5, 6};
    double b2[] = {1, 2, 3, 4};
    
    // tc3: (2x2) * (2x2) = (2x2)
    double a3[] = {1, 2, 3, 4};
    double b3[] = {-1, -2, -3, -4};

    // tc4: (3x1) * (1x3) = (3x3)
    double a4[] = {3.14, 2.71, 1.41};
    double b4[] = {0.0, -1.0, -3.14};

    // Test Cases.
    struct {
        int h1; int w1;
        int h2; int w2;
        double *a;
        double *b;
    } test_cases[] = {
        { 2, 3, 3, 2, a1, b1 },
        { 3, 2, 2, 2, a2, b2 },
        { 2, 2, 2, 2, a3, b3 },
        { 3, 1, 1, 3, a4, b4 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix a = cvl_mat_create_from(tc.a, tc.h1, tc.w1);
        Matrix b = cvl_mat_create_from(tc.b, tc.h2, tc.w2);
        Matrix res = cvl_mat_mul_new(&a, &b);

        ASSERT_EQ(res.height, tc.h1);
        ASSERT_EQ(res.width,  tc.w2);

        for (int i = 0; i < res.height; ++i) {
            for (int j = 0; j < res.width; ++j) {
                double expected = 0.0;
                for (int k = 0; k < tc.w1; ++k) {
                    expected += a.map[i][k] * b.map[k][j];
                }
                EXPECT_DOUBLE_EQ(res.map[i][j], expected);
            }
        }
        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(res);
    }
}

// cvl_mat_scale

TEST(MatTest, Scale) {
    double v1[] = {1, 2, 3};
    double v2[] = {-1, -2, -3};
    double v3[] = {3.14, 2.71, 1.41};
    double v4[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Test Cases.
    struct { int h; int w; double *entry; double scalar; } test_cases[] = {
        { 1, 3, v1, 2.0 },
        { 1, 3, v2, -0.5 },
        { 1, 3, v3, 10.0 },
        { 3, 3, v4, 1 / 9.0 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix m = cvl_mat_create_from(tc.entry, tc.h, tc.w);
        cvl_mat_scale(&m, tc.scalar);

        ASSERT_EQ(m.height, tc.h);
        ASSERT_EQ(m.width,  tc.w);

        for (int i = 0; i < m.height; ++i) {
            for (int j = 0; j < m.width; ++j) {
                double expected = tc.entry[i * tc.w + j] * tc.scalar;
                EXPECT_DOUBLE_EQ(m.map[i][j], expected);
            }
        }
        cvl_mat_free(m);
    }
}

// cvl_mat_hadamard

TEST(MatTest, Hadamard) {
    double v1[] = {1, 2, 3};
    double v2[] = {4, 5, 6};
    double v3[] = {1, 2, 3, 4, 5, 6};
    double v4[] = {-1, -2, -3, -4, -5, -6};
    double v5[] = {3.14, 2.71, 1.41};
    double v6[] = {0.0, -1.0, -3.14};
    double v7[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Test Cases.
    struct { int h; int w; double *a; double *b; } test_cases[] = {
        { 1, 3, v1, v2 },
        { 2, 3, v3, v4 },
        { 3, 1, v5, v6 },
        { 3, 3, v7, v7 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix a = cvl_mat_create_from(tc.a, tc.h, tc.w);
        Matrix b = cvl_mat_create_from(tc.b, tc.h, tc.w);
        Matrix res = cvl_mat_hadamard_new(&a, &b);

        ASSERT_EQ(res.height, tc.h);
        ASSERT_EQ(res.width,  tc.w);

        for (int i = 0; i < res.height; ++i) {
            for (int j = 0; j < res.width; ++j) {
                double expected = tc.a[i * tc.w + j] * tc.b[i * tc.w + j];
                EXPECT_DOUBLE_EQ(res.map[i][j], expected);
            }
        }
        cvl_mat_free(a);
        cvl_mat_free(b);
        cvl_mat_free(res);
    }
}

// cvl_mat_transpose

TEST(MatTest, Transpose) {
    // tc1: (2x3) --> (3x2)
    double i1[] = {
        1, 2, 3,
        4, 5, 6,
    };
    double e1[] = {
        1, 4,
        2, 5,
        3, 6,
    };

    // tc2: (3x3) --> (3x3)
    double i2[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
    };
    double e2[] = {
        1, 4, 7,
        2, 5, 8,
        3, 6, 9,
    };

    // Test Cases.
    struct { int h; int w; double *entry; } test_cases[] = {
        { 2, 3, i1 },
        { 3, 2, e1 },
        { 3, 3, i2 },
    };

    // Run Test Cases.
    for (const auto& tc : test_cases) {
        Matrix src = cvl_mat_create_from(tc.entry, tc.h, tc.w);
        Matrix dst = cvl_mat_transpose_new(&src);

        ASSERT_EQ(dst.height, tc.w);
        ASSERT_EQ(dst.width,  tc.h);

        for (int i = 0; i < dst.height; ++i) {
            for (int j = 0; j < dst.width; ++j) {
                EXPECT_DOUBLE_EQ(dst.map[i][j], src.map[j][i]);
            }
        }
        cvl_mat_free(src);
        cvl_mat_free(dst);
    }
}

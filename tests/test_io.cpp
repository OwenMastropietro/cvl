#include <cvl/cvl.h>
#include <gtest/gtest.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_bad_ext(void);
void test_write_pbm(void);
void test_write_pgm(void);
void test_write_ppm(void);

TEST(IOTest, Imwrite) {
    // test_write_pbm();
    // test_write_pgm();
    // test_write_ppm();

    // Test 1: invalid extension
    {
        cvl_Mat img = cvl_mat_create(5, 10, 1, CVL_UINT8);
        ASSERT_EQ(cvl_imwrite("test.bad", &img), -1);
        cvl_mat_free(&img);
    }

    // Test 2: write PBM
    {
        cvl_Mat img = cvl_mat_create(5, 10, 1, CVL_UINT8);

        ASSERT_EQ(cvl_imwrite("test.pbm", &img), 0);

        FILE *f = fopen("test.pbm", "rb");
        ASSERT_NE(f, nullptr);

        // Read type.
        char type[3] = {0};
        ASSERT_TRUE(fscanf(f, "%s", type));
        ASSERT_EQ(strncmp(type, "P4", 2), 0);

        // Read dims.
        int width = 0;
        int height = 0;
        ASSERT_EQ(fscanf(f, "%d %d\n", &width, &height), 2);
        EXPECT_EQ(width, 10);
        EXPECT_EQ(height, 5);

        fclose(f);
        cvl_mat_free(&img);
    }

    // Test 3: write PGM
    {
        cvl_Mat img = cvl_mat_create(5, 10, 1, CVL_UINT8);

        ASSERT_EQ(cvl_imwrite("test_img.pgm", &img), 0);

        FILE *f = fopen("test_img.pgm", "rb");
        ASSERT_NE(f, nullptr);

        // Read type.
        char type[3] = {0};
        ASSERT_TRUE(fscanf(f, "%s", type));
        ASSERT_EQ(strncmp(type, "P5", 2), 0);

        // Read dims.
        int width = 0;
        int height = 0;
        ASSERT_EQ(fscanf(f, "%d %d\n", &width, &height), 2);
        EXPECT_EQ(width, 10);
        EXPECT_EQ(height, 5);

        fclose(f);
        cvl_mat_free(&img);
    }

    // Test 4: write PPM
    {
        cvl_Mat img = cvl_mat_create(5, 10, 3, CVL_UINT8);

        ASSERT_EQ(cvl_imwrite("test_img.ppm", &img), 0);

        FILE *f = fopen("test_img.ppm", "rb");
        ASSERT_NE(f, nullptr);

        // Read type.
        char type[3] = {0};
        ASSERT_TRUE(fscanf(f, "%s", type));
        ASSERT_EQ(strncmp(type, "P6", 2), 0);

        // Read dims.
        int width = 0;
        int height = 0;
        ASSERT_EQ(fscanf(f, "%d %d\n", &width, &height), 2);
        EXPECT_EQ(width, 10);
        EXPECT_EQ(height, 5);

        fclose(f);
        cvl_mat_free(&img);
    }
}

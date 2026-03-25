#include <cvl/cvl.h>
#include <gtest/gtest.h>

void test_bad_ext(void);
void test_write_pbm(void);
void test_write_pgm(void);
void test_write_ppm(void);

void test_imread_pgm(void);
void test_imread_rectangular(void);
void test_imread_pbm(void);
void test_imread_pbm_unaligned(void);
void test_imread_ppm(void);
void test_imread_maxval_scaling(void);

TEST(IOTest, WriteImage) {
    test_bad_ext();
    test_write_pbm();
    test_write_pgm();
    test_write_ppm();
}

TEST(IOTest, ReadImage) {
    test_imread_pgm();
    test_imread_rectangular();
    test_imread_pbm();
    test_imread_pbm_unaligned();
    test_imread_ppm();
    test_imread_maxval_scaling();
}


void test_bad_ext(void) {
    Image img = cvl_img_create(5, 10);

    ASSERT_TRUE(cvl_imwrite("invalid.ext", &img) == CVL_ERR_FILE_IO);

    cvl_img_free(img);
}

void test_write_pbm(void) {
    Image img = cvl_img_create(5, 10);

    ASSERT_TRUE(cvl_imwrite("test.pbm", &img) == CVL_OK);

    FILE *f = fopen("test.pbm", "rb");
    ASSERT_TRUE(f);

    // Read type.
    char type[3] = {0};
    ASSERT_TRUE(fscanf(f, "%s", type) != 0);
    ASSERT_TRUE(strncmp(type, "P4", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    ASSERT_TRUE(fscanf(f, "%d %d\n", &width, &height) == 2);
    ASSERT_TRUE(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

void test_write_pgm(void) {
    Image img = cvl_img_create(5, 10);

    ASSERT_TRUE(cvl_imwrite("test_img.pgm", &img) == CVL_OK);

    FILE *f = fopen("test_img.pgm", "rb");
    ASSERT_TRUE(f);

    // Read type.
    char type[3] = {0};
    ASSERT_TRUE(fscanf(f, "%s", type) != 0);
    ASSERT_TRUE(strncmp(type, "P5", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    ASSERT_TRUE(fscanf(f, "%d %d\n", &width, &height) == 2);
    ASSERT_TRUE(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

void test_write_ppm(void) {
    Image img = cvl_img_create(5, 10);

    ASSERT_TRUE(cvl_imwrite("test_img.ppm", &img) == CVL_OK);

    FILE *f = fopen("test_img.ppm", "rb");
    ASSERT_TRUE(f);

    // Read type.
    char type[3] = {0};
    ASSERT_TRUE(fscanf(f, "%s", type) != 0);
    ASSERT_TRUE(strncmp(type, "P6", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    ASSERT_TRUE(fscanf(f, "%d %d\n", &width, &height) == 2);
    ASSERT_TRUE(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

// cvl_imread

void test_imread_pgm(void) {
    const int h = 2;
    const int w = 2;
    const char *filename = TEST_SOURCE_DIR "/data/test.pgm";

    Image img = cvl_img_create(h, w);
    img.map[0][0].i = 0;   img.map[0][1].i = 50;
    img.map[1][0].i = 100; img.map[1][1].i = 250;

    cvl_imwrite(filename, &img);
    Image res = cvl_imread(filename);
    ASSERT_TRUE(res.map != NULL);

    ASSERT_EQ(res.height, h);
    ASSERT_EQ(res.width, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            EXPECT_EQ(res.map[i][j].i, img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_rectangular(void) {
    const int h = 3;
    const int w = 5;
    const char *filename = TEST_SOURCE_DIR "/data/test.pgm";

    Image img = cvl_img_create(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            img.map[i][j].i = i * 10 + j;
        }
    }

    cvl_imwrite(filename, &img);
    Image res = cvl_imread(filename);

    ASSERT_EQ(res.height, h);
    ASSERT_EQ(res.width, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            EXPECT_EQ(res.map[i][j].i, img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_pbm(void) {
    const int h = 2;
    const int w = 8; // 1 byte per row
    const char *filename = TEST_SOURCE_DIR "/data/test.pbm";

    Image img = cvl_img_create(h, w);
    for (int j = 0; j < w; j++) {
        img.map[0][j].i = (j % 2) ? 255 : 0;
        img.map[1][j].i = (j % 2) ? 0 : 255;
    }

    cvl_imwrite(filename, &img);
    Image res = cvl_imread(filename);

    ASSERT_EQ(res.height, h);
    ASSERT_EQ(res.width, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            EXPECT_EQ(res.map[i][j].i, img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_pbm_unaligned(void) {
    const int h = 2;
    const int w = 10; // not divisible by 8
    const char *filename = TEST_SOURCE_DIR "/data/test.pbm";

    Image img = cvl_img_create(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            img.map[i][j].i = (j % 3 == 0) ? 255 : 0;
        }
    }

    cvl_imwrite(filename, &img);
    Image res = cvl_imread(filename);

    ASSERT_EQ(res.height, h);
    ASSERT_EQ(res.width, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            EXPECT_EQ(res.map[i][j].i, img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_ppm(void) {
    const int h = 2;
    const int w = 2;
    const char *filename = TEST_SOURCE_DIR "/data/test.ppm";

    Image img = cvl_img_create(h, w);

    img.map[0][0].r = 255; img.map[0][0].g = 0;   img.map[0][0].b = 0;
    img.map[0][1].r = 0;   img.map[0][1].g = 255; img.map[0][1].b = 0;
    img.map[1][0].r = 0;   img.map[1][0].g = 0;   img.map[1][0].b = 255;
    img.map[1][1].r = 255; img.map[1][1].g = 255; img.map[1][1].b = 255;

    cvl_imwrite(filename, &img);
    Image res = cvl_imread(filename);

    ASSERT_EQ(res.height, h);
    ASSERT_EQ(res.width, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            EXPECT_EQ(res.map[i][j].r, img.map[i][j].r);
            EXPECT_EQ(res.map[i][j].g, img.map[i][j].g);
            EXPECT_EQ(res.map[i][j].b, img.map[i][j].b);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_maxval_scaling(void) {
    const char *filename = TEST_SOURCE_DIR "/data/test_scale.pgm";

    // manually write a PGM with maxval != 255
    FILE *f = fopen(filename, "wb");
    ASSERT_TRUE(f);
    fprintf(f, "P5\n2 1\n100\n");

    unsigned char data[2] = {50, 100}; // should map to ~127 and 255
    fwrite(data, 1, 2, f);
    fclose(f);

    Image res = cvl_imread(filename);

    ASSERT_EQ(res.height, 1);
    ASSERT_EQ(res.width, 2);

    EXPECT_EQ(res.map[0][0].i, (unsigned char)(50 * 255 / 100));
    EXPECT_EQ(res.map[0][1].i, 255);

    cvl_img_free(res);
}

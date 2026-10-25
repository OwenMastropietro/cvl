#include <cvl/cvl.h>
#include <gtest/gtest.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

// ==============================================
// cvl_imread
// ==============================================

std::string TEST_DIR = std::string(TEST_SOURCE_DIR) + "/data/";

TEST(ImRead, FileNotFound) {
    std::string path = TEST_DIR + "nonexistent.sol";
    cvl_Mat img = cvl_imread(path.c_str());
    ASSERT_EQ(img.data, nullptr);
}

TEST(ImRead, InvalidFormat) {
    std::string path = TEST_DIR + "test.txt";
    cvl_Mat img = cvl_imread(path.c_str());
    ASSERT_EQ(img.data, nullptr);
}

TEST(ImRead, ReadPBM) {
    {
        std::string path = TEST_DIR + "test.pbm";
        cvl_Mat img = cvl_imread(path.c_str());
        ASSERT_NE(img.data,     nullptr);
        EXPECT_EQ(img.width,    10);
        EXPECT_EQ(img.height,   2);
        EXPECT_EQ(img.channels, 1);
        EXPECT_EQ(img.depth,    CVL_UINT8);
        cvl_mat_free(&img);
    }
}

TEST(ImRead, ReadPGM) {
    {
        std::string path = TEST_DIR + "test.pgm";
        cvl_Mat img = cvl_imread(path.c_str());
        ASSERT_NE(img.data,     nullptr);
        EXPECT_EQ(img.width,    5);
        EXPECT_EQ(img.height,   3);
        EXPECT_EQ(img.channels, 1);
        EXPECT_EQ(img.depth,    CVL_UINT8);
        cvl_mat_free(&img);
    }
}
        
TEST(ImRead, ReadPPM) {
    {
        std::string path = TEST_DIR + "test.ppm";
        cvl_Mat img = cvl_imread(path.c_str());
        ASSERT_NE(img.data,     nullptr);
        EXPECT_EQ(img.width,    2);
        EXPECT_EQ(img.height,   2);
        EXPECT_EQ(img.channels, 3);
        EXPECT_EQ(img.depth,    CVL_UINT8);
        cvl_mat_free(&img);
    }
}

TEST(ImRead, ReadPNG) {
    {
        std::string path = TEST_DIR + "test.png";
        cvl_Mat img = cvl_imread(path.c_str());
        ASSERT_NE(img.data,     nullptr);
        EXPECT_EQ(img.width,    481);
        EXPECT_EQ(img.height,   321);
        EXPECT_EQ(img.channels, 3);
        EXPECT_EQ(img.depth,    CVL_UINT8);
        cvl_mat_free(&img);
    }
}

TEST(ImRead, ReadJPG) {
    {
        std::string path = TEST_DIR + "test.jpg";
        cvl_Mat img = cvl_imread(path.c_str());
        ASSERT_NE(img.data,     nullptr);
        EXPECT_EQ(img.width,    481);
        EXPECT_EQ(img.height,   321);
        EXPECT_EQ(img.channels, 3);
        EXPECT_EQ(img.depth,    CVL_UINT8);
        cvl_mat_free(&img);
    }
}

// ==============================================
// cvl_imwrite
// ==============================================

TEST(ImWrite, InvalidFormat) {
        cvl_Mat img = cvl_mat_create(2, 3, 1, CVL_UINT8);
        ASSERT_NE(cvl_imwrite("test.bad", &img), 0);
        ASSERT_EQ(cvl_imwrite(nullptr, &img), -1);
        ASSERT_EQ(cvl_imwrite("/tmp/cvl-noext", &img), -1);
        ASSERT_EQ(cvl_imwrite("/no/such/dir/out.pgm", &img), -1);
        cvl_mat_free(&img);
}

TEST(ImRead, TruncatedAndBadMaxval) {
    const char *short_path = "/tmp/cvl-short.pgm";
    FILE *f = fopen(short_path, "wb");
    ASSERT_NE(f, nullptr);
    fputs("P5\n8 8\n255\n", f);
    fwrite("abcd", 1, 4, f);
    fclose(f);
    cvl_Mat short_img = cvl_imread(short_path);
    EXPECT_EQ(short_img.data, nullptr);

    const char *max_path = "/tmp/cvl-badmax.pgm";
    f = fopen(max_path, "wb");
    ASSERT_NE(f, nullptr);
    fputs("P5\n2 2\n0\nabcd", f);
    fclose(f);
    cvl_Mat badmax = cvl_imread(max_path);
    EXPECT_EQ(badmax.data, nullptr);

    const char *ascii_path = "/tmp/cvl-ascii.pgm";
    f = fopen(ascii_path, "wb");
    ASSERT_NE(f, nullptr);
    fputs("P2\n2 2\n255\n1 2 3 4\n", f);
    fclose(f);
    cvl_Mat ascii = cvl_imread(ascii_path);
    EXPECT_EQ(ascii.data, nullptr);
}

TEST(ImWrite, HonorsStride) {
    cvl_Mat wide = cvl_mat_create(2, 4, 1, CVL_UINT8);
    ASSERT_NE(wide.data, nullptr);
    uint8_t *px = static_cast<uint8_t *>(wide.data);
    for (int i = 0; i < 8; ++i) {
        px[i] = static_cast<uint8_t>(i + 1);
    }
    wide.width = 2;

    ASSERT_EQ(cvl_imwrite("/tmp/cvl-stride.pgm", &wide), 0);
    cvl_Mat back = cvl_imread("/tmp/cvl-stride.pgm");
    ASSERT_NE(back.data, nullptr);
    EXPECT_EQ(back.height, 2);
    EXPECT_EQ(back.width, 2);
    uint8_t *b = static_cast<uint8_t *>(back.data);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(b[2], 5);
    EXPECT_EQ(b[3], 6);

    cvl_mat_free(&wide);
    cvl_mat_free(&back);
}

TEST(ImWrite, WritePBM) {
    {
        uint8_t vals[2][3] = {
            {0,   255, 0},
            {255, 0,   255},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 1, CVL_UINT8, vals);

        ASSERT_EQ(cvl_imwrite("test.pbm", &img), 0);

        cvl_Mat loaded = cvl_imread("test.pbm");
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 1);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        EXPECT_EQ(memcmp(img.data, loaded.data, img.height * img.width), 0);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

TEST(ImWrite, WritePGM) {
    {
        uint8_t vals[2][3] = {
            {0, 128, 255},
            {4, 20,  69},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 1, CVL_UINT8, vals);

        ASSERT_EQ(cvl_imwrite("test.pgm", &img), 0);

        cvl_Mat loaded = cvl_imread("test.pgm");
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 1);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        EXPECT_EQ(memcmp(img.data, loaded.data, img.height * img.width), 0);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

TEST(ImWrite, WritePPM) {
    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        ASSERT_EQ(cvl_imwrite("test.ppm", &img), 0);

        cvl_Mat loaded = cvl_imread("test.ppm");
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 3);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        EXPECT_EQ(memcmp(img.data, loaded.data, img.height * img.width * img.channels), 0);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

TEST(ImWrite, WritePNG) {
    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        ASSERT_EQ(cvl_imwrite("test.png", &img), 0);

        cvl_Mat loaded = cvl_imread("test.png");
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 3);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        EXPECT_EQ(memcmp(img.data, loaded.data, img.height * img.width * img.channels), 0);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

TEST(ImWrite, WriteJPG) {
    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        ASSERT_EQ(cvl_imwrite("test.jpg", &img), 0);

        cvl_Mat loaded = cvl_imread("test.jpg");
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 3);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

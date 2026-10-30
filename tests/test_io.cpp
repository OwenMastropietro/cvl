#include <cvl/cvl.h>
#include <gtest/gtest.h>
#include "test_utils.h"

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
        TempDir temp;
        cvl_Mat img = cvl_mat_create(2, 3, 1, CVL_UINT8);
        ASSERT_NE(cvl_imwrite("test.bad", &img), 0);
        ASSERT_EQ(cvl_imwrite(nullptr, &img), -1);
        ASSERT_EQ(cvl_imwrite(temp.file("noext").string().c_str(), &img), -1);
        ASSERT_EQ(cvl_imwrite((temp.file("missing").string() + "/out.pgm").c_str(), &img), -1);
        cvl_mat_free(&img);
}

TEST(ImRead, TruncatedAndBadMaxval) {
    TempDir temp;
    FILE *f;

    auto short_path = temp.file("short.pgm");
    f = fopen(short_path.string().c_str(), "wb");
    ASSERT_NE(f, nullptr);

    fputs("P5\n8 8\n255\n", f);
    fwrite("abcd", 1, 4, f);
    fclose(f);

    cvl_Mat short_img = cvl_imread(short_path.string().c_str());
    EXPECT_EQ(short_img.data, nullptr);

    auto max_path = temp.file("badmax.pgm");
    f = fopen(max_path.string().c_str(), "wb");
    ASSERT_NE(f, nullptr);

    fputs("P5\n2 2\n0\nabcd", f);
    fclose(f);

    cvl_Mat badmax = cvl_imread(max_path.string().c_str());
    EXPECT_EQ(badmax.data, nullptr);

    auto ascii_path = temp.file("ascii.pgm");
    f = fopen(ascii_path.string().c_str(), "wb");
    ASSERT_NE(f, nullptr);

    fputs("P2\n2 2\n255\n1 2 3 4\n", f);
    fclose(f);

    cvl_Mat ascii = cvl_imread(ascii_path.string().c_str());
    EXPECT_EQ(ascii.data, nullptr);
}

TEST(ImWrite, HonorsStride) {
    TempDir temp;

    cvl_Mat wide = cvl_mat_create(2, 4, 1, CVL_UINT8);
    ASSERT_NE(wide.data, nullptr);
    uint8_t *px = static_cast<uint8_t *>(wide.data);
    for (int i = 0; i < 8; ++i) {
        px[i] = static_cast<uint8_t>(i + 1);
    }
    wide.width = 2;

    auto stride_path = temp.file("stride.pgm");
    ASSERT_EQ(cvl_imwrite(stride_path.string().c_str(), &wide), 0);
    cvl_Mat back = cvl_imread(stride_path.string().c_str());
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
    TempDir temp;

    {
        uint8_t vals[2][3] = {
            {0,   255, 0},
            {255, 0,   255},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 1, CVL_UINT8, vals);

        auto path = temp.file("test.pbm");
        ASSERT_EQ(cvl_imwrite(path.string().c_str(), &img), 0);

        cvl_Mat loaded = cvl_imread(path.string().c_str());
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
    TempDir temp;

    {
        uint8_t vals[2][3] = {
            {0, 128, 255},
            {4, 20,  69},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 1, CVL_UINT8, vals);

        auto path = temp.file("test.pgm");
        ASSERT_EQ(cvl_imwrite(path.string().c_str(), &img), 0);

        cvl_Mat loaded = cvl_imread(path.string().c_str());
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
    TempDir temp;

    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        auto path = temp.file("test.ppm");
        ASSERT_EQ(cvl_imwrite(path.string().c_str(), &img), 0);

        cvl_Mat loaded = cvl_imread(path.string().c_str());
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
    TempDir temp;

    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        auto path = temp.file("test.png");
        ASSERT_EQ(cvl_imwrite(path.string().c_str(), &img), 0);

        cvl_Mat loaded = cvl_imread(path.string().c_str());
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
    TempDir temp;

    {
        uint8_t vals[2][3][3] = {
            {{0, 0,  0},  {128, 128, 128}, {255, 255, 255}},
            {{4, 20, 69}, {255, 0,   0},   {0,   255, 0}},
        };

        cvl_Mat img = cvl_mat_create_from(2, 3, 3, CVL_UINT8, vals);

        auto path = temp.file("test.jpg");
        ASSERT_EQ(cvl_imwrite(path.string().c_str(), &img), 0);

        cvl_Mat loaded = cvl_imread(path.string().c_str());
        ASSERT_NE(loaded.data,     nullptr);
        EXPECT_EQ(loaded.height,   2);
        EXPECT_EQ(loaded.width,    3);
        EXPECT_EQ(loaded.channels, 3);
        EXPECT_EQ(loaded.depth,    CVL_UINT8);

        cvl_mat_free(&img);
        cvl_mat_free(&loaded);
    }
}

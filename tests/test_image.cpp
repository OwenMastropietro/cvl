#include "cvl_core.h"
#include <gtest/gtest.h>

TEST(ImageTest, CreateImage) {
    const int h = 3;
    const int w = 3;

    Image img = cvl_img_create(h, w);

    EXPECT_EQ(img.height, h);
    EXPECT_EQ(img.width, w);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            EXPECT_EQ(img.map[i][j].i, 255); // todo: zero-fill instead
        }
    }
}

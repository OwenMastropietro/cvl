#include "cvl_imgproc.h"
#include "cvl_io.h"
#include <assert.h>
#include <stdio.h>

void test_imread_pgm(void);
void test_imread_rectangular(void);
void test_imread_pbm(void);
void test_imread_pbm_unaligned(void);
void test_imread_ppm(void);
void test_imread_maxval_scaling(void);

void test_imread(void) {
    test_imread_pgm();
    test_imread_rectangular();
    test_imread_pbm();
    test_imread_pbm_unaligned();
    test_imread_ppm();
    test_imread_maxval_scaling();

    printf("  cvl_imread - passed\n");
}

void test_imread_pgm(void) {
    const int h = 2;
    const int w = 2;

    Image img = cvl_img_create(h, w);
    img.map[0][0].i = 0;   img.map[0][1].i = 50;
    img.map[1][0].i = 100; img.map[1][1].i = 250;

    cvl_imwrite("./data/modified/test.pgm", &img);
    Image res = cvl_imread("./data/modified/test.pgm");

    assert(res.height == h);
    assert(res.width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            assert(res.map[i][j].i == img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_rectangular(void) {
    const int h = 3, w = 5;

    Image img = cvl_img_create(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            img.map[i][j].i = i * 10 + j;
        }
    }

    cvl_imwrite("./data/modified/test.pgm", &img);
    Image res = cvl_imread("./data/modified/test.pgm");

    assert(res.height == h);
    assert(res.width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            assert(res.map[i][j].i == img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_pbm(void) {
    const int h = 2, w = 8; // 1 byte per row

    Image img = cvl_img_create(h, w);

    for (int j = 0; j < w; j++) {
        img.map[0][j].i = (j % 2) ? 255 : 0;
        img.map[1][j].i = (j % 2) ? 0 : 255;
    }

    cvl_imwrite("./data/modified/test.pbm", &img);
    Image res = cvl_imread("./data/modified/test.pbm");

    assert(res.height == h);
    assert(res.width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            assert(res.map[i][j].i == img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_pbm_unaligned(void) {
    const int h = 2, w = 10; // not divisible by 8

    Image img = cvl_img_create(h, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            img.map[i][j].i = (j % 3 == 0) ? 255 : 0;
        }
    }

    cvl_imwrite("./data/modified/test.pbm", &img);
    Image res = cvl_imread("./data/modified/test.pbm");

    assert(res.height == h);
    assert(res.width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            assert(res.map[i][j].i == img.map[i][j].i);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_ppm(void) {
    const int h = 2, w = 2;

    Image img = cvl_img_create(h, w);

    img.map[0][0].r = 255; img.map[0][0].g = 0;   img.map[0][0].b = 0;
    img.map[0][1].r = 0;   img.map[0][1].g = 255; img.map[0][1].b = 0;
    img.map[1][0].r = 0;   img.map[1][0].g = 0;   img.map[1][0].b = 255;
    img.map[1][1].r = 255; img.map[1][1].g = 255; img.map[1][1].b = 255;

    cvl_imwrite("./data/modified/test.ppm", &img);
    Image res = cvl_imread("./data/modified/test.ppm");

    assert(res.height == h);
    assert(res.width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            assert(res.map[i][j].r == img.map[i][j].r);
            assert(res.map[i][j].g == img.map[i][j].g);
            assert(res.map[i][j].b == img.map[i][j].b);
        }
    }

    cvl_img_free(img);
    cvl_img_free(res);
}

void test_imread_maxval_scaling(void) {
    // manually write a PGM with maxval != 255
    FILE *f = fopen("./data/modified/test_scale.pgm", "wb");
    fprintf(f, "P5\n2 1\n100\n");

    unsigned char data[2] = {50, 100}; // should map to ~127 and 255
    fwrite(data, 1, 2, f);
    fclose(f);

    Image res = cvl_imread("./data/modified/test_scale.pgm");

    assert(res.height == 1);
    assert(res.width == 2);

    assert(res.map[0][0].i == (unsigned char)(50 * 255 / 100));
    assert(res.map[0][1].i == 255);

    cvl_img_free(res);
}

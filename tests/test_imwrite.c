#include "cvl_imgproc.h"
#include "cvl_io.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_bad_ext(void);
void test_write_pbm(void);
void test_write_pgm(void);
void test_write_ppm(void);

void test_imwrite(void) {
    test_bad_ext();
    test_write_pbm();
    test_write_pgm();
    test_write_ppm();

    printf("  cvl_imwrite - passed\n");
}

void test_bad_ext(void) {
    Image img = cvl_img_create(5, 10);

    assert(cvl_imwrite("test.bad", &img) == -1);

    cvl_img_free(img);
}

void test_write_pbm(void) {
    Image img = cvl_img_create(5, 10);

    assert(cvl_imwrite("test.pbm", &img) == 0);

    FILE *f = fopen("test.pbm", "rb");
    assert(f);

    // Read type.
    char type[3] = {0};
    assert(fscanf(f, "%s", type));
    assert(strncmp(type, "P4", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    assert(fscanf(f, "%d %d\n", &width, &height) == 2);
    assert(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

void test_write_pgm(void) {
    Image img = cvl_img_create(5, 10);

    assert(cvl_imwrite("test_img.pgm", &img) == 0);

    FILE *f = fopen("test_img.pgm", "rb");
    assert(f);

    // Read type.
    char type[3] = {0};
    assert(fscanf(f, "%s", type));
    assert(strncmp(type, "P5", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    assert(fscanf(f, "%d %d\n", &width, &height) == 2);
    assert(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

void test_write_ppm(void) {
    Image img = cvl_img_create(5, 10);

    assert(cvl_imwrite("test_img.ppm", &img) == 0);

    FILE *f = fopen("test_img.ppm", "rb");
    assert(f);

    // Read type.
    char type[3] = {0};
    assert(fscanf(f, "%s", type));
    assert(strncmp(type, "P6", 2) == 0);

    // Read dims.
    int width = 0;
    int height = 0;
    assert(fscanf(f, "%d %d\n", &width, &height) == 2);
    assert(width == 10 && height == 5);

    fclose(f);
    cvl_img_free(img);
}

// Example Program: Using CVL.

#include "cvl_imgproc.h"
#include "cvl_io.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void part_i(void) {
    cvl_Mat img = cvl_imread("./data/original/text.pgm");
    assert(img.channels == 1);

    cvl_imwrite("./data/modified/text.pgm", &img);

    cvl_threshold(&img, 128, 255, CVL_THRESH_BINARY);

    cvl_imwrite("./data/modified/text_bw.pbm", &img);

    srand(42);
    cvl_add_noise(&img, 0.05);
    cvl_imwrite("./data/modified/text_noise.pbm", &img);

    cvl_expand(&img);
    cvl_shrink(&img);
    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_imwrite("./data/modified/text_clear.pbm", &img);

    cvl_mat_free(&img);
}

// Connected Component Labeling.
void part_ii(void) {
    cvl_Mat img = cvl_imread("./data/original/text.pgm");
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_INT32);

    cvl_imwrite("./data/modified/text_bw.pgm", &img);
    cvl_threshold(&img, 128, 255, CVL_THRESH_BINARY);
    cvl_imwrite("./data/modified/text_bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// Count & Color Components (according to a threshold).
void part_iii(void) {
    cvl_Mat img = cvl_imread("./data/original/text.pgm");
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_INT32);
    cvl_Mat colored = {0};

    cvl_threshold(&img, 128, 255, CVL_THRESH_BINARY);
    cvl_imwrite("./data/modified/text_bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_cvtcolor(&img, &colored, CVL_COLOR_GRAY2RGB);
    num_components = cvl_color_components(&colored, &labels, 20);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("./data/modified/text_colored.ppm", &colored);

    cvl_mat_free(&colored);
    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// Perform Mean & Median Blurring (via Convolution).
void p2(void) {
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    cvl_Mat grey = {0};
    cvl_cvtcolor(&img, &grey, CVL_COLOR_RGB2GRAY);
    cvl_imwrite("./data/modified/sample.pgm", &grey);
    cvl_Mat grey_f64 = cvl_mat_create(grey.height, grey.width, 1, CVL_FLOAT64);
    cvl_convert_to_f64(&grey, &grey_f64);

    // Mean Windowing.
    cvl_Mat mean = cvl_mat_create(grey.height, grey.width, 1, CVL_FLOAT64);
    cvl_blur(&grey_f64, &mean, 3);

    cvl_Mat mean_u8 = cvl_mat_create(grey.height, grey.width, 1, CVL_UINT8);
    cvl_convert_to_u8(&mean, &mean_u8);
    cvl_imwrite("./data/modified/mean.pgm", &mean_u8);
    
    // Median Windowing.
    cvl_Mat median = cvl_mat_create(grey.height, grey.width, 1, CVL_FLOAT64);
    cvl_median_blur(&grey_f64, &median, 3);
    
    cvl_Mat median_u8 = cvl_mat_create(grey.height, grey.width, 1, CVL_UINT8);
    cvl_convert_to_u8(&median, &median_u8);
    cvl_imwrite("./data/modified/median.pgm", &median_u8);

    
    cvl_mat_free(&median_u8);
    cvl_mat_free(&median);
    cvl_mat_free(&mean_u8);
    cvl_mat_free(&mean);
    cvl_mat_free(&grey_f64);
    cvl_mat_free(&grey);
    cvl_mat_free(&img);
}

int main(void) {

    // part_i();
    // part_ii();
    // part_iii();

    p2();

    return 0;
}

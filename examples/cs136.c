// Example: Assignment checkpoints/deliverables for CS136.

#include <cvl/cvl.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void p1_i(void) {
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

// CCL - Count Components.
void p1_ii(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Initialize Labels Container.
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_32S);

    // Label Components.
    int num_components = 0;
    num_components = cvl_connected_components(&img, &labels, 4);

    printf("\nNumber of Components: %d\n", num_components);

    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// CCL - Count & Color Components (according to a threshold).
void p1_iii(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Initialize Labels Container.
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_32S);

    // Label Components.
    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    // Color Labeled Components.
    cvl_Mat components = cvl_cvt_color_new(&img, CVL_COLOR_GRAY2RGB);
    num_components = cvl_color_components(&components, &labels, 100);
    printf("\nNumber of Components: %d\n", num_components);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.pgm", &img);
    cvl_imwrite("./data/modified/2-components.ppm", &components);

    // Cleanup.
    cvl_mat_free(&components);
    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// Perform Mean & Median Blurring (via Convolution).
void p2(void) {
    // Load input image.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Apply Mean Blur.
    cvl_Mat mean_blur = cvl_blur_mean_new(&img, 3);

    // Apply Median Blur.
    cvl_Mat median_blur = cvl_blur_median_new(&img, 3);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-mean.ppm", &mean_blur);
    cvl_imwrite("./data/modified/3-median.ppm", &median_blur);

    // Cleanup.
    cvl_mat_free(&median_blur);
    cvl_mat_free(&mean_blur);
    cvl_mat_free(&img);
}

int main(void) {

    // p1_i();
    // p1_ii();
    // p1_iii();

    p2();

    return 0;
}

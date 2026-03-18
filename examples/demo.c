// Example Program: Using CVL.

#include "cvl_imgproc.h"
#include "cvl_io.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void part_i(void) {
    Image img = readImage("./data/original/text.pgm");

    cvl_threshold(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    srand(42);
    cvl_add_noise(&img, 0.1);
    cvl_imwrite("./data/modified/noisy.pbm", &img);

    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_expand(&img);
    cvl_shrink(&img);
    cvl_imwrite("./data/modified/cleared.pbm", &img);

    cvl_imfree(img);
}

// Connected Component Labeling.
void part_ii(void) {
    Image img = readImage("./data/original/text.pgm");
    Matrix mat = createMatrix(img.height, img.width); // labels

    cvl_threshold(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_matfree(mat);
    cvl_imfree(img);
}

// Count & Color Components (according to a threshold).
void part_iii(void) {
    Image img = readImage("./data/original/text.pgm");
    Matrix mat = createMatrix(img.height, img.width); // labels

    cvl_threshold(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    num_components = cvl_color_components(&img, &mat, 100);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("./data/modified/components.ppm", &img);

    cvl_matfree(mat);
    cvl_imfree(img);
}

void p2(void) {
    // Load Original Greyscale Image.
    Image img = readImage("./data/original/sample.ppm");
    Matrix src = image2Matrix(img);
    cvl_imwrite("./data/modified/original.pgm", &img);

    // Mean Windowing.
    Matrix mean = createMatrix(img.height, img.width);
    cvl_blur(&src, &mean, 3);
    Image mean_img = matrix2Image(mean, 0, 1.0);
    cvl_imwrite("./data/modified/mean.pgm", &mean_img);
    cvl_imfree(mean_img);
    cvl_matfree(mean);

    // Median Windowing.
    Matrix median = createMatrix(img.height, img.width);
    cvl_median_blur(&src, &median, 3);
    Image median_img = matrix2Image(median, 0, 1.0);
    cvl_imwrite("./data/modified/median.pgm", &median_img);
    cvl_imfree(median_img);
    cvl_matfree(median);

    cvl_imfree(img);
    cvl_matfree(src);
}

int main(void) {

    part_i();
    // part_ii();
    // part_iii();

    // p2();

    return 0;
}

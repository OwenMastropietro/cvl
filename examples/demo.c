// Example Program: Interfacing with CVL and NetPBM.

#include "cvl.h"
#include "netpbm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void part_i(void) {
    Image img = readImage("./data/original/text.pgm");

    cvl_threshold(&img, 128);
    writeImage(img, "./data/modified/bw.pbm");

    srand(42);
    cvl_add_noise(&img, 0.1);
    writeImage(img, "./data/modified/noisy.pbm");

    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_expand(&img);
    cvl_shrink(&img);
    writeImage(img, "./data/modified/cleared.pbm");

    deleteImage(img);
}

// Connected Component Labeling.
void part_ii(void) {
    Image img = readImage("./data/original/text.pgm");
    Matrix mat = createMatrix(img.height, img.width); // labels

    cvl_threshold(&img, 128);
    writeImage(img, "./data/modified/bw.pbm");

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    deleteMatrix(mat);
    deleteImage(img);
}

// Count & Color Components (according to a threshold).
void part_iii(void) {
    Image img = readImage("./data/original/text.pgm");
    Matrix mat = createMatrix(img.height, img.width); // labels

    cvl_threshold(&img, 128);
    writeImage(img, "./data/modified/bw.pbm");

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    num_components = cvl_color_components(&img, &mat, 100);
    printf("\nNumber of Components: %d\n", num_components);

    writeImage(img, "./data/modified/components.ppm");

    deleteMatrix(mat);
    deleteImage(img);
}

void p2(void) {
    // Load Original Greyscale Image.
    Image img = readImage("./data/original/sample.ppm");
    Matrix src = image2Matrix(img);
    writeImage(img, "./data/modified/original.pgm");

    // Mean Windowing.
    Matrix mean = createMatrix(img.height, img.width);
    cvl_blur(&src, &mean, 3);
    Image mean_img = matrix2Image(mean, 0, 1.0);
    writeImage(mean_img, "./data/modified/mean.pgm");
    deleteImage(mean_img);
    deleteMatrix(mean);

    // Median Windowing.
    Matrix median = createMatrix(img.height, img.width);
    cvl_median_blur(&src, &median, 3);
    Image median_img = matrix2Image(median, 0, 1.0);
    writeImage(median_img, "./data/modified/median.pgm");
    deleteImage(median_img);
    deleteMatrix(median);

    deleteImage(img);
    deleteMatrix(src);
}

int main(void) {

    // part_i();
    // part_ii();
    // part_iii();

    p2();

    return 0;
}

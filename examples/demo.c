// Example Program: Using CVL.

#include "cvl_imgproc.h"
#include "cvl_io.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void part_i(void) {
    Image img = cvl_imread("./data/original/text.pgm");

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    srand(42);
    cvl_add_noise(&img, 0.1);
    cvl_imwrite("./data/modified/noisy.pbm", &img);

    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_expand(&img);
    cvl_shrink(&img);
    cvl_imwrite("./data/modified/cleared.pbm", &img);

    cvl_img_free(img);
}

// Connected Component Labeling.
void part_ii(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    Matrix mat = cvl_mat_create(img.height, img.width); // labels

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_mat_free(mat);
    cvl_img_free(img);
}

// Count & Color Components (according to a threshold).
void part_iii(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    Matrix mat = cvl_mat_create(img.height, img.width); // labels

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/bw.pbm", &img);

    int num_components = cvl_connected_components(&img, &mat, 4);
    printf("\nNumber of Components: %d\n", num_components);

    num_components = cvl_color_components(&img, &mat, 100);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("./data/modified/components.ppm", &img);

    cvl_mat_free(mat);
    cvl_img_free(img);
}

void p2(void) {
    // Load Original Greyscale Image.
    Image img = cvl_imread("./data/original/sample.ppm");
    Matrix src = cvl_img2mat(img);
    cvl_imwrite("./data/modified/original.pgm", &img);

    // Mean Windowing.
    Matrix mean = cvl_mat_create(img.height, img.width);
    cvl_blur(&src, &mean, 3);
    Image mean_img = cvl_mat2img(mean, 0, 1.0);
    cvl_imwrite("./data/modified/mean.pgm", &mean_img);
    cvl_img_free(mean_img);
    cvl_mat_free(mean);

    // Median Windowing.
    Matrix median = cvl_mat_create(img.height, img.width);
    cvl_median_blur(&src, &median, 3);
    Image median_img = cvl_mat2img(median, 0, 1.0);
    cvl_imwrite("./data/modified/median.pgm", &median_img);
    cvl_img_free(median_img);
    cvl_mat_free(median);

    cvl_img_free(img);
    cvl_mat_free(src);
}

void p3_sobel(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    cvl_imwrite("./data/modified/lena.ppm", &img);
    
    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/lena.pbm", &img);
    
    Matrix lena = cvl_img2mat(img);
    Matrix lena_smooth = cvl_blur_new(&lena, 3);
    
    Matrix gx = cvl_mat_create(lena.height, lena.width);
    Matrix gy = cvl_mat_create(lena.height, lena.width);
    cvl_sobel(&lena_smooth, &gx, &gy);
    
    Image horizontal = cvl_mat2img(gx, 0, 1);
    Image vertical = cvl_mat2img(gy, 0, 1);    
    cvl_imwrite("./data/modified/sobel-lena-grads-horizontal.pgm", &horizontal);
    cvl_imwrite("./data/modified/sobel-lena-grads-vertical.pgm", &vertical);
    
    Matrix mags = cvl_sobel_mag(&lena_smooth);
    Matrix angs = cvl_sobel_angle(&lena_smooth);
    Image mags_img = cvl_mat2img(mags, 0, 1);
    Image angs_img = cvl_mat2img(angs, 0, 1);
    cvl_imwrite("./data/modified/sobel-lena-magnitudes.pgm", &mags_img);
    cvl_binarize(&angs_img, 3.14/2);
    cvl_imwrite("./data/modified/sobel-lena-angles.pbm", &angs_img);
    
    cvl_img_free(angs_img);
    cvl_img_free(mags_img);
    cvl_mat_free(angs);
    cvl_mat_free(mags);
    cvl_img_free(vertical);
    cvl_img_free(horizontal);
    cvl_mat_free(gy);
    cvl_mat_free(gx);
    cvl_mat_free(lena_smooth);
    cvl_mat_free(lena);
    cvl_img_free(img);
}

void p3_canny(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    cvl_imwrite("./data/modified/lena.ppm", &img);
    
    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/lena.pbm", &img);
    
    Matrix lena = cvl_img2mat(img);
    Matrix lena_edges = cvl_canny_new(&lena, -1, 50, 120);

    Image lena_edges_img = cvl_mat2img(lena_edges, 0, 1);
    cvl_imwrite("./data/modified/lena-canny.pgm", &lena_edges_img);

    cvl_img_free(lena_edges_img);
    cvl_mat_free(lena_edges);
    cvl_mat_free(lena);
    cvl_img_free(img);
}

int main(void) {

    // part_i();
    // part_ii();
    // part_iii();

    // p2();
    p3_sobel();
    p3_canny();

    return 0;
}

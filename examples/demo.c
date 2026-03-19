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

    cvl_img_free(img);
}

// Connected Component Labeling.
void part_ii(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    Matrix mat = cvl_mat_create(img.height, img.width); // labels

    cvl_threshold(&img, 128);
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

    cvl_threshold(&img, 128);
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
    
    cvl_threshold(&img, 128);
    cvl_imwrite("./data/modified/lena.pbm", &img);
    
    Matrix lena = cvl_img2mat(img);
    Matrix lena_smooth = cvl_mat_create(lena.height, lena.width);
    cvl_blur(&lena, &lena_smooth, 3);
    
    Matrix gx = cvl_mat_create(lena.height, lena.width);
    Matrix gy = cvl_mat_create(lena.height, lena.width);
    cvl_sobel(&lena_smooth, &gx, &gy);
    
    Image horizontal = cvl_mat2img(gx, 0, 1);
    Image vertical = cvl_mat2img(gy, 0, 1);
    
    cvl_threshold(&horizontal, 128);
    cvl_threshold(&vertical, 128);
    
    cvl_imwrite("./data/modified/sobel-lena_mags_horizontal.pbm", &horizontal);
    cvl_imwrite("./data/modified/sobel-lena_mags_vertical.pbm", &vertical);
    
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
    
    cvl_threshold(&img, 128);
    cvl_imwrite("./data/modified/lena.pbm", &img);
    
    Matrix lena = cvl_img2mat(img);
    Matrix edges = cvl_mat_create(lena.height, lena.width);
    cvl_canny(&lena, &edges, -1, 50, 120);
    
    Image lena_canny = cvl_mat2img(edges, 0, 1);
    cvl_threshold(&lena_canny, 128);
    cvl_imwrite("./data/modified/lena-canny.pbm", &lena_canny);

    cvl_img_free(lena_canny);
    cvl_mat_free(edges);
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

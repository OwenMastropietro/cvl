// Example: Image Smoothing/Blurring.

#include <cvl/cvl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Image img = cvl_imread("./data/original/sample.ppm");
    cvl_binarize(&img, 128);
    Matrix mat = cvl_img2mat(img);

    cvl_imwrite("./data/modified/1-original.pbm", &img);

    Matrix smoothed;
    Image smoothed_img;

    // Averaging.
    smoothed = cvl_blur_new(&mat, 3);
    smoothed_img = cvl_mat2img(smoothed, 0, 1.0);
    cvl_imwrite("./data/modified/2-mean-blur.pgm", &smoothed_img);

    // Gaussian Blurring.
    smoothed = cvl_gaussian_blur_new(&mat, 1);
    smoothed_img = cvl_mat2img(smoothed, 0, 1.0);
    cvl_imwrite("./data/modified/3-gaussian-blur.pgm", &smoothed_img);

    // Median Blurring.
    smoothed = cvl_median_blur_new(&mat, 3);
    smoothed_img = cvl_mat2img(smoothed, 0, 1.0);
    cvl_imwrite("./data/modified/4-median-blur.pgm", &smoothed_img);

    cvl_img_free(smoothed_img);
    cvl_mat_free(smoothed);
    cvl_mat_free(mat);
    cvl_img_free(img);

    return 0;
}

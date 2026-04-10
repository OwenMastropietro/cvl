// A Small Computer Vision Library.

#pragma once

#include <cvl/cvl_io.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int cvl_threshold(Image *img, Image *dst, int thresh, int maxval, int type);

int cvl_binarize(Image *img, int thresh);

void cvl_add_noise(Image *img, double p);

void cvl_rotate(Image *img);

void cvl_invert(Image *img, int maxval);

void cvl_expand(Image *img);

void cvl_shrink(Image *img);

int cvl_connected_components(Image *img, Matrix *labels, int connectivity);

int cvl_color_components(Image *img, Matrix *labels, int thresh);

void cvl_correlate(Matrix *src, Matrix *dst, Matrix *kernel);

void cvl_convolve(Matrix *src, Matrix *dst, Matrix *kernel);

void cvl_blur_box(Matrix *src, Matrix *dst, int ksize, bool normalize);

void cvl_blur_mean(Matrix *src, Matrix *dst, int ksize);

void cvl_blur_gauss(Matrix *src, Matrix *dst, double sigma);

void cvl_blur_median(Matrix *src, Matrix *dst, int ksize);

void cvl_sobel(const Matrix *src, Matrix *gx, Matrix *gy);

void cvl_scharr(const Matrix *src, Matrix *gx, Matrix *gy);

void cvl_laplacian(const Matrix *src, Matrix *dst);

void cvl_canny(Matrix *src, Matrix *dst, double sigma, int lo, int hi);

// Convenience "_new" wrappers

Image cvl_threshold_new(Image *src, int thresh, int maxval, int type);

Image cvl_binarize_new(Image *src, int thresh);

Matrix cvl_correlate_new(Matrix *src, Matrix *kernel);

Matrix cvl_convolve_new(Matrix *src, Matrix *kernel);

Matrix cvl_blur_box_new(Matrix *src, int ksize, bool normalize);

Matrix cvl_blur_mean_new(Matrix *src, int ksize);

Matrix cvl_blur_gauss_new(Matrix *src, double sigma);

Matrix cvl_blur_median_new(Matrix *src, int ksize);

Matrix cvl_sobel_mag(Matrix *src);

Matrix cvl_scharr_mag(Matrix *src);

Matrix cvl_sobel_angle(Matrix *src);

Matrix cvl_laplacian_new(const Matrix *src);

Matrix cvl_canny_new(Matrix *src, double sigma, int lo, int hi);

#ifdef __cplusplus
}
#endif

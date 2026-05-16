// A Small Computer Vision Library.

#pragma once

#include <cvl/core.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void cvl_cvt_color(const cvl_Mat *src, cvl_Mat *dst, int code);

void cvl_cvt_depth(const cvl_Mat *src, cvl_Mat *dst, cvl_depth_t ddepth, double alpha, double beta);

void cvl_threshold(cvl_Mat *img, int thresh, int maxval, int type);

void cvl_rotate(cvl_Mat *img);

void cvl_invert(cvl_Mat *img, int maxval);

void cvl_dilate(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_erode(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_open(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_close(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_crop(const cvl_Mat *src, cvl_Mat *dst, int r, int c);

void cvl_resize(const cvl_Mat *src, cvl_Mat *dst, cvl_interp_t type);

int cvl_connected_components(const cvl_Mat *src, cvl_Mat *labels, int connectivity);

int cvl_color_components(cvl_Mat *dst, const cvl_Mat *labels, int thresh);

void cvl_correlate(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_convolve(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_convolve_sep(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kx, cvl_Mat *ky);

void cvl_blur_box(const cvl_Mat *src, cvl_Mat *dst, int ksize, bool normalize);

void cvl_blur_mean(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_blur_gauss(const cvl_Mat *src, cvl_Mat *dst, int ksize, double sigma);

void cvl_blur_median(const cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_sobel(const cvl_Mat *src, cvl_Mat *gx, cvl_Mat *gy);

void cvl_sobel_mag(const cvl_Mat *src, cvl_Mat *dst);

// ================
// Convenience "_new" wrappers
// ================

cvl_Mat cvl_cvt_color_new(const cvl_Mat *src, int code);

cvl_Mat cvl_cvt_depth_new(const cvl_Mat *src, cvl_depth_t ddepth, double alpha, double beta);

cvl_Mat cvl_threshold_new(const cvl_Mat *src, int thresh, int maxval, int type);

cvl_Mat cvl_crop_new(const cvl_Mat *src, int r, int c, int h, int w);

cvl_Mat cvl_resize_new(const cvl_Mat *src, int height, int width, cvl_interp_t type);

cvl_Mat cvl_correlate_new(const cvl_Mat *src, cvl_Mat *kernel);

cvl_Mat cvl_convolve_new(const cvl_Mat *src, cvl_Mat *kernel);

cvl_Mat cvl_blur_box_new(const cvl_Mat *src, int ksize, bool normalize);

cvl_Mat cvl_blur_mean_new(const cvl_Mat *src, int ksize);

cvl_Mat cvl_blur_gauss_new(const cvl_Mat *src, int ksize, double sigma);

cvl_Mat cvl_blur_median_new(const cvl_Mat *src, int ksize);

cvl_Mat cvl_sobel_mag_new(const cvl_Mat *src);

#ifdef __cplusplus
}
#endif

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

void cvl_add_noise(cvl_Mat *img, double p);

void cvl_rotate(cvl_Mat *img);

void cvl_invert(cvl_Mat *img, int maxval);

void cvl_expand(cvl_Mat *img);

void cvl_shrink(cvl_Mat *img);

int cvl_connected_components(const cvl_Mat *src, cvl_Mat *labels, int connectivity);

int cvl_color_components(cvl_Mat *dst, const cvl_Mat *labels, int thresh);

void cvl_correlate(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_convolve(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_blur_box(const cvl_Mat *src, cvl_Mat *dst, int ksize, bool normalize);

void cvl_blur_mean(const cvl_Mat *src, cvl_Mat *dst, int ksize);

// void cvl_blur_gauss(cvl_Mat *src, cvl_Mat *dst, double sigma);

void cvl_blur_median(const cvl_Mat *src, cvl_Mat *dst, int ksize);

// todo: remove these guys (opt for convert_to)

void cvl_convert_to_u8(cvl_Mat *src, cvl_Mat *dst);

void cvl_convert_to_f64(cvl_Mat *src, cvl_Mat *dst);

// ================
// Convenience "_new" wrappers
// ================

cvl_Mat cvl_cvt_color_new(const cvl_Mat *src, int code);

cvl_Mat cvl_cvt_depth_new(const cvl_Mat *src, cvl_depth_t ddepth, double alpha, double beta);

cvl_Mat cvl_threshold_new(const cvl_Mat *src, int thresh, int maxval, int type);

cvl_Mat cvl_correlate_new(const cvl_Mat *src, cvl_Mat *kernel);

cvl_Mat cvl_convolve_new(const cvl_Mat *src, cvl_Mat *kernel);

cvl_Mat cvl_blur_box_new(const cvl_Mat *src, int ksize, bool normalize);

cvl_Mat cvl_blur_mean_new(const cvl_Mat *src, int ksize);

// cvl_Mat cvl_blur_gauss_new(const cvl_Mat *src, double sigma);

cvl_Mat cvl_blur_median_new(const cvl_Mat *src, int ksize);

#ifdef __cplusplus
}
#endif

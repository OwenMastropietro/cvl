// A Small Computer Vision Library.

#pragma once

#include "cvl_core.h"

void cvl_cvtcolor(cvl_Mat *src, cvl_Mat *dst, int code);

void cvl_threshold(cvl_Mat *img, int thresh, int maxval, int type);

void cvl_add_noise(cvl_Mat *img, double p);

void cvl_rotate(cvl_Mat *img);

void cvl_invert(cvl_Mat *img, int maxval);

void cvl_expand(cvl_Mat *img);

void cvl_shrink(cvl_Mat *img);

int cvl_connected_components(cvl_Mat *img, cvl_Mat *labels, int connectivity);

int cvl_color_components(cvl_Mat *img, cvl_Mat *labels, int thresh);

void cvl_correlate(cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_convolve(cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

void cvl_blur(cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_median_blur(cvl_Mat *src, cvl_Mat *dst, int ksize);

void cvl_convert_to_u8(cvl_Mat *src, cvl_Mat *dst);

void cvl_convert_to_f64(cvl_Mat *src, cvl_Mat *dst);

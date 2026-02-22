// A Small Computer Vision Library.

#pragma once

#include "../netpbm/netpbm.h"

#define BLACK 0   // off - "absense of all color" - background
#define WHITE 255 // on - "presence of all color" - foreground

void cvl_threshold(Image *img, int thresh);

void cvl_add_noise(Image *img, double p);

void cvl_rotate(Image *img);

void cvl_invert(Image *img, int maxval);

void cvl_expand(Image *img);

void cvl_shrink(Image *img);

int cvl_connected_components(Image *img, Matrix *labels, int connectivity);

int cvl_color_components(Image *img, Matrix *labels, int thresh);

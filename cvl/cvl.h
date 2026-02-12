// A Small Computer Vision Library.

#pragma once

#include "../netpbm/netpbm.h"

#define BLACK 0   // off - "absense of all color"
#define WHITE 255 // on - "presence of all color"

void cvl_threshold(Image *img, int thresh);

void cvl_add_noise(Image *img, double p);

void cvl_rotate(Image *img);

void cvl_invert(Image *img, int maxval);

void cvl_expand(Image *img);

void cvl_shrink(Image *img);

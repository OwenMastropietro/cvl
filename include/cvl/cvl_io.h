/**
 * Functions for reading and writing binary PBM, PGM, and PPM image files.
 *
 * Based on template from (V2.2 by Marc Pomplun on 10/19/2013).
 */

#pragma once

#include <cvl/cvl_core.h>
#include <cvl/cvl_error.h>

#ifdef __cplusplus
extern "C" {
#endif

Image cvl_imread(const char *filename);

cvl_status_t cvl_imwrite(const char *filename, Image *img);

#ifdef __cplusplus
}
#endif

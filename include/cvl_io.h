/**
 * Functions for reading and writing binary PBM, PGM, and PPM image files.
 *
 * Based on template from (V2.2 by Marc Pomplun on 10/19/2013).
 */

#pragma once

#include "cvl_core.h"

cvl_Mat cvl_imread(const char *filename);

int cvl_imwrite(const char *filename, cvl_Mat *img);

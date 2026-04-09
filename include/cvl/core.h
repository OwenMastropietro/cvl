/**
 * Functions for reading and writing binary PBM, PGM, and PPM image files.
 *
 * Based on template from (V2.2 by Marc Pomplun on 10/19/2013).
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define BLACK 0   // off - "absense of all color"
#define WHITE 255 // on - "presence of all color"
#define PI 3.14159265358979323846
#define NO_CHANGE -1
#define INVERT -2
#define SQR(x) ((x) * (x))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
// Returns mat.at(r, c, ch).
#define CVL_AT_F64(mat, r, c, ch) \
    ((double*)((uint8_t*)(mat)->data + (r)*(mat)->stride))[(c)*(mat)->channels + (ch)]
// Returns mat.at(r, c, ch).
#define CVL_AT_U8(mat, r, c, ch) \
    ((uint8_t*)((uint8_t*)(mat)->data + (r)*(mat)->stride))[(c)*(mat)->channels + (ch)]

// internal representation for matrices (images)
typedef struct {
    int height;
    int width;
    int channels;
    int depth;
    size_t stride;
    void *data;
} cvl_Mat;

// image formats
typedef enum cvl_format_t {
    CVL_FMT_UNKNOWN,
    CVL_FMT_PBM,
    CVL_FMT_PGM,
    CVL_FMT_PPM,
} cvl_format_t;

// pixel depth
typedef enum cvl_depth_t {
    CVL_UINT8,   // 8-bit unsigned integer (todo: rename: CVL_8U)
    CVL_32S,     // 32-bit signed integer
    CVL_FLOAT32, // 32-bit floating point
    CVL_FLOAT64, // 64-bit floating point
} cvl_depth_t;

// color conversion codes
typedef enum cvl_color_conversion {
    CVL_COLOR_NONE,
    CVL_COLOR_RGB2GRAY,
    CVL_COLOR_BGR2GRAY,
    CVL_COLOR_GRAY2RGB,
    CVL_COLOR_GRAY2BGR,
} cvl_color_conversion;

// thresholding modes
typedef enum cvl_thresh_type {
    CVL_THRESH_BINARY,
    CVL_THRESH_BINARY_INV,
    CVL_THRESH_TRUNC,
    CVL_THRESH_TOZERO,
    CVL_THRESH_TOZERO_INV,
} cvl_thresh_type;

#ifdef __cplusplus
extern "C" {
#endif

size_t cvl_elem_size(cvl_depth_t depth);

cvl_Mat cvl_mat_create(int height, int width, int channels, cvl_depth_t depth);

cvl_Mat cvl_mat_create_fill(int height, int width, int channels, cvl_depth_t depth, const void *fill_value);

cvl_Mat cvl_mat_create_from(int height, int width, int channels, cvl_depth_t depth, const void *data);

cvl_Mat cvl_mat_copy(const cvl_Mat *src);

void cvl_mat_free(cvl_Mat *mat);

// Accessors

uint8_t *cvl_mat_row(cvl_Mat *mat, int r);

uint8_t *cvl_row_u8(cvl_Mat *mat, int r);

double *cvl_row_f64(cvl_Mat *mat, int r);

int32_t *cvl_row_i32(cvl_Mat *mat, int r);

#ifdef __cplusplus
}
#endif

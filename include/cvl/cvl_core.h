#pragma once

#include <stdint.h>

#define BLACK 0   // off - "absense of all color"
#define WHITE 255 // on - "presence of all color"
#define SQR(x) ((x) * (x))
#define PI 3.14159265358979323846
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define NO_CHANGE -1
#define INVERT -2

typedef struct Point {
    int x; // horizonal (column)
    int y; // vertical (row)
} Point;

typedef struct {
    uint8_t r, g, b, i;
} Pixel;

typedef struct {
    int height, width;
    Pixel **map;
} Image;

typedef struct {
    int height, width;
    double **map;
} Matrix;

// image formats
typedef enum cvl_format_t {
    CVL_FMT_UNKNOWN,
    CVL_FMT_PBM,
    CVL_FMT_PGM,
    CVL_FMT_PPM,
} cvl_format_t;

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

// Image

Image cvl_img_create(int height, int width);
Image cvl_img_create_fill(int height, int width, int value);
Image cvl_img_copy(Image *src);
void cvl_img_free(Image img);

void cvl_set_pixel(Image img, int row, int col, int r, int g, int b, int i);
Image cvl_mat2img(Matrix mx, int scale, double gamma);
Matrix cvl_img2mat(Image img);

// Matrix

Matrix cvl_mat_create(int height, int width);
Matrix cvl_mat_create_fill(int height, int width, double fill_value);
Matrix cvl_mat_create_from(double *entry, int height, int width);
Matrix cvl_mat_copy(const Matrix *src);
void cvl_mat_free(Matrix mx);

void cvl_mat_add(const Matrix *a, const Matrix *b, Matrix *dst);
void cvl_mat_sub(const Matrix *a, const Matrix *b, Matrix *dst);
void cvl_mat_mul(const Matrix *a, const Matrix *b, Matrix *dst);
void cvl_mat_scale(Matrix *m, double scalar);
void cvl_mat_hadamard(const Matrix *a, const Matrix *b, Matrix *dst);
void cvl_mat_transpose(const Matrix *src, Matrix *dst);

void cvl_mat_square(const Matrix *src, Matrix *dst);
void cvl_outer(const Matrix *a, const Matrix *b, Matrix *dst);
void cvl_outer_arr(const double *a, int m, const double *b, int n, Matrix *dst);
void cvl_mat_abs(Matrix *src);

// Convenience "_new" helpers.

Matrix cvl_mat_add_new(const Matrix *a, const Matrix *b);
Matrix cvl_mat_sub_new(const Matrix *a, const Matrix *b);
Matrix cvl_mat_mul_new(const Matrix *a, const Matrix *b);
Matrix cvl_mat_scale_new(const Matrix *m, double scalar);
Matrix cvl_mat_hadamard_new(const Matrix *a, const Matrix *b);
Matrix cvl_mat_transpose_new(const Matrix *src);

Matrix cvl_outer_new(const Matrix *a, const Matrix *b);
Matrix cvl_outer_arr_new(const double *a, int m, const double *b, int n);
Matrix cvl_mat_abs_new(const Matrix *src);
Matrix cvl_mat_square_new(const Matrix *src);

#ifdef __cplusplus
}
#endif

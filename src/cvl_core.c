#define _CRT_SECURE_NO_WARNINGS

#include <cvl/cvl_core.h>
#include <cvl/cvl_io.h>

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================
// IMAGE
// ================

// Create a new image of the given size and fill it with white pixels.
Image cvl_img_create(int height, int width) {
    // todo: zero-fill
    return cvl_img_create_fill(height, width, WHITE);
}

// Create a new image of the given size and fill it with pixel value.
Image cvl_img_create_fill(int height, int width, int pixel_value) {
    int i, j;
    Image img;

    img.height = height;
    img.width = width;

    img.map = (Pixel **)malloc(sizeof(Pixel *) * height);
    for (i = 0; i < height; i++) {
        img.map[i] = (Pixel *)malloc(sizeof(Pixel) * width);
        for (j = 0; j < width; j++) {
            img.map[i][j].r = pixel_value;
            img.map[i][j].g = pixel_value;
            img.map[i][j].b = pixel_value;
            img.map[i][j].i = pixel_value;
        }
    }

    return img;
}

// Copy an input image.
Image cvl_img_copy(Image *src) {
    Image dst = cvl_img_create(src->height, src->width);

    for (int i = 0; i < src->height; ++i) {
        for (int j = 0; j < src->width; ++j) {
            dst.map[i][j] = src->map[i][j];
        }
    }

    return dst;
}

// Delete a previously created image and free its allocated memory on the heap.
void cvl_img_free(Image img) {
    for (int i = 0; i < img.height; ++i) {
        free(img.map[i]);
    }
    free(img.map);
}

// Convert the intensity components of an image into a matrix of identical size.
Matrix cvl_img2mat(Image img) {
    int m, n;
    Matrix result = cvl_mat_create(img.height, img.width);

    for (m = 0; m < img.height; m++) {
        for (n = 0; n < img.width; n++) {
            result.map[m][n] = (double)img.map[m][n].i;
        }
    }

    return result;
}

// ================
// MATRIX
// ================

// Create a new matrix of the given size and fill it with zeroes.
Matrix cvl_mat_create(int height, int width) {
    return cvl_mat_create_fill(height, width, 0.0);
}

// Create a new matrix of the given size and fill it with `fill_value`.
Matrix cvl_mat_create_fill(int height, int width, double fill_value) {
    Matrix m = {
        .height = 0,
        .width  = 0,
        .map    = NULL,
    };

    if (height <= 0 || width <= 0) return m;

    m.height = height;
    m.width  = width;
    m.map    = (double **)malloc(sizeof(double *) * height);

    for (int i = 0; i < height; i++) {
        m.map[i] = (double *)malloc(sizeof(double) * width);
        for (int j = 0; j < width; j++) {
            m.map[i][j] = fill_value;
        }
    }

    return m;
}

// Create a new matrix of the given size and fill it with content of 2D double array.
Matrix cvl_mat_create_from(double *entry, int height, int width) {
    // todo: cvl_mat_create_from(h, w, const values)
    Matrix m = cvl_mat_create(height, width);

    if (!entry || m.map == NULL) return m;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            m.map[i][j] = entry[i * width + j];
        }
    }

    return m;
}

// Copy an input matrix.
Matrix cvl_mat_copy(const Matrix *src) {
    const int h = src->height;
    const int w = src->width;

    Matrix dst = cvl_mat_create(h, w);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            dst.map[i][j] = src->map[i][j];
        }
    }

    return dst;
}

// Delete a previously created matrix and free its allocated memory on the heap.
void cvl_mat_free(Matrix mat) {
    // todo: cvl_mat_free(Matrix *mat)
    // if (!mat || !mat.map) return;

    for (int i = 0; i < mat.height; ++i) {
        free(mat.map[i]);
    }
    free(mat.map);

    // mat->map = NULL
    // mat->height = 0
    // mat->width = 0
}

// Element-wise sum (a + b).
void cvl_mat_add(const Matrix *a, const Matrix *b, Matrix *dst) {
    const int h = a->height;
    const int w = a->width;

    assert(b->height == h && b->width == w && dst->height == h && dst->width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            dst->map[i][j] = a->map[i][j] + b->map[i][j];
        }
    }
}

// Element-wise sum (a + b).
Matrix cvl_mat_add_new(const Matrix *a, const Matrix *b) {
    Matrix dst = cvl_mat_create(a->height, a->width);
    cvl_mat_add(a, b, &dst);
    return dst;
}

// Element-wise difference (a - b).
void cvl_mat_sub(const Matrix *a, const Matrix *b, Matrix *dst) {
    const int h = a->height;
    const int w = a->width;

    assert(b->height == h && b->width == w && dst->height == h && dst->width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            dst->map[i][j] = a->map[i][j] - b->map[i][j];
        }
    }
}

// Element-wise difference (a - b).
Matrix cvl_mat_sub_new(const Matrix *a, const Matrix *b) {
    Matrix dst = cvl_mat_create(a->height, a->width);
    cvl_mat_sub(a, b, &dst);
    return dst;
}

// Standard matrix multiplication - (mxn) * (nxp) = (mxp).
void cvl_mat_mul(const Matrix *a, const Matrix *b, Matrix *dst) {
    const int m = a->height;
    const int n = a->width;
    const int p = b->width;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            dst->map[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                dst->map[i][j] += a->map[i][k] * b->map[k][j];
            }
        }
    }
}

// Standard matrix multiplication - (mxn) * (nxp) = (mxp).
Matrix cvl_mat_mul_new(const Matrix *a, const Matrix *b) {
    Matrix dst = cvl_mat_create(a->height, b->width);
    cvl_mat_mul(a, b, &dst);
    return dst;
}

// Element-wise scaling (m * scalar).
void cvl_mat_scale(Matrix *m, double scalar) {
    const int h = m->height;
    const int w = m->width;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            m->map[i][j] = m->map[i][j] * scalar;
        }
    }
}

// Element-wise scaling (m * scalar).
Matrix cvl_mat_scale_new(const Matrix *m, double scalar) {
    Matrix dst = cvl_mat_copy(m);
    cvl_mat_scale(&dst, scalar);
    return dst;
}

// Element-wise product (a * b).
void cvl_mat_hadamard(const Matrix *a, const Matrix *b, Matrix *dst) {
    const int h = a->height;
    const int w = a->width;

    assert(b->height == h && b->width == w && dst->height == h && dst->width == w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            dst->map[i][j] = a->map[i][j] * b->map[i][j];
        }
    }
}

// Element-wise product (a * b).
Matrix cvl_mat_hadamard_new(const Matrix *a, const Matrix *b) {
    Matrix dst = cvl_mat_create(a->height, a->width);
    cvl_mat_hadamard(a, b, &dst);
    return dst;
}

// Transposes a matrix (mxn) --> (nxm).
void cvl_mat_transpose(const Matrix *src, Matrix *dst) {
    assert(src->height == dst->width && src->width == dst->height);
    const int h = dst->height;
    const int w = dst->width;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            dst->map[i][j] = src->map[j][i];
        }
    }
}

// Transposes a matrix (mxn) --> (nxm).
Matrix cvl_mat_transpose_new(const Matrix *src) {
    Matrix dst = cvl_mat_create(src->width, src->height);
    cvl_mat_transpose(src, &dst);
    return dst;
}

void cvl_mat_square(const Matrix *src, Matrix *dst) {
    const int h = src->height;
    const int w = src->width;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            dst->map[i][j] = src->map[i][j] * src->map[i][j];
        }
    }
}

Matrix cvl_mat_square_new(const Matrix *src) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_mat_square(src, &dst);
    return dst;
}

void cvl_outer(const Matrix *a, const Matrix *b, Matrix *dst) {
    const int m = a->height * a->width;
    const int n = b->height * b->width;

    for (int i = 0; i < m; ++i) {
        int ai = i / a->width;
        int aj = i % a->width;
        double va = a->map[ai][aj];

        for (int j = 0; j < n; ++j) {
            int bi = j / b->width;
            int bj = j % b->width;
            double vb = b->map[bi][bj];

            dst->map[i][j] = va * vb;
        }
    }
}

Matrix cvl_outer_new(const Matrix *a, const Matrix *b) {
    const int m = a->height * a->width;
    const int n = b->height * b->width;
    Matrix dst = cvl_mat_create(m, n);
    cvl_outer(a, b, &dst);
    return dst;
}

void cvl_outer_arr(const double *a, int m, const double *b, int n, Matrix *dst) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            dst->map[i][j] = a[i] * b[j];
        }
    }
}

Matrix cvl_outer_arr_new(const double *a, int m, const double *b, int n) {
    Matrix dst = cvl_mat_create(m, n);
    cvl_outer_arr(a, m, b, n, &dst);
    return dst;
}

// Returns element-wise |src|.
void cvl_mat_abs(Matrix *src) {
    const int h = src->height;
    const int w = src->width;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            src->map[i][j] = fabs(src->map[i][j]);
        }
    }
}

// Returns element-wise |src|.
Matrix cvl_mat_abs_new(const Matrix *src) {
    Matrix dst = cvl_mat_copy(src);
    cvl_mat_abs(&dst);
    return dst;
}

// Converts a matrix to an image with scaling and gamma correction.
// - scale == 0: values are 1/255 normalized before applying gamma.
// - scale != 0: values are min/max normalized before applying gamma.
// - gamma == 1.0: linear scaling (no change in contrast).
// - gamma < 1.0: enhances darker values (brightens the image).
// - gamma > 1.0: suppressess darker values (darkens the image).
//
// The final result is clamped to [0, 255].
Image cvl_mat2img(Matrix mat, int scale, double gamma) {
    const int h = mat.height;
    const int w = mat.width;

    double vmin = DBL_MAX;
    double vmax = -DBL_MAX;

    if (scale) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                double v = mat.map[i][j];

                if (v < vmin) vmin = v;
                if (v > vmax) vmax = v;
            }
        }
        if (vmax - vmin < 1e-10) vmax += 1.0;
    }

    double range = vmax - vmin;

    Image img = cvl_img_create(h, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel *p = &img.map[i][j];
            double v = mat.map[i][j];

            double norm = scale ? (v - vmin) / range : v / 255.0;
            double out  = 255.0 * pow(norm, gamma);

            out = fmin(fmax(out, 0.0), 255.0); // clamp to [0, 255]

            uint8_t val = (uint8_t)(out + 0.5);

            p->r = p->g = p->b = p->i = val;
        }
    }

    return img;
}

/**
 * Set the color for pixel (row=y, col=x) in the image.
 * 
 * Params r, g, b, and i accept:
 * - -2 to ignore - keeps previously stored channel value.
 * - -1 to invert - inverts channel value (255 - v).
 * - [0, 255] to change color - sets channel value to new value.
 */
void cvl_set_pixel(Image img, int row, int col, int r, int g, int b, int i) {
    if (img.map == NULL) return;
    if ((unsigned)row >= img.height || (unsigned)col >= img.width) return;

    Pixel *p = &img.map[row][col];

    if (r == INVERT)             p->r = 255 - p->r;
    else if ((unsigned)r <= 255) p->r = r;

    if (g == INVERT)             p->g = 255 - p->g;
    else if ((unsigned)g <= 255) p->g = g;

    if (b == INVERT)             p->b = 255 - p->b;
    else if ((unsigned)b <= 255) p->b = b;

    if (i == INVERT)             p->i = 255 - p->i;
    else if ((unsigned)i <= 255) p->i = i;
}

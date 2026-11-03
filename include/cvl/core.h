/**
 * @file core.h
 * @brief Core types, constants, and matrix operations.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
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

/**
 * @brief A unified matrix type for representing images, kernels, or other arbitrary matrix-like structures.
 *
 * Rows are stored consecutively, with channels belonging to the same element
 * stored next to one another.
 * The address of row `r` is: `(uint8_t *)mat->data + r * mat->stride`.
 *
 * The same type represents both images and other matrix data, such as
 * floating-point kernels. For example, an 8-bit image and a 64-bit kernel are
 * both cvl_Mat, distinguished by `depth` and `channels`.
 */
typedef struct {
    int height;    /**< Number of rows. */
    int width;     /**< Number of columns. */
    int channels;  /**< Number of channels per element. For example, an RGB image has three channels. */
    int depth;     /**< Data type used to represent each channel. See @ref cvl_depth_t. */
    size_t stride; /**< Number of bytes from one row to the next. */
    void *data;    /**< Pointer to the first byte of matrix storage. */
} cvl_Mat;

/**
 * Enumeration of supported image formats.
 *
 * | Value             | Extension | Description                                                                                                                                        |
 * | ----------------- | --------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
 * | CVL_FMT_UNKNOWN   | -         | Unknown or unsupported image format.                                                                                                               |
 * | CVL_FMT_PBM       | .pbm      | [Netpbm](https://netpbm.sourceforge.net/) Portable Bitmap format for binary images.                                                                |
 * | CVL_FMT_PGM       | .pgm      | [Netpbm](https://netpbm.sourceforge.net/) Portable Graymap format for grayscale images.                                                            |
 * | CVL_FMT_PPM       | .ppm      | [Netpbm](https://netpbm.sourceforge.net/) Portable Pixmap format for colored images.                                                               |
 * | CVL_FMT_PNG       | .png      | Portable Network Graphics, supported via [stb](https://github.com/nothings/stb).                                                                   |
 * | CVL_FMT_JPG       | .jpg      | Joint Photographic Experts Group, supported via [stb](https://github.com/nothings/stb). `JPEG` is a redundant verbocity that will not be condoned. |
 *
 * The PBM, PGM, and PPM formats are supported natively.
 */
typedef enum cvl_format_t {
    CVL_FMT_UNKNOWN,
    CVL_FMT_PBM,
    CVL_FMT_PGM,
    CVL_FMT_PPM,
    CVL_FMT_PNG,
    CVL_FMT_JPG,
} cvl_format_t;

/**
 * Enumeration of supported data types used to represent each matrix channel.
 * 
 * See @ref cvl_cvt_depth() for converting between supported depths.
 */
typedef enum cvl_depth_t {
    CVL_UINT8,   /**< 8-bit unsigned integer (todo: rename: CVL_8U) */
    CVL_32S,     /**< 32-bit signed integer */
    CVL_FLOAT32, /**< 32-bit floating point */
    CVL_FLOAT64, /**< 64-bit floating point */
} cvl_depth_t;

/**
 * Enumeration of supported color-space conversions.
 *
 * See @ref cvl_cvt_color() for converting between supported color spaces.
 *
 * | Value              | Source Channels | Destination Channels | Description |
 * |--------------------|-----------------|----------------------|-------------|
 * | CVL_COLOR_NONE     | —               | —                    | No color conversion. |
 * | CVL_COLOR_RGB2GRAY | 3               | 1                    | Converts RGB color to grayscale. |
 * | CVL_COLOR_BGR2GRAY | 3               | 1                    | Converts BGR color to grayscale. |
 * | CVL_COLOR_GRAY2RGB | 1               | 3                    | Copies the grayscale value into each RGB channel. |
 * | CVL_COLOR_GRAY2BGR | 1               | 3                    | Copies the grayscale value into each BGR channel. |
 */
typedef enum cvl_color_conversion {
    CVL_COLOR_NONE,
    CVL_COLOR_RGB2GRAY,
    CVL_COLOR_BGR2GRAY,
    CVL_COLOR_GRAY2RGB,
    CVL_COLOR_GRAY2BGR,
} cvl_color_conversion;

/**
 * Enumeration of supported thresholding operations
 *
 * See @ref cvl_threshold() for applying thresholds.
 *
 * | Mode                  | Operation |
 * |-----------------------|-----------|
 * | CVL_THRESH_BINARY     | `src > thresh ? maxval : 0` |
 * | CVL_THRESH_BINARY_INV | `src > thresh ? 0 : maxval` |
 * | CVL_THRESH_TRUNC      | `src > thresh ? thresh : src` |
 * | CVL_THRESH_TOZERO     | `src > thresh ? src : 0` |
 * | CVL_THRESH_TOZERO_INV | `src > thresh ? 0 : src` |
 */
typedef enum cvl_thresh_type {
    CVL_THRESH_BINARY,
    CVL_THRESH_BINARY_INV,
    CVL_THRESH_TRUNC,
    CVL_THRESH_TOZERO,
    CVL_THRESH_TOZERO_INV,
} cvl_thresh_type;

/**
 * Enumeration of supported interpolation methods.
 */
typedef enum cvl_interp_t {
    CVL_INTER_NEAREST, /**< Nearest-neighbor interpolation. */
    CVL_INTER_LINEAR,  /**< Bilinear interpolation. */
} cvl_interp_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the size, in bytes, of a matrix data type.
 * 
 * @param depth Data type. See @ref cvl_depth_t.
 * @return Size of one channel element in bytes.
 */
size_t cvl_elem_size(cvl_depth_t depth);

/**
 * Creates an empty matrix.
 * 
 * Equivalent to calling the designated initializer.
 *
 * @return An empty @ref cvl_Mat with no allocated matrix storage.
 */
cvl_Mat cvl_mat_empty(void);

/**
 * Creates a zero-initialized matrix.
 *
 * @param height Number of rows.
 * @param width Number of columns.
 * @param channels Number of channels per element.
 * @param depth Data type used to represent each channel. See @ref cvl_depth_t.
 * @return The newly allocated matrix.
 */
cvl_Mat cvl_mat_create(int height, int width, int channels, cvl_depth_t depth);

/**
 * Creates a matrix initialized with a fill value.
 *
 * Initializes every matrix channel using the supplied fill value.
 *
 * @param height Number of rows.
 * @param width Number of columns.
 * @param channels Number of channels per element.
 * @param depth Data type used to represent each channel. See @ref cvl_depth_t.
 * @param fill_value Pointer to the value used to initialize each channel.
 * @return The newly allocated matrix, or an empty matrix on failure.
 */
cvl_Mat cvl_mat_create_fill(int height, int width, int channels, cvl_depth_t depth, const void *fill_value);

/**
 * Creates a matrix initialized from existing data.
 *
 * Initializes the matrix using the supplied data buffer.
 *
 * @param height Number of rows.
 * @param width Number of columns.
 * @param channels Number of channels per element.
 * @param depth Data type used to represent each channel. See @ref cvl_depth_t.
 * @param data Pointer to the source data.
 * @return The newly allocated matrix, or an empty matrix on failure.
 * 
 * @todo Fold into @ref cvl_mat_create_fill().
 */
cvl_Mat cvl_mat_create_from(int height, int width, int channels, cvl_depth_t depth, const void *data);

/**
 * Creates a copy of an existing matrix.
 *
 * @param src Pointer to the source matrix.
 * @return The copied matrix, or an empty matrix on failure.
 */
cvl_Mat cvl_mat_copy(const cvl_Mat *src);

/**
 * Releases the storage associated with a matrix.
 *
 * Frees the matrix's allocated data and resets its state.
 *
 * @param mat Pointer to the matrix to release.
 */
void cvl_mat_free(cvl_Mat *mat);

// Accessors

/**
 * Returns a pointer to the beginning of a matrix row.
 *
 * The returned pointer addresses the first byte of row `r`.
 * Use the matrix's `stride` to determine the distance between rows.
 *
 * @param mat Pointer to the matrix.
 * @param r Row index.
 * @return Pointer to the first byte of the requested row.
 */
uint8_t *cvl_mat_row(cvl_Mat *mat, int r);

/**
 * Returns a constant pointer to the beginning of a matrix row.
 *
 * The returned pointer addresses the first byte of row `r`.
 * Use the matrix's `stride` to determine the distance between rows.
 *
 * @param mat Pointer to the matrix.
 * @param r Row index.
 * @return Constant pointer to the first byte of the requested row.
 */
const uint8_t *cvl_mat_row_const(const cvl_Mat *mat, int r);

#ifdef __cplusplus
}
#endif

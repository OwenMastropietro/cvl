/**
 * @file error.h
 * @brief Error handling, status codes and matrix predicates.
 */

#pragma once

#include <cvl/core.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum cvl_status {
    CVL_OK = 0,     /**< A great success. */
    CVL_ERR_NULL,   /**< A required pointer was NULL. */
    CVL_ERR_RANGE,  /**< A size, channel count, depth, or parameter was outside the values the function accepts. */
    CVL_ERR_NOMEM,  /**< An allocation failed. */
    CVL_ERR_IO,     /**< Opening, reading, or writing a file failed.  */
    CVL_ERR_FORMAT, /**< The file is not a supported image. */
    CVL_ERR_STATE,  /**< The matrices exist, but they cannot be used together. */
} cvl_status;

/**
 * Checks whether `mat` has storage and non-negative dimensions.
 *
 * @param mat Matrix to check.
 * @return True when `mat` and `mat->data` are non-NULL and the height, width,
 * and channel count are non-negative.
 */
bool cvl_mat_ok(const cvl_Mat *mat);

/**
 * Checks whether two matrices have the same height and width.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True when the heights and widths are equal.
 */
bool cvl_mat_same_size(const cvl_Mat *a, cvl_Mat *b);

/**
 * Checks whether two matrices have the same height, width, and channel count.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @return True when the size and channel count are equal.
 */
bool cvl_mat_same_shape(const cvl_Mat *a, cvl_Mat *b);

#ifdef __cplusplus
}
#endif

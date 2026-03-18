#define _CRT_SECURE_NO_WARNINGS

#include "cvl_core.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t cvl_elem_size(int depth) {
    switch (depth) {
        case CVL_INT32: return sizeof(int);
        case CVL_UINT8: return sizeof(uint8_t);
        case CVL_FLOAT32: return sizeof(float);
        case CVL_FLOAT64: return sizeof(double);
        default: assert(0);
    }

    return 1; // todo
}

cvl_Mat cvl_mat_create(int height, int width, int channels, int depth) {
    assert(height > 0 && width > 0 && channels > 0);

    cvl_Mat m;
    m.height = height;
    m.width = width;
    m.channels = channels;
    m.depth = depth;

    size_t elem_size = cvl_elem_size(depth);
    size_t total_bytes = (size_t)height * width * channels * elem_size;

    m.stride = width * channels * elem_size;
    m.data = malloc(total_bytes);
    assert(m.data);

    memset(m.data, BLACK, total_bytes);
    
    return m;
}

void cvl_mat_free(cvl_Mat *m) {
    free(m->data);
    m->data = NULL;
}

// Returns pointer to row in matrix.
double *cvl_row_f64(cvl_Mat *mat, int r) {
    assert(mat->depth == CVL_FLOAT64);
    return (double *)((uint8_t *)mat->data + r * mat->stride);
}

// Returns pointer to row in matrix.
uint8_t *cvl_row_u8(cvl_Mat *mat, int r) {
    assert(mat->depth == CVL_UINT8);
    return (uint8_t *)mat->data + r * mat->stride;
}

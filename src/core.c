#define _CRT_SECURE_NO_WARNINGS

#include <cvl/core.h>

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t cvl_elem_size(cvl_depth_t depth) {
    switch (depth) {
        case CVL_INT32:   return sizeof(int);
        case CVL_UINT8:   return sizeof(uint8_t);
        case CVL_FLOAT32: return sizeof(float);
        case CVL_FLOAT64: return sizeof(double);
    }
    assert(0 && "Unknown depth");
    return 0;
}

cvl_Mat cvl_mat_create(int height, int width, int channels, cvl_depth_t depth) {
    assert(height > 0 && width > 0 && channels > 0);

    size_t elem_size = cvl_elem_size(depth);

    cvl_Mat m;
    m.height = height;
    m.width = width;
    m.channels = channels;
    m.depth = depth;
    m.stride = width * channels * elem_size;

    size_t total_bytes = (size_t)height * m.stride;
    m.data = calloc(1, total_bytes);
    assert(m.data);

    return m;
}

cvl_Mat cvl_mat_create_fill(int height, int width, int channels, cvl_depth_t depth, const void *fill_value) {
    // todo: opencv uses m.setTo(v) or setTo(scalar{r, g, b})
    // todo: I don't check depth and type of fill_value match... good luck
    assert(height > 0 && width > 0 && channels > 0);
    assert(fill_value != NULL);

    cvl_Mat mat = cvl_mat_create(height, width, channels, depth);
    assert(mat.data);

    size_t elem_size = cvl_elem_size(depth);
    size_t total_elems = (size_t)height * width * channels;

    uint8_t *data = (uint8_t *)mat.data;
    for(size_t i = 0; i < total_elems; ++i) {
        memcpy(data + i * elem_size, fill_value, elem_size);
    }

    return mat;
}

cvl_Mat cvl_mat_create_from(int height, int width, int channels, cvl_depth_t depth, const void *data) {
    assert(height > 0 && width > 0 && channels > 0);
    assert(data);

    cvl_Mat mat = cvl_mat_create(height, width, channels, depth);
    assert(mat.data);

    size_t total_bytes = (size_t)height * mat.stride;
    memcpy(mat.data, data, total_bytes);

    return mat;
}

cvl_Mat cvl_mat_copy(const cvl_Mat *src) {
    cvl_Mat dst;
    dst.height = src->height;
    dst.width = src->width;
    dst.channels = src->channels;
    dst.depth = src->depth;
    dst.stride = src->stride;

    dst.data = malloc(dst.height * dst.stride);
    assert(dst.data);

    memcpy(dst.data, src->data, dst.height * dst.stride);

    return dst;
}

void cvl_mat_free(cvl_Mat *m) {
    free(m->data);
    m->data = NULL;
}

// Returns raw byte pointer to the start of row r.
uint8_t *cvl_mat_row(cvl_Mat *mat, int r) {
    assert(mat != NULL);
    assert(mat->data != NULL);
    assert(0 <= r && r < mat->height);

    return (uint8_t *)mat->data + (size_t)r * mat->stride;
}

uint8_t *cvl_row_u8(cvl_Mat *mat, int r) {
    assert(mat->depth == CVL_UINT8);
    return cvl_mat_row(mat, r);
}

double *cvl_row_f64(cvl_Mat *mat, int r) {
    assert(mat->depth == CVL_FLOAT64);
    return (double *)cvl_mat_row(mat, r);
}

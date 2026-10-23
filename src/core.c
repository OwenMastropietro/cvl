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
        case CVL_UINT8:   return sizeof(uint8_t);
        case CVL_32S:     return sizeof(int32_t);
        case CVL_FLOAT32: return sizeof(float);
        case CVL_FLOAT64: return sizeof(double);
    }
    assert(0 && "Unknown depth");
    return 0;
}

static cvl_Mat cvl_mat_empty(void) {
    cvl_Mat m;
    m.height = 0;
    m.width = 0;
    m.channels = 0;
    m.depth = CVL_UINT8;
    m.stride = 0;
    m.data = NULL;
    return m;
}

// width * channels * elem_size, or 0 if the product does not fit in size_t.
static size_t cvl_mat_stride(int width, int channels, size_t elem_size) {
    if (width <= 0 || channels <= 0 || elem_size == 0) {
        return 0;
    }

    if ((size_t)width > SIZE_MAX / (size_t)channels) {
        return 0;
    }
    size_t row_elems = (size_t)width * (size_t)channels;

    if (row_elems > SIZE_MAX / elem_size) {
        return 0;
    }
    return row_elems * elem_size;
}

// height * stride, or 0 if either factor is 0 or the product overflows.
static size_t cvl_mat_bytes(int height, size_t stride) {
    if (height <= 0 || stride == 0 || (size_t)height > SIZE_MAX / stride) {
        return 0;
    }
    return (size_t)height * stride;
}

cvl_Mat cvl_mat_create(int height, int width, int channels, cvl_depth_t depth) {
    size_t elem_size = cvl_elem_size(depth);
    size_t stride = cvl_mat_stride(width, channels, elem_size);
    size_t total_bytes = cvl_mat_bytes(height, stride);
    if (total_bytes == 0) {
        return cvl_mat_empty();
    }

    void *data = calloc(1, total_bytes);
    if (!data) {
        return cvl_mat_empty();
    }

    cvl_Mat m;
    m.height = height;
    m.width = width;
    m.channels = channels;
    m.depth = depth;
    m.stride = stride;
    m.data = data;
    return m;
}

cvl_Mat cvl_mat_create_fill(int height, int width, int channels, cvl_depth_t depth, const void *fill_value) {
    // todo: opencv uses m.setTo(v) or setTo(scalar{r, g, b})
    // todo: I don't check depth and type of fill_value match... good luck
    if (!fill_value) {
        return cvl_mat_empty();
    }

    cvl_Mat mat = cvl_mat_create(height, width, channels, depth);
    if (!mat.data) {
        return mat;
    }

    size_t elem_size = cvl_elem_size(depth);
    size_t total_elems = (size_t)height * (size_t)width * (size_t)channels;

    uint8_t *data = (uint8_t *)mat.data;
    for(size_t i = 0; i < total_elems; ++i) {
        memcpy(data + i * elem_size, fill_value, elem_size);
    }

    return mat;
}

cvl_Mat cvl_mat_create_from(int height, int width, int channels, cvl_depth_t depth, const void *data) {
    if (!data) {
        return cvl_mat_empty();
    }

    cvl_Mat mat = cvl_mat_create(height, width, channels, depth);
    if (!mat.data) {
        return mat;
    }

    memcpy(mat.data, data, (size_t)height * mat.stride);

    return mat;
}

cvl_Mat cvl_mat_copy(const cvl_Mat *src) {
    if (!src || !src->data) {
        return cvl_mat_empty();
    }

    size_t total_bytes = cvl_mat_bytes(src->height, src->stride);
    if (total_bytes == 0) {
        return cvl_mat_empty();
    }

    void *data = malloc(total_bytes);
    if (!data) {
        return cvl_mat_empty();
    }

    memcpy(data, src->data, total_bytes);

    cvl_Mat dst;
    dst.height = src->height;
    dst.width = src->width;
    dst.channels = src->channels;
    dst.depth = src->depth;
    dst.stride = src->stride;
    dst.data = data;
    return dst;
}

void cvl_mat_free(cvl_Mat *m) {
    free(m->data);
    m->data = NULL;
}

// Returns a mutable raw byte pointer to the start of row r.
uint8_t *cvl_mat_row(cvl_Mat *mat, int r) {
    assert(mat && mat->data);
    assert(0 <= r && r < mat->height);

    return (uint8_t *)mat->data + (size_t)r * mat->stride;
}

// Returns an immutable raw byte pointer to the start of row r.
const uint8_t *cvl_mat_row_const(const cvl_Mat *mat, int r) {
    assert(mat && mat->data);
    assert(0 <= r && r < mat->height);

    return (const uint8_t *)mat->data + (size_t)r * mat->stride;
}

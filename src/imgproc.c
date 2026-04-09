#include <cvl/imgproc.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// Helpers
// ==========================

typedef struct {
    int32_t parent;
} UFNode;

// Find root via path compression.
static int32_t uf_find(UFNode *uf, int x) {
    if (uf[x].parent != x) {
        uf[x].parent = uf_find(uf, uf[x].parent);
    }

    return uf[x].parent;
}

// Union two (equivalent) sets by roots.
static void uf_union(UFNode *uf, int32_t a, int32_t b) {
    int32_t ra = uf_find(uf, a);
    int32_t rb = uf_find(uf, b);
    if (ra != rb) {
        uf[rb].parent = ra;
    }
}

// Changes all pixels with a pixel_value neighbor to pixel_value.
static void _morph(cvl_Mat *img, int pixel_value) {
    assert(img && img->data);
    assert(img->channels == 1);
    assert(img->depth == CVL_UINT8);

    const int h = img->height;
    const int w = img->width;
    const size_t stride = img->stride;

    uint8_t *data = img->data;

    bool *mask = calloc(h * w, sizeof(bool));
    assert(mask);

    int dh[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dw[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int n_neighbors = 8;

    // Set Mask - Marking Pixels to Change.
    for (int i = 0; i < h; ++i) {
        uint8_t *row = data + i * stride;

        for (int j = 0; j < w; ++j) {
            if (row[j] == pixel_value) continue;

            for (int k = 0; k < n_neighbors; ++k) {
                int ni = i + dh[k];
                int nj = j + dw[k];

                bool in_bounds = ((0 <= ni && ni < h) && (0 <= nj && nj < w));
                if (!in_bounds) continue;

                uint8_t *nrow = data + ni * stride;
                if (nrow[nj] == pixel_value) {
                    mask[i * w + j] = true;
                    break;
                }
            }
        }
    }

    // Change Pixels According to Mask.
    for (int i = 0; i < h; ++i) {
        uint8_t *row = data + i * stride;

        for (int j = 0; j < w; ++j) {
            if (mask[i * w + j]) {
                row[j] = pixel_value;
            }
        }
    }

    free(mask);
}

// Compares two doubles for qsort ordering.
static int _cmp(const void *a, const void *b) {
    return (*(double *)a - *(double *)b);
}

// Returns the median an array (sorts the array in place).
static double _median(double nums[], int n) {
    qsort(nums, n, sizeof(double), _cmp); // hawk tua

    if (n % 2 == 0) { // average of middle two
        double a = nums[(n - 1) / 2];
        double b = nums[n / 2];
        return (a + b) / 2.0;
    }

    return nums[n / 2];
}

// Clamp x to inclusive range [lo, hi].
static inline int _clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// Clamps and rounds double into uint8_t.
static inline uint8_t cvl_sat_u8_f64(double v) {
    return (v < 0.0) ? 0 : (v > 255.0) ? 255 : (uint8_t)(v + 0.5);
}

// ==========================
// Color & Depth Conversion
// ==========================

void cvl_cvt_color(const cvl_Mat *src, cvl_Mat *dst, int code) {
    assert(src && src->data && dst);
    assert(src->height == dst->height && src->width == dst->width);

    const int height = src->height;
    const int width = src->width;

    const uint8_t *sdata = src->data;
    uint8_t *ddata = dst->data;

    // ITU-R BT.601-7 luminance
    const double lum_r = 0.299;
    const double lum_g = 0.587;
    const double lum_b = 0.114;

    for (int i = 0; i < height; ++i) {
        const uint8_t *srow = sdata + i * src->stride;
        uint8_t *drow = ddata + i * dst->stride;

        switch (code) {
        // case CVL_BGR2RGB:
        // case CVL_RGB2BGR: {
        //     for (int x = 0; x < width; x++) {
        //         const uint8_t *p = srow + x * 3;
        //         uint8_t *q = drow + x * 3;
        //         q[0] = p[2];  // swap r & b
        //         q[1] = p[1];  // g
        //         q[2] = p[0];  // swap r & b
        //     }
        //     break;
        // }
        case CVL_COLOR_RGB2GRAY: {
            for (int j = 0; j < width; ++j) {
                const uint8_t *p = srow + j * 3;
                double gray = lum_r * p[0] + lum_g * p[1] + lum_b * p[2];
                drow[j] = cvl_sat_u8_f64(gray);
            }
            break;
        }
        case CVL_COLOR_BGR2GRAY: {
            for (int j = 0; j < width; ++j) {
                const uint8_t *p = srow + j * 3;
                double gray = lum_b * p[0] + lum_g * p[1] + lum_r * p[2];
                drow[j] = cvl_sat_u8_f64(gray);
            }
            break;
        }
        case CVL_COLOR_GRAY2BGR:
        case CVL_COLOR_GRAY2RGB: {
            for (int j = 0; j < width; ++j) {
                uint8_t g = srow[j];
                uint8_t *p = drow + j * 3;
                p[0] = g;
                p[1] = g;
                p[2] = g;
            }
            break;
        }
        default:
            assert(false);
        }
    }
}

cvl_Mat cvl_cvt_color_new(const cvl_Mat *src, int code) {
    int dch = 0;
    switch (code) {
    case CVL_COLOR_RGB2GRAY:
    case CVL_COLOR_BGR2GRAY:
        assert(src->channels == 3);
        dch = 1;
        break;

    case CVL_COLOR_GRAY2RGB:
    case CVL_COLOR_GRAY2BGR:
        assert(src->channels == 1);
        dch = 3;
        break;
    }

    cvl_Mat dst = cvl_mat_create(src->height, src->width, dch, CVL_UINT8);
    assert(dst.data);

    cvl_cvt_color(src, &dst, code);

    return dst;
}

/**
 * Converts a matrix to another type with scaling.
 *
 * Effectively: `dst(x, y) = saturate_cast<ddepth>(α * src(x, y) + β)`
 *
 * @param src Input matrix.
 * @param dst Output matrix.
 * @param ddepth Type to convert to.
 * @param alpha Scale factor (1.0 for no change).
 * @param beta Value added to scaled values (0 for no change).
 */
void cvl_cvt_depth(const cvl_Mat *src, cvl_Mat *dst, cvl_depth_t ddepth, double alpha, double beta) {
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    assert(dst->depth == ddepth);

    const int h = src->height;
    const int w = src->width;
    const int ch = src->channels;
    const cvl_depth_t sdepth = src->depth;

    for (int i = 0; i < h; ++i) {

        switch (sdepth) {
        case CVL_UINT8: {
            uint8_t *srow = cvl_row_u8(src, i);

            switch (ddepth) {
            case CVL_UINT8: {
                uint8_t *drow = cvl_row_u8(dst, i);
                for (int j = 0; j < w * ch; ++j) {
                    double v = (double)srow[j];
                    v = alpha * v + beta;
                    drow[j] = cvl_sat_u8_f64(v);
                }
                break;
            }

            case CVL_FLOAT64: {
                double *drow = cvl_row_f64(dst, i);
                for (int j = 0; j < w * ch; ++j) {
                    double v = (double)srow[j];
                    v = alpha * v + beta;
                    drow[j] = v;
                }
                break;
            }

            default:
                assert(false);
            }
            break;
        }

        case CVL_FLOAT64: {
            double *srow = cvl_row_f64(src, i);

            switch (ddepth) {
            case CVL_UINT8: {
                uint8_t *drow = cvl_row_u8(dst, i);
                for (int j = 0; j < w * ch; ++j) {
                    double v = srow[j];
                    v = alpha * v + beta;
                    drow[j] = cvl_sat_u8_f64(v);
                }
                break;
            }

            case CVL_FLOAT64: {
                double *drow = cvl_row_f64(dst, i);
                for (int j = 0; j < w * ch; ++j) {
                    double v = srow[j];
                    v = alpha * v + beta;
                    drow[j] = v;
                }
                break;
            }

            default:
                assert(false);
            }
            break;
        }

        default:
            assert(false);
        }
    }
}

cvl_Mat cvl_cvt_depth_new(const cvl_Mat *src, cvl_depth_t ddepth, double alpha, double beta) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, ddepth);
    cvl_cvt_depth(src, &dst, ddepth, alpha, beta);
    return dst;
}

void cvl_convert_to_u8(cvl_Mat *src, cvl_Mat *dst) {
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    assert(src->depth == CVL_FLOAT64);
    assert(dst->depth == CVL_UINT8);

    const int h = src->height;
    const int w = src->width;
    const int chs = src->channels;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int c = 0; c < chs; ++c) {
                double val = CVL_AT_F64(src, i, j, c);
                CVL_AT_U8(dst, i, j, c) = cvl_sat_u8_f64(val);
            }
        }
    }
}

void cvl_convert_to_f64(cvl_Mat *src, cvl_Mat *dst) {
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    assert(src->depth == CVL_UINT8);
    assert(dst->depth == CVL_FLOAT64);

    int h = src->height;
    int w = src->width;
    int chs = src->channels;

    for (int r = 0; r < h; ++r) {
        const uint8_t *srow = (uint8_t *)src->data + r * src->stride;
        double *drow = (double *)((uint8_t *)dst->data + r * dst->stride);

        for (int c = 0; c < w; ++c) {
            for (int ch = 0; ch < chs; ++ch) {
                drow[c * chs + ch] = (double)srow[c * chs + ch];
            }
        }
    }
}

void cvl_normalize(cvl_Mat *src, cvl_Mat *dst) {
    cvl_cvt_depth(src, dst, dst->depth, 1.0 / 255.0, 0);
}

// ==========================
// Morphology & Transforms
// ==========================

// Applies a fixed-level threshold to each array element — determined by type.
void cvl_threshold(cvl_Mat *img, int thresh, int maxval, int type) {
    assert(img && img->data);
    assert(0 < img->channels && img->channels < 4);

    const size_t height = img->height;
    const size_t width = img->width;
    const size_t channels = img->channels;
    const size_t stride = img->stride;

    switch (img->depth) {
    case CVL_UINT8: {
        uint8_t *data = (uint8_t *)img->data;
        uint8_t t = (uint8_t)thresh;
        uint8_t mv = (uint8_t)maxval;
        for (size_t i = 0; i < height; ++i) {
            uint8_t *row = data + i * stride;
            for (size_t j = 0; j < width * channels; ++j) {
                uint8_t v = row[j];
                switch (type) {
                case CVL_THRESH_BINARY:
                    row[j] = (v > t) ? mv : 0;
                    break;
                case CVL_THRESH_BINARY_INV:
                    row[j] = (v > t) ? 0 : mv;
                    break;
                case CVL_THRESH_TRUNC:
                    row[j] = (v > t) ? t : v;
                    break;
                case CVL_THRESH_TOZERO:
                    row[j] = (v > t) ? v : 0;
                    break;
                case CVL_THRESH_TOZERO_INV:
                    row[j] = (v > t) ? 0 : v;
                    break;
                default:
                    assert(false); // CVL_THRESH_UNKNOWN
                }
            }
        }
        break;
    }
    case CVL_FLOAT32: {
        float *data = (float *)img->data;
        float t = (float)thresh;
        float mv = (float)maxval;
        for (size_t i = 0; i < height; ++i) {
            float *row = (float *)((uint8_t *)data + i * stride);
            for (size_t j = 0; j < width * channels; ++j) {
                float v = row[j];
                switch (type) {
                case CVL_THRESH_BINARY:
                    row[j] = (v > t) ? mv : 0.0f;
                    break;
                case CVL_THRESH_BINARY_INV:
                    row[j] = (v > t) ? 0.0f : mv;
                    break;
                case CVL_THRESH_TRUNC:
                    row[j] = (v > t) ? t : v;
                    break;
                case CVL_THRESH_TOZERO:
                    row[j] = (v > t) ? v : 0.0f;
                    break;
                case CVL_THRESH_TOZERO_INV:
                    row[j] = (v > t) ? 0.0f : v;
                    break;
                default:
                    assert(false); // CVL_THRESH_UNKNOWN
                }
            }
        }
        break;
    }
    case CVL_FLOAT64: {
        double *data = (double *)img->data;
        double t = (double)thresh;
        double mv = (double)maxval;
        for (size_t i = 0; i < height; ++i) {
            double *row = (double *)((uint8_t *)data + i * stride);
            for (size_t j = 0; j < width * channels; ++j) {
                double v = row[j];
                switch (type) {
                case CVL_THRESH_BINARY:
                    row[j] = (v > t) ? mv : 0.0;
                    break;
                case CVL_THRESH_BINARY_INV:
                    row[j] = (v > t) ? 0.0 : mv;
                    break;
                case CVL_THRESH_TRUNC:
                    row[j] = (v > t) ? t : v;
                    break;
                case CVL_THRESH_TOZERO:
                    row[j] = (v > t) ? v : 0.0;
                    break;
                case CVL_THRESH_TOZERO_INV:
                    row[j] = (v > t) ? 0.0 : v;
                    break;
                default:
                    assert(false); // CVL_THRESH_UNKNOWN
                }
            }
        }
        break;
    }
    }
}

cvl_Mat cvl_threshold_new(const cvl_Mat *src, int thresh, int maxval, int type) {
    cvl_Mat dst = cvl_mat_copy(src);
    cvl_threshold(&dst, thresh, maxval, type);
    return dst;
}

// Randomly flips binary pixels with probability p.
void cvl_add_noise(cvl_Mat *img, double p) {
    assert(img && img->data);
    assert(img->channels == 1);
    assert(img->depth == CVL_UINT8);
    assert(0.0 <= p && p <= 1.0);

    uint8_t *data = img->data;

    for (int i = 0; i < img->height; ++i) {
        uint8_t *row = data + i * img->stride;

        for (int j = 0; j < img->width; ++j) {
            uint8_t pixel = row[j];
            assert(pixel == BLACK || pixel == WHITE);

            double r = (double)rand() / RAND_MAX;
            if (r < p) {
                row[j] = (pixel == BLACK) ? WHITE : BLACK;
            }
        }
    }
}

// Rotates the image 180º.
void cvl_rotate(cvl_Mat *img) {
    const int h = img->height;
    const int w = img->width;
    const int chs = img->channels;

    const size_t elem_size = cvl_elem_size(img->depth);
    const size_t pixel_size = chs * elem_size;

    uint8_t *data = (uint8_t *)img->data;

    uint8_t *tmp[64];
    assert(pixel_size <= sizeof(tmp));

    for (int i = 0; i < h / 2; ++i) {
        for (int j = 0; j < w; ++j) {
            int r = h - i - 1;
            int c = w - j - 1;

            uint8_t *p1 = data + i * img->stride + j * pixel_size;
            uint8_t *p2 = data + r * img->stride + c * pixel_size;

            // swap pixel
            memcpy(tmp, p1, pixel_size);
            memcpy(p1, p2, pixel_size);
            memcpy(p2, tmp, pixel_size);
        }
    }

    if (h % 2 == 1) {
        int i = h / 2;
        for (int j = 0; j < w / 2; ++j) {
            int c = w - j - 1;

            uint8_t *p1 = data + i * img->stride + j * pixel_size;
            uint8_t *p2 = data + i * img->stride + c * pixel_size;

            // swap pixel
            memcpy(tmp, p1, pixel_size);
            memcpy(p1, p2, pixel_size);
            memcpy(p2, tmp, pixel_size);
        }
    }
}

// Inverts RGB channels according to the given max value.
void cvl_invert(cvl_Mat *img, int maxval) {
    const int h = img->height;
    const int w = img->width;
    const int chs = img->channels;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int ch = 0; ch < chs; ++ch) {
                if (img->depth == CVL_UINT8) {
                    uint8_t v = CVL_AT_U8(img, i, j, ch);
                    double out = maxval - v;
                    CVL_AT_U8(img, i, j, ch) = cvl_sat_u8_f64(out);
                } else if (img->depth == CVL_FLOAT64) {
                    double v = CVL_AT_F64(img, i, j, ch);
                    CVL_AT_F64(img, i, j, ch) = maxval - v;
                } else {
                    assert(false);
                }
            }
        }
    }
}

// Changes all pixels with black neighbors to black.
void cvl_expand(cvl_Mat *img) { _morph(img, BLACK); }

// Changes all pixels with white neighbors to white.
void cvl_shrink(cvl_Mat *img) { _morph(img, WHITE); }

// ==========================
// Connected Component Labeling
// ==========================

// Performs Connected Component Labeling. Returns number of components found.
int cvl_connected_components(const cvl_Mat *src, cvl_Mat *labels, int connectivity) {
    assert(src && src->data);
    assert(labels && labels->data);
    assert(src->height == labels->height && src->width == labels->width);
    assert(src->channels == 1 && labels->channels == 1);
    assert(src->depth == CVL_UINT8 && labels->depth == CVL_32S);
    assert(connectivity == 4 || connectivity == 8);

    const int h = src->height;
    const int w = src->width;

    size_t max_label = (size_t)h * w + 1;
    assert(max_label < INT32_MAX);

    UFNode *uf = malloc(max_label * sizeof(UFNode));
    assert(uf);

    int dh[] = {-1, 0, -1, -1}; // |- 0 -| or |0 0 0|
    int dw[] = { 0, -1, -1, 1}; // |0 - -|    |0 - -|
    const int n_neighbors = (connectivity == 4) ? 2 : 4;

    int32_t next_label = 1;

    // Pass I - Assign Labels and Equivalences.
    for (int i = 0; i < h; ++i) {
        uint8_t *srow = cvl_row_u8(src, i);
        int32_t *lrow = cvl_row_i32(labels, i);

        for (int j = 0; j < w; ++j) {
            if (srow[j] == WHITE) continue;

            int count = 0;
            int neighbor_labels[4];
            for (int k = 0; k < n_neighbors; ++k) {
                int ni = i + dh[k];
                int nj = j + dw[k];

                bool in_bounds = (0 <= ni && ni < h) && (0 <= nj && nj < w);
                if (!in_bounds) continue;

                int32_t *nrow = cvl_row_i32(labels, ni);
                if (nrow[nj] > 0) {
                    neighbor_labels[count++] = nrow[nj];
                }
            }

            if (count == 0) { // assign new label
                lrow[j] = next_label;
                uf[next_label].parent = next_label;
                next_label++;
            } else { // assign exsisting label and mark equivalence
                int32_t min_label = neighbor_labels[0];
                for (int k = 0; k < count; ++k) {
                    if (neighbor_labels[k] < min_label) {
                        min_label = neighbor_labels[k];
                    }
                }
                lrow[j] = min_label;

                for (int k = 0; k < count; ++k) { // assign equivalence
                    uf_union(uf, min_label, neighbor_labels[k]);
                }
            }
        }
    }

    // Pass II - Reconcile Equivalences.
    for (int i = 0; i < h; ++i) {
        int32_t *lrow = cvl_row_i32(labels, i);

        for (int j = 0; j < w; ++j) {
            if (lrow[j] > 0) {
                lrow[j] = uf_find(uf, lrow[j]);
            }
        }
    }

    // Pass III - Count Unique Labels.
    bool *seen = calloc(next_label, sizeof(bool));
    assert(seen);
    int num_components = 0;
    for (int i = 0; i < h; ++i) {
        int32_t *lrow = cvl_row_i32(labels, i);

        for (int j = 0; j < w; ++j) {
            if (lrow[j] > 0 && !seen[lrow[j]]) {
                seen[lrow[j]] = true;
                num_components++;
            }
        }
    }

    free(seen);
    free(uf);

    return num_components;
}

// Colors components larger than thresh. Returns the number of such components.
int cvl_color_components(cvl_Mat *img, const cvl_Mat *labels, int thresh) {
    assert(img && img->data);
    assert(labels && labels->data);
    assert(img->height == labels->height && img->width == labels->width);
    assert(img->channels == 3);
    assert(labels->channels == 1);
    assert(img->depth == CVL_UINT8 && labels->depth == CVL_32S);
    assert(thresh >= 0);

    const int h = img->height;
    const int w = img->width;
    const int ch = img->channels;

    size_t max_label = (size_t)h * w + 1;
    assert(max_label < INT32_MAX);

    size_t *sizes = calloc(max_label, sizeof(size_t));
    assert(sizes);

    // Pass I - Calculate Component Sizes.
    for (int i = 0; i < h; ++i) {
        int32_t *row = cvl_row_i32(labels, i);
        for (int j = 0; j < w; ++j) {
            if (row[j] > 0) {
                sizes[row[j]]++;
            }
        }
    }

    // Count Components.
    int count = 0;
    for (size_t i = 0; i < max_label; ++i) {
        if (sizes[i] >= (size_t)thresh) {
            count++;
        }
    }

    // Pass II - Color Components.
    const int min = 50; // hopefully far enough from black?
    const int max = 256 - min;

    for (int i = 0; i < h; ++i) {
        uint8_t *row = cvl_row_u8(img, i);
        int32_t *lrow = cvl_row_i32(labels, i);
        for (int j = 0; j < w; ++j) {
            int label = lrow[j];
            if (label > 0 && sizes[label] >= (size_t)thresh) {
                row[j * ch + 0] = min + (label * 37) % max;  // r
                row[j * ch + 1] = min + (label * 73) % max;  // g
                row[j * ch + 2] = min + (label * 109) % max; // b
            }
        }
    }

    free(sizes);

    return count;
}

// ==========================
// Correlation & Convolution
// ==========================

// Floating point correlation of src with kernel (using zero padding).
static void _correlate_f(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel) {
    // G(r, c) = \sum_{i=-m}^{m} \sum_{j=-n}^{n} K(i, j) * I(r + i), c + j)
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    assert(src->depth == CVL_FLOAT64);
    assert(dst->depth == CVL_FLOAT64);
    assert(kernel->depth == CVL_FLOAT64);

    const int h = src->height;
    const int w = src->width;
    const int chs = src->channels;

    const int kh = kernel->height;
    const int kw = kernel->width;

    const int ar = kh / 2.0; // anchor (row)
    const int ac = kw / 2.0; // anchor (column)

    double *kdata = kernel->data;

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            for (int ch = 0; ch < chs; ++ch) {
                double ws = 0.0; // weighted sum

                for (int i = 0; i < kh; ++i) {
                    for (int j = 0; j < kw; ++j) {
                        int rr = r + i - ar;
                        int cc = c + j - ac;
                        bool in_bounds = (0 <= rr && rr < h) && (0 <= cc && cc < w);
                        if (in_bounds) { // BORDER_CONSTANT
                            double kval = kdata[i * kw + j];
                            double sval = CVL_AT_F64(src, rr, cc, ch);
                            ws += kval * sval;
                        }
                    }
                }

                CVL_AT_F64(dst, r, c, ch) = ws;
            }
        }
    }
}

// Correlate src with kernel using zero padding.
void cvl_correlate(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel) {
    cvl_Mat src_f64 = cvl_mat_create(src->height, src->width, src->channels, CVL_FLOAT64);
    cvl_Mat dst_f64 = cvl_mat_create(src->height, src->width, src->channels, CVL_FLOAT64);

    cvl_cvt_depth(src, &src_f64, CVL_FLOAT64, 1.0, 0.0); // src --> f64

    _correlate_f(&src_f64, &dst_f64, kernel);

    cvl_cvt_depth(&dst_f64, dst, dst->depth, 1.0, 0.0); // f64 --> dst

    cvl_mat_free(&src_f64);
    cvl_mat_free(&dst_f64);
}

cvl_Mat cvl_correlate_new(const cvl_Mat *src, cvl_Mat *kernel) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_correlate(src, &dst, kernel);
    return dst;
}

// Convolve src with kernel using zero padding.
void cvl_convolve(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel) {
    const int h = kernel->height;
    const int w = kernel->width;
    const int chs = kernel->channels;
    const int depth = kernel->depth;

    // Create Flipped Kernel (rotated 180 degrees).
    cvl_Mat flipped = cvl_mat_create(h, w, chs, depth);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int ch = 0; ch < chs; ++ch) {
                double v = CVL_AT_F64(kernel, h - 1 - i, w - 1 - j, ch);
                CVL_AT_F64(&flipped, i, j, ch) = v;
            }
        }
    }

    cvl_correlate(src, dst, &flipped);

    cvl_mat_free(&flipped);
}

cvl_Mat cvl_convolve_new(const cvl_Mat *src, cvl_Mat *kernel) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_convolve(src, &dst, kernel);
    return dst;
}

// Applies 1D horizontal convolution.
static void _convolve_x(const cvl_Mat *src, cvl_Mat *dst, const cvl_Mat *kernel) {
    assert(src->depth == CVL_FLOAT64 && dst->depth == CVL_FLOAT64);

    const int h = src->height;
    const int w = src->width;
    const int chs = src->channels;

    const int radius = kernel->width / 2;
    double *kdata = kernel->data;

    for (int y = 0; y < h; ++y) {
        double *srow = cvl_row_f64(src, y);
        double *drow = cvl_row_f64(dst, y);

        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < chs; ++ch) {
                double sum = 0.0;

                for (int k = -radius; k <= radius; ++k) {
                    int xx = x + k;
                    xx = _clamp(xx, 0, w - 1); // BOARDER_REPLICATE
                    // if (xx < 0|| xx >= w) continue; // BOARDER_CONSTANT
                    sum += kdata[k + radius] * srow[xx * chs + ch];
                }

                drow[x * chs + ch] = sum;
            }
        }
    }
}

// Applies 1D vertical convolution.
static void _convolve_y(const cvl_Mat *src, cvl_Mat *dst, const cvl_Mat *kernel) {
    assert(src->depth == CVL_FLOAT64 && dst->depth == CVL_FLOAT64);

    const int w = src->width;
    const int h = src->height;
    const int chs = src->channels;

    const int radius = kernel->width / 2;
    double *kdata = kernel->data;

    for (int y = 0; y < h; ++y) {
        double *drow = cvl_row_f64(dst, y);

        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < chs; ++ch) {
                double sum = 0.0;

                for (int k = -radius; k < radius; ++k) {
                    int yy = y + k;
                    yy = _clamp(yy, 0, h - 1); // BOARDER_REPLICATE
                    // if (yy < 0 || yy >= h) continue; // BOARDER_CONSTANT

                    double *srow = cvl_row_f64(src, yy);
                    sum += kdata[k + radius] * srow[x * chs + ch];
                }

                drow[x * chs + ch] = sum;
            }
        }
    }
}

/**
 * Applies a separable linear filter to an image.
 * 
 * First, every row of src is convolved with the horizontal kernel,
 * then every column of the result is convolved with the vertical kernel.
 * 
 * @param src Input matrix.
 * @param dst Output matrix.
 * @param kx Horizontal kernel.
 * @param ky Vertical kernel.
 */
void cvl_convolve_sep(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kx, cvl_Mat *ky) {
    // todo: impl. _is_separable(kernel)

    cvl_Mat tmp = cvl_mat_create(src->height, src->width, src->channels, CVL_FLOAT64);

    _convolve_x(src, &tmp, kx);
    _convolve_y(&tmp, dst, ky);

    cvl_mat_free(&tmp);
}

// ==========================
// Blurring / Smoothing
// ==========================

// Apply mean blur using a uniform kernel with optional normalization.
void cvl_blur_box(const cvl_Mat *src, cvl_Mat *dst, int ksize, bool normalize) {
    assert(src && src->data);
    assert(dst && dst->data);
    assert(src->height == dst->height && src->width == dst->width);

    const double norm = normalize ? 1.0 / (ksize * ksize) : 1.0;

    cvl_Mat kernel = cvl_mat_create_fill(ksize, ksize, 1, CVL_FLOAT64, &norm);

    cvl_convolve(src, dst, &kernel);

    cvl_mat_free(&kernel);
}

cvl_Mat cvl_blur_box_new(const cvl_Mat *src, int ksize, bool normalize) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_blur_box(src, &dst, ksize, normalize);
    return dst;
}

// Apply mean blur using a normalized uniform kernel.
void cvl_blur_mean(const cvl_Mat *src, cvl_Mat *dst, int ksize) {
    cvl_blur_box(src, dst, ksize, true);
}

cvl_Mat cvl_blur_mean_new(const cvl_Mat *src, int ksize) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_blur_mean(src, &dst, ksize);
    return dst;
}

// Returns a normalized 1D Gaussian kernel.
static cvl_Mat _gauss_kernel(int ksize, double sigma) {
    if (ksize < 1 || ksize % 2 == 0) {
        ksize = 5;
    }

    if (sigma <= 0.0) { // compute from ksize
        sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8; // from opencv
    }

    cvl_Mat kernel = cvl_mat_create(1, ksize, 1, CVL_FLOAT64);
    double *k = kernel.data;

    const int radius = ksize / 2;
    double sum = 0.0;

    for (int x = -radius; x < radius; ++x) {
        double gx = exp(-(x * x) / (2.0 * sigma * sigma));
        k[x + radius] = gx;
        sum += gx;
    }

    for (int x = 0; x < ksize; ++x) {
        k[x] /= sum;
    }

    return kernel;
}

/**
 * Applies Gaussian blur using a kernel defined by ksize and/or sigma.
 * 
 * @param src Input matrix.
 * @param dst Output matrix (blurred).
 * @param ksize Kernel size (if 0, derived from sigma).
 * @param sigma Standard deviation of the Gaussian kernel (if 0, derived from ksize).
 */
void cvl_blur_gauss(const cvl_Mat *src, cvl_Mat *dst, int ksize, double sigma) {
    assert(ksize > 0.0 || sigma > 0.0);

    if (ksize > 0.0 && sigma <= 0.0) { // compute sigma from ksize
        sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8; // from opencv
    }
    if (sigma > 0.0 && ksize <= 0.0) { // compute ksize from sigma
        ksize = 2 * ceil(3.0 * sigma) + 1;
    }


    // Convert to f64.
    cvl_Mat src_f64 = cvl_mat_create(src->height, src->width, src->channels, CVL_FLOAT64);
    cvl_Mat dst_f64 = cvl_mat_create(src->height, src->width, src->channels, CVL_FLOAT64);

    cvl_cvt_depth(src, &src_f64, CVL_FLOAT64, 1.0, 0.0);

    cvl_Mat kernel = _gauss_kernel(ksize, sigma);
    cvl_convolve_sep(&src_f64, &dst_f64, &kernel, &kernel);

    cvl_cvt_depth(&dst_f64, dst, dst->depth, 1.0, 0.0);

    cvl_mat_free(&kernel);
    cvl_mat_free(&dst_f64);
    cvl_mat_free(&src_f64);
}

/**
 * Applies Gaussian blur using a kernel defined by ksize and/or sigma.
 * 
 * @param src Input matrix.
 * @param ksize Kernel size (if 0, derived from sigma).
 * @param sigma Standard deviation of the Gaussian kernel (if 0, derived from ksize).
 * 
 * @returns Blurred matrix.
 */
cvl_Mat cvl_blur_gauss_new(const cvl_Mat *src, int ksize, double sigma) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_blur_gauss(src, &dst, ksize, sigma);
    return dst;
}

// Apply median blur using replicated outlier pixel values.
void cvl_blur_median(const cvl_Mat *src, cvl_Mat *dst, int ksize) {
    assert(src->height == dst->height && src->width == dst->width);
    assert(ksize % 2 == 1);

    const int h = src->height;
    const int w = src->width;
    const int chs = src->channels;

    const int ar = ksize / 2; // anchor (row)
    const int ac = ksize / 2; // anchor (column)

    const int kxk = ksize * ksize;

    double *kernel = malloc(sizeof(double) * kxk);
    assert(kernel);

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            for (int ch = 0; ch < chs; ++ch) {
                int idx = 0;

                for (int i = 0; i < ksize; ++i) {
                    for (int j = 0; j < ksize; ++j) {
                        int rr = _clamp(r + i - ar, 0, h - 1);
                        int cc = _clamp(c + j - ac, 0, w - 1);

                        // BORDER_REPLICATE
                        kernel[idx++] = CVL_AT_F64(src, rr, cc, ch);
                    }
                }

                CVL_AT_F64(dst, r, c, ch) = _median(kernel, kxk);
            }
        }
    }

    free(kernel);
}

cvl_Mat cvl_blur_median_new(const cvl_Mat *src, int ksize) {
    cvl_Mat dst = cvl_mat_create(src->height, src->width, src->channels, src->depth);
    cvl_blur_median(src, &dst, ksize);
    return dst;
}

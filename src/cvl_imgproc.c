#include "cvl_imgproc.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// Internal Helpers
// ==========================
typedef struct {
    int parent;
} UFNode;

// Find root via path compression.
static int uf_find(UFNode *uf, int x) {
    if (uf[x].parent != x) {
        uf[x].parent = uf_find(uf, uf[x].parent);
    }

    return uf[x].parent;
}

// Union two (equivalent) sets by roots.
static void uf_union(UFNode *uf, int a, int b) {
    int ra = uf_find(uf, a);
    int rb = uf_find(uf, b);
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

static inline uint8_t saturate_u8(int v) {
    return (v < 0) ? 0 : (v > 255) ? 255 : (uint8_t)v;
}

static inline uint8_t cvl_sat_u8(double v) {
    return (v < 0.0) ? 0 : (v > 255.0) ? 255 : (uint8_t)(v + 0.5);
}

// ==========================
// Color & Depth Conversion
// ==========================

void cvl_cvtcolor(cvl_Mat *src, cvl_Mat *dst, int code) {
    assert(src && src->data && dst);

    const int height = src->height;
    const int width = src->width;
    const int schannels = src->channels;
    const uint8_t *sdata = src->data;

    // Determine Destination Channels.
    int dchannels = 0;
    switch (code) {
        case CVL_COLOR_RGB2GRAY:
        case CVL_COLOR_BGR2GRAY:
            assert(schannels == 3);
            dchannels = 1;
            break;
        case CVL_COLOR_GRAY2RGB:
        case CVL_COLOR_GRAY2BGR:
            assert(schannels == 1);
            dchannels = 3;
            break;
        default: assert(false);
    }

    *dst = cvl_mat_create(height, width, dchannels, CVL_UINT8);
    uint8_t *ddata = dst->data;

    // ITU-R BT.601-7
    const double lum_r = 0.299;
    const double lum_g = 0.587;
    const double lum_b = 0.114;;

    for (int i = 0; i < height; ++i) {
        const uint8_t *srow = sdata + i * src->stride;
        uint8_t *drow = ddata  + i * dst->stride;

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
                    int gray = (int)lum_r*p[0] + lum_g*p[1] + lum_b*p[2];
                    drow[j] = saturate_u8(gray);
                }
                break;
            }
            case CVL_COLOR_BGR2GRAY: {
                for (int j = 0; j < width; ++j) {
                    const uint8_t *p = srow + j * 3;
                    int gray = (int)lum_b*p[0] + lum_g*p[1] + lum_r*p[2];
                    drow[j] = saturate_u8(gray);
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
            default: assert(false);
        }
    }
}

void cvl_convert_to(cvl_Mat *src, cvl_Mat *dst, double alpha, double beta) {
    // void cvl_cvtdepth(cvl_Mat *src, cvl_Mat *dst, int ddepth)
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    
    for (int i = 0; i < src->height; ++i) {
        uint8_t *srow_u8 =  cvl_row_u8(src, i);
        double *srow_f64 = cvl_row_f64(src, i);
        
        uint8_t *drow_u8 = cvl_row_u8(dst, i);
        double *drow_f64 = cvl_row_f64(dst, i);
        
        for (int j = 0; j < src->width; ++j) {
            double v;
            
            switch (src->depth) {
                case CVL_UINT8:   v = (double)srow_u8[j]; break;
                case CVL_FLOAT64: v = srow_f64[j]; break;
                default: assert(false);
            }
            
            v = alpha * v + beta;
            
            switch (dst->depth) {
                case CVL_UINT8:   drow_u8[j] = cvl_sat_u8(v); break;
                case CVL_FLOAT64: drow_f64[j] = v; break;
                default: assert(false);
            }
        }
    }
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

                // clamp
                if (val < 0.0) val = 0.0;
                if (val > 255.0) val = 255.0;

                // round + cast
                CVL_AT_U8(dst, i, j, c) = (uint8_t)(val + 0.5);
            }
        }
    }
}

void cvl_convert_to_f64(cvl_Mat *src, cvl_Mat *dst) {
    assert(src->height == dst->height);
    assert(src->width  == dst->width);
    assert(src->channels == dst->channels);
    assert(src->depth == CVL_UINT8);
    assert(dst->depth == CVL_FLOAT64);

    int h = src->height;
    int w = src->width;
    int chs = src->channels;

    for (int r = 0; r < h; ++r) {
        const uint8_t *srow = (uint8_t*)src->data + r * src->stride;
        double *drow = (double*)((uint8_t*)dst->data + r * dst->stride);

        for (int c = 0; c < w; ++c) {
            for (int ch = 0; ch < chs; ++ch) {
                drow[c * chs + ch] = (double)srow[c * chs + ch];
            }
        }
    }
}

void cvl_normalize(cvl_Mat *src, cvl_Mat *dst) {
    cvl_convert_to(src, dst, 1.0 / 255.0, 0);
}

// ==========================
// Morphology & Transforms
// ==========================

// Applies a fixed-level threshold to each array element — determined by type.
void cvl_threshold(cvl_Mat *img, int thresh, int maxval, int type) {
    assert(img && img->data);
    assert(0 < img->channels && img->channels < 4);

    const size_t height = img->height;
    const size_t width  = img->width;
    const size_t channels = img->channels;
    const size_t stride = img->stride;

    switch (img->depth) {
        case CVL_UINT8: {
            uint8_t *data = (uint8_t*)img->data;
            uint8_t t     = (uint8_t)thresh;
            uint8_t mv    = (uint8_t)maxval;
            for (size_t i = 0; i < height; ++i) {
                uint8_t *row = data + i * stride;
                for (size_t j = 0; j < width * channels; ++j) {
                    uint8_t v = row[j];
                    switch (type) {
                        case CVL_THRESH_BINARY:     row[j] = (v > t) ? mv : 0; break;
                        case CVL_THRESH_BINARY_INV: row[j] = (v > t) ? 0 : mv; break;
                        case CVL_THRESH_TRUNC:      row[j] = (v > t) ? t : v;  break;
                        case CVL_THRESH_TOZERO:     row[j] = (v > t) ? v : 0;  break;
                        case CVL_THRESH_TOZERO_INV: row[j] = (v > t) ? 0 : v;  break;
                        default: assert(false);  // CVL_THRESH_UNKNOWN
                    }
                }
            }
            break;
        }
        case CVL_FLOAT32: {
            float *data = (float*)img->data;
            float t     = (float)thresh;
            float mv    = (float)maxval;
            for (size_t i = 0; i < height; ++i) {
                float *row = (float *)((uint8_t *)data + i * stride);
                for (size_t j = 0; j < width * channels; ++j) {
                    float v = row[j];
                    switch (type) {
                        case CVL_THRESH_BINARY:     row[j] = (v > t) ? mv : 0.0f; break;
                        case CVL_THRESH_BINARY_INV: row[j] = (v > t) ? 0.0f : mv; break;
                        case CVL_THRESH_TRUNC:      row[j] = (v > t) ? t : v;     break;
                        case CVL_THRESH_TOZERO:     row[j] = (v > t) ? v : 0.0f;  break;
                        case CVL_THRESH_TOZERO_INV: row[j] = (v > t) ? 0.0f : v;  break;
                        default: assert(false);  // CVL_THRESH_UNKNOWN
                    }
                }
            }
            break;
        }
        case CVL_FLOAT64: {
            double *data = (double*)img->data;
            double t     = (double)thresh;
            double mv    = (double)maxval;
            for (size_t i = 0; i < height; ++i) {
                double *row = (double *)((uint8_t *)data + i * stride);
                for (size_t j = 0; j < width * channels; ++j) {
                    double v = row[j];
                    switch (type) {
                        case CVL_THRESH_BINARY:     row[j] = (v > t) ? mv : 0.0; break;
                        case CVL_THRESH_BINARY_INV: row[j] = (v > t) ? 0.0 : mv; break;
                        case CVL_THRESH_TRUNC:      row[j] = (v > t) ? t : v;    break;
                        case CVL_THRESH_TOZERO:     row[j] = (v > t) ? v : 0.0;  break;
                        case CVL_THRESH_TOZERO_INV: row[j] = (v > t) ? 0.0 : v;  break;
                        default: assert(false);  // CVL_THRESH_UNKNOWN
                    }
                }
            }
            break;
        }
    }
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
                    CVL_AT_U8(img, i, j, ch) = cvl_sat_u8(out);
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
int cvl_connected_components(cvl_Mat *img, cvl_Mat *labels, int connectivity) {
    assert(img && img->data);
    assert(labels && labels->data);
    assert(img->height == labels->height && img->width == labels->width);
    assert(img->channels == 1 && labels->channels == 1);
    assert(img->depth == CVL_UINT8 && labels->depth == CVL_INT32);
    assert(connectivity == 4 || connectivity == 8);

    const int h = img->height;
    const int w = img->width;

    uint8_t *idata = img->data;
    int *ldata = (int *)labels->data;

    size_t istride = img->stride;
    size_t lstride = labels->stride;

    UFNode *uf = malloc((h * w + 1) * sizeof(UFNode));
    assert(uf);

    int dh[] = {-1, 0, -1, -1}; // |- 0 -| or |0 0 0|
    int dw[] = {0, -1, -1, 1};  // |0 - -|    |0 - -|
    const int n_neighbors = (connectivity == 4) ? 2 : 4;

    int next_label = 1;

    // Pass I - Assign Labels and Equivalences.
    for (int i = 0; i < h; ++i) {
        uint8_t *irow = idata + i * istride;
        int *lrow = (int *)((uint8_t *)ldata + i * lstride);
        
        for (int j = 0; j < w; ++j) {
            if (irow[j] == WHITE) continue;
            
            int count = 0;
            int neighbor_labels[4];
            for (int k = 0; k < n_neighbors; ++k) {
                int ni = i + dh[k];
                int nj = j + dw[k];
                
                bool in_bounds = (0 <= ni && ni < h) && (0 <= nj && nj < w);
                if (!in_bounds) continue;
                
                int *nrow = (int *)((uint8_t *)ldata + ni * lstride);
                if (nrow[nj] > 0) {
                    neighbor_labels[count++] = nrow[nj];
                }
            }
            
            if (count == 0) { // assign new label
                lrow[j] = next_label;
                uf[next_label].parent = next_label;
                next_label++;
            } else { // assign exsisting label and mark equivalence
                int min_label = neighbor_labels[0];
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
        int *lrow = (int *)((uint8_t *)ldata + i * lstride);
        
        for (int j = 0; j < w; ++j) {
            int label = lrow[j];
            if (label > 0) {
                lrow[j] = uf_find(uf, label);
            }
        }
    }
    
    // Pass III - Count Unique Labels.
    bool *seen = calloc(next_label, sizeof(bool));
    assert(seen);
    int num_components = 0;
    for (int i = 0; i < h; ++i) {
        int *lrow = (int *)((uint8_t *)ldata + i * lstride);
        
        for (int j = 0; j < w; ++j) {
            int label = lrow[j];
            if (label > 0 && !seen[label]) {
                seen[label] = true;
                num_components++;
            }
        }
    }
    
    free(seen);
    free(uf);

    return num_components;
}

// Colors components larger than thresh. Returns the number of such components.
int cvl_color_components(cvl_Mat *img, cvl_Mat *labels, int thresh) {
    assert(img && img->data);
    assert(labels && labels->data);
    assert(img->height == labels->height && img->width == labels->width);
    assert(img->channels == 3);
    assert(labels->channels == 1);
    assert(img->depth == CVL_UINT8 && labels->depth == CVL_INT32);

    const int h = img->height;
    const int w = img->width;
    const int ch = img->channels;

    uint8_t *idata = img->data;
    int *ldata = labels->data;

    size_t lstride = labels->stride;
    size_t istride = img->stride;

    int max_label = h * w + 1;

    int count = 0;
    int *sizes = calloc(max_label, sizeof(int));
    assert(sizes);

    // Pass I - Calculate Component Sizes.
    for (int i = 0; i < h; ++i) {
        int *row = ldata + i * lstride;
        for (int j = 0; j < w; ++j) {
            int label = row[j];
            if (label > 0) {
                sizes[label]++;
            }
        }
    }

    // Count Components.
    for (int i = 0; i < max_label; ++i) {
        if (sizes[i] >= thresh) {
            count++;
        }
    }

    // Pass II - Color Components.
    const int min = 50; // hopefully far enough from black?
    const int max = 256 - min;

    for (int i = 0; i < h; ++i) {
        uint8_t *row = idata + i * istride;
        int *lrow = (int *)((uint8_t *)ldata + i * lstride);
        for (int j = 0; j < w; ++j) {
            int label = lrow[j];
            if (label > 0 && sizes[label] >= thresh) {
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
// Filtering & Convolution
// ==========================

// Correlate src with kernel using zero padding.
void cvl_correlate(cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel) {
    // G(r, c) = \sum_{i=-m}^{m} \sum_{j=-n}^{n} K(i, j) * I(r + i), c + j)
    assert(src->height == dst->height);
    assert(src->width == dst->width);
    assert(src->channels == dst->channels);
    assert(src->depth == CVL_FLOAT64);
    assert(src->depth == dst->depth);
    assert(src->depth == kernel->depth);

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

// Convolve src with kernel using zero padding.
void cvl_convolve(cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel) {
    cvl_rotate(kernel);
    cvl_correlate(src, dst, kernel);
    cvl_rotate(kernel); // undo rotation
}

// Apply mean blur using normalized ksize * ksize uniformly kernel.
void cvl_blur(cvl_Mat *src, cvl_Mat *dst, int ksize) {
    assert(src && src->data);
    assert(dst && dst->data);
    assert(src->height == dst->height && src->width == dst->width);
    assert(dst->depth == CVL_FLOAT64);

    cvl_Mat kernel = cvl_mat_create(ksize, ksize, 1, CVL_FLOAT64);
    double *data = kernel.data;

    double v = 1.0 / (ksize * ksize);
    for (int i = 0; i < kernel.height * kernel.width; ++i) {
        data[i] = v;
    }

    cvl_convolve(src, dst, &kernel);

    cvl_mat_free(&kernel);
}

// Apply median blur using replicated outlier pixel values.
void cvl_median_blur(cvl_Mat *src, cvl_Mat *dst, int ksize) {
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

                        kernel[idx++] = CVL_AT_F64(src, rr, cc, ch); // BORDER_REPLICATE
                    }
                }

                CVL_AT_F64(dst, r, c, ch) = _median(kernel, kxk);
            }
        }
    }

    free(kernel);
}

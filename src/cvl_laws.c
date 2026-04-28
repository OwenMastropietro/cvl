
#include <assert.h>
#include <cvl/cvl_imgproc.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum cvl_laws_t {
    CVL_LAWS_L5, // level
    CVL_LAWS_E5, // edge
    CVL_LAWS_S5, // spot
    CVL_LAWS_W5, // wave
    CVL_LAWS_R5, // ripple
} cvl_laws_t;

static double *cvl_laws_mask_1d(cvl_laws_t t) {
    static const double L5[5] = { 1,  4,  6,  4,  1};
    static const double E5[5] = {-1, -2,  0,  2,  1};
    static const double S5[5] = {-1,  0,  2,  0, -1};
    static const double W5[5] = {-1,  2,  0, -2,  1};
    static const double R5[5] = { 1, -4,  6, -4,  1};

    static double L5_NORM[5], E5_NORM[5], S5_NORM[5], W5_NORM[5], R5_NORM[5];

    static bool initialized = false; // heck yeah
    if (!initialized) {
        for (int i = 0; i < 5; ++i) L5_NORM[i] = L5[i] / 16.0;
        for (int i = 0; i < 5; ++i) E5_NORM[i] = E5[i] / 6.0;
        for (int i = 0; i < 5; ++i) S5_NORM[i] = S5[i] / 4.0;
        for (int i = 0; i < 5; ++i) W5_NORM[i] = W5[i] / 6.0;
        for (int i = 0; i < 5; ++i) R5_NORM[i] = R5[i] / 16.0;
        initialized = true;
    }

    switch (t) {
        case CVL_LAWS_L5: return L5_NORM;
        case CVL_LAWS_E5: return E5_NORM;
        case CVL_LAWS_S5: return S5_NORM;
        case CVL_LAWS_W5: return W5_NORM;
        case CVL_LAWS_R5: return R5_NORM;
        default: assert(false);
    }
}

static Matrix cvl_laws_mask_2d(cvl_laws_t t1, cvl_laws_t t2) {
    double *a = cvl_laws_mask_1d(t1);
    double *b = cvl_laws_mask_1d(t2);

    Matrix k = cvl_mat_create(5, 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            k.map[i][j] = a[i] * b[j];
        }
    }
    return k;
}

// Compute 25 Laws texture energy maps (L/E/S/W/R combinations).
static Matrix *cvl_laws_energies(const Matrix *src, int ksize) {
    const int ntypes = 5;
    cvl_laws_t types[5] = {CVL_LAWS_L5, CVL_LAWS_E5, CVL_LAWS_S5, CVL_LAWS_W5, CVL_LAWS_R5};

    const size_t nmaps = ntypes * ntypes;
    Matrix *energies = malloc(nmaps * sizeof(Matrix));
    assert(energies);

    int i = 0;
    for (int t1 = 0; t1 < ntypes; ++t1) {
        for (int t2 = 0; t2 < ntypes; ++t2) {
            // todo: src = src - mean
            // Matrix mean = cvl_blur_mean_new(src, ksize);
            // Matrix norm = cvl_mat_sub_new(src, &mean);
            Matrix mask = cvl_laws_mask_2d(types[t1], types[t2]);
            Matrix resp = cvl_convolve_new(src, &mask);
            cvl_mat_abs(&resp);

            energies[i++] = cvl_blur_mean_new(&resp, ksize);

            cvl_mat_free(resp);
            cvl_mat_free(mask);
        }
    }

    return energies;
}

// --------------------------------------------------------------------

static void _kmeans_init(double *centroids, const double *data, int n, int d, int k) {
    for (int c = 0; c < k; ++c) {
        int r = rand() % n;
        for (int f = 0; f < d; ++f) {
            centroids[c * d + f] = data[r * d + f];
        }
    }
}

static double _kmeans_dist_sq(const double *a, const double *b, int d) {
    double sum = 0.0;
    for (int i = 0; i < d; ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

static int cvl_kmeans(const double *data, int n, int d, int k, int *labels, int max_iter) {
    if (k < 1 || n < k) return -1;

    double *prev_cs = malloc(k * d * sizeof(double)); // prev centroids
    double *next_cs = malloc(k * d * sizeof(double)); // next centroids
    size_t *sizes   = malloc(k * sizeof(size_t));     // centroid sizes
    assert(prev_cs && next_cs && sizes);

    _kmeans_init(prev_cs, data, n, d, k);

    int iter = 0;
    bool converged = false;
    while (iter < max_iter && !converged) {
        // Assign.
        for (int p = 0; p < n; ++p) {
            double min_dist = DBL_MAX;
            int best = 0;
            for (int c = 0; c < k; ++c) {
                double dist = _kmeans_dist_sq(&data[p * d], &prev_cs[c * d], d);
                if (dist < min_dist) {
                    min_dist = dist;
                    best = c;
                }
            }
            labels[p] = best;
        }

        // Update.
        memset(next_cs, 0, k * d * sizeof(double));
        memset(sizes, 0, k * sizeof(size_t));

        for (int p = 0; p < n; ++p) {
            int c = labels[p];
            sizes[c]++;
            for (int f = 0; f < d; ++f) {
                next_cs[c * d + f] += data[p * d + f];
            }
        }

        converged = true;
        for (int c = 0; c < k; ++c) {
            if (sizes[c] == 0) continue; // empty cluster, keep old centroid

            for (int f = 0; f < d; ++f) {
                double prev = prev_cs[c * d + f];
                double next = next_cs[c * d + f] / sizes[c];
                next_cs[c * d + f] = next;

                converged = (fabs(next - prev) <= 1e-5);
            }
        }
        memcpy(prev_cs, next_cs, k * d * sizeof(double));
        iter++;
    }

    free(prev_cs);
    free(next_cs);
    free(sizes);
    return iter;
}

// --------------------------------------------------------------------
// Full texture segmentation pipeline
Image cvl_texture_segment_laws_kmeans(Image *src, int kclusters, int energy_window) {
    if (!src || src->height == 0 || src->width == 0 || kclusters < 2) {
        return cvl_img_create(0, 0); // invalid
    }
    if (energy_window % 2 == 0) {
        energy_window = 15;
    }

    int h = src->height;
    int w = src->width;
    int npixels = h * w;

    // 1 - Convert to Grayscale.
    Matrix gray = cvl_img2mat(*src);

    // 2 - Laws energy maps (25 features).
    const int nfeatures = 25;
    Matrix *energy_maps = cvl_laws_energies(&gray, energy_window);
    cvl_mat_free(gray);

    // 3 - flatten features (npixels × nfeatures).
    double *features = malloc(npixels * nfeatures * sizeof(double));
    assert(features);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int p = i * w + j;
            for (int f = 0; f < nfeatures; ++f) {
                features[p * nfeatures + f] = energy_maps[f].map[i][j];
            }
        }
    }
    for (int f = 0; f < nfeatures; ++f) {
        cvl_mat_free(energy_maps[f]);
    }
    free(energy_maps);

    // 4 - Component Labeling (K-means Clustering).
    int *labels = malloc(npixels * sizeof(int));
    assert(labels);
    srand(42);
    cvl_kmeans(features, npixels, nfeatures, kclusters, labels, 50);
    free(features);

    // 5 - Component Coloring.
    Image dst = cvl_img_create(h, w);
    const int min_col = 40;
    const int range = 256 - 2 * min_col;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int lbl = labels[i * w + j];
            Pixel *p = &dst.map[i][j];
            p->r = min_col + (lbl * 73 % range);
            p->g = min_col + (lbl * 137 % range);
            p->b = min_col + (lbl * 211 % range);
            p->i = (p->r + p->g + p->b) / 3.0;
        }
    }
    free(labels);

    return dst;
}

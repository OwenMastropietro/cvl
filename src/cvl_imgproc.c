#include <cvl/cvl_imgproc.h>

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
static void _morph(Image *img, int pixel_value) {
    int h = img->height;
    int w = img->width;

    bool (*mask)[w] = calloc(h, sizeof(*mask));
    assert(mask);

    int dh[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dw[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    const int n_neighbors = 8;

    // Set Mask - Marking Pixels to Change.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (img->map[i][j].i == pixel_value) {
                continue;
            }

            for (int k = 0; k < n_neighbors; ++k) {
                int ni = i + dh[k];
                int nj = j + dw[k];

                bool in_bounds = ((0 <= ni && ni < h) && (0 <= nj && nj < w));
                if (in_bounds && img->map[ni][nj].i == pixel_value) {
                    mask[i][j] = true;
                    break;
                }
            }
        }
    }

    // Change Pixels According to Mask.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (mask[i][j]) {
                img->map[i][j].i = pixel_value;
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
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}

// Applies a fixed-level threshold to each array element - determined by type.
int cvl_threshold(Image *src, Image *dst, int thresh, int maxval, int type) {
    if (!src->map || !dst->map) return 0;
    if (src->height != dst->height || src->width != dst->width) return 0;
    if (!(0 <= thresh && thresh <= 255)) return 0;
    if (!(0 <= maxval && maxval <= 255)) return 0;

    const int h = src->height;
    const int w = src->width;
    const uint8_t t = (uint8_t)thresh;
    const uint8_t mv = (uint8_t)maxval;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            uint8_t v = src->map[i][j].i;

            switch (type) {
                case CVL_THRESH_BINARY:     v = (v > t) ? mv : 0; break;
                case CVL_THRESH_BINARY_INV: v = (v > t) ? 0 : mv; break;
                case CVL_THRESH_TRUNC:      v = (v > t) ? t : v;  break;
                case CVL_THRESH_TOZERO:     v = (v > t) ? v : 0;  break;
                case CVL_THRESH_TOZERO_INV: v = (v > t) ? 0 : v;  break;
            }

            Pixel *p = &dst->map[i][j];
            p->r = p->g = p->b = p->i = v;
            // todo: impl. cvl_cvtcolor and separate rgb manip.
        }
    }

    return 1;
}

// Changes all pixels below thresh to black (0), otherwise to white (255).
int cvl_binarize(Image *img, int thresh) {
    return cvl_threshold(img, img, thresh, 255, CVL_THRESH_BINARY);
}

// Randomly flips binary pixels with probability p.
void cvl_add_noise(Image *img, double p) {
    assert(0.0 <= p && p <= 1.0);

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int pixel = img->map[i][j].i; // assuming rgb irrelevant
            assert(pixel == BLACK || pixel == WHITE);

            double r = (double)rand() / RAND_MAX;
            if (r < p) {
                uint8_t bw = (pixel == BLACK) ? WHITE : BLACK;
                img->map[i][j] = (Pixel){bw, bw, bw, bw};
            }
        }
    }
}

// Rotates the image 180º.
void cvl_rotate(Image *img) {
    int h = img->height;
    int w = img->width;

    Image rotated = cvl_img_create(h, w);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            rotated.map[i][j] = img->map[h - i - 1][w - j - 1];
        }
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            img->map[i][j] = rotated.map[i][j];
        }
    }

    cvl_img_free(rotated);
}

// Inverts RGB channels according to the given max value.
void cvl_invert(Image *img, int maxval) {
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            img->map[i][j].r = maxval - img->map[i][j].r;
            img->map[i][j].g = maxval - img->map[i][j].g;
            img->map[i][j].b = maxval - img->map[i][j].b;
        }
    }
}

// Changes all pixels with black neighbors to black.
void cvl_expand(Image *img) { _morph(img, BLACK); }

// Changes all pixels with white neighbors to white.
void cvl_shrink(Image *img) { _morph(img, WHITE); }

// Performs Connected Component Labeling. Returns number of components found.
int cvl_connected_components(Image *img, Matrix *labels, int connectivity) {
    assert(img->height == labels->height && img->width == labels->width);
    assert(connectivity == 4 || connectivity == 8);

    int h = img->height;
    int w = img->width;

    UFNode *uf = malloc((h * w + 1) * sizeof(UFNode));
    assert(uf);

    int dh[] = {-1, 0, -1, -1}; // |- 0 -| or |0 0 0|
    int dw[] = {0, -1, -1, 1};  // |0 - -|    |0 - -|
    const int n_neighbors = (connectivity == 4) ? 2 : 4;
    int next_label = 1;

    // Pass I - Assign Labels and Equivalences.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (img->map[i][j].i == WHITE) {
                continue; // component = connected black pixels
            }

            int count = 0;
            int neighbor_labels[4];
            for (int k = 0; k < n_neighbors; ++k) {
                int ni = i + dh[k];
                int nj = j + dw[k];

                bool in_bounds = (0 <= ni) && (0 <= nj && nj < w);
                if (in_bounds && labels->map[ni][nj] > 0) {
                    neighbor_labels[count++] = (int)labels->map[ni][nj];
                }
            }

            if (count == 0) { // assign new label
                labels->map[i][j] = next_label;
                uf[next_label].parent = next_label;
                next_label++;
            } else { // assign exsisting label and mark equivalence
                int min_label = neighbor_labels[0];
                for (int k = 0; k < count; ++k) {
                    if (neighbor_labels[k] < min_label) {
                        min_label = neighbor_labels[k];
                    }
                }
                labels->map[i][j] = min_label;

                for (int k = 0; k < count; ++k) { // assign equivalence
                    uf_union(uf, min_label, neighbor_labels[k]);
                }
            }
        }
    }

    // Pass II - Reconcile Equivalences.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int label = (int)labels->map[i][j];
            if (label > 0) {
                labels->map[i][j] = uf_find(uf, label);
            }
        }
    }

    // Pass III - Count Unique Labels.
    bool *seen = calloc(next_label, sizeof(bool));
    assert(seen);
    int num_components = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int label = (int)labels->map[i][j];
            if (label > 0) {
                if (!seen[label]) {
                    seen[label] = true;
                    num_components++;
                }
            }
        }
    }

    free(seen);
    free(uf);

    return num_components;
}

// Colors components larger than thresh. Returns the number of such components.
int cvl_color_components(Image *img, Matrix *labels, int thresh) {
    assert(img->height == labels->height && img->width == labels->width);

    int h = img->height;
    int w = img->width;
    int max_label = h * w + 1; // or pass in via num_components ?

    int count = 0;
    int *sizes = calloc(max_label, sizeof(int));
    assert(sizes);

    // Pass I - Calculate Component Sizes.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int label = (int)labels->map[i][j];
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
        for (int j = 0; j < w; ++j) {
            int label = (int)labels->map[i][j];

            if (label > 0 && sizes[label] >= thresh) {
                img->map[i][j].r = min + (label * 37) % max;
                img->map[i][j].g = min + (label * 73) % max;
                img->map[i][j].b = min + (label * 109) % max;
            }
        }
    }

    free(sizes);

    return count;
}

// Correlate src with kernel using zero padding.
void cvl_correlate(Matrix *src, Matrix *dst, Matrix *kernel) {
    // G(r, c) = \sum_{i=-m}^{m} \sum_{j=-n}^{n} K(i, j) * I(r + i), c + j)
    assert(src->height == dst->height && src->width == dst->width);

    int h = src->height;
    int w = src->width;

    int ar = floor(kernel->height / 2.0); // anchor (row)
    int ac = floor(kernel->width / 2.0);  // anchor (column)

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            double ws = 0.0; // weighted sum
            for (int i = 0; i < kernel->height; ++i) {
                for (int j = 0; j < kernel->width; ++j) {
                    int rr = r + i - ar;
                    int cc = c + j - ac;
                    bool in_bounds = (0 <= rr && rr < h) && (0 <= cc && cc < w);
                    if (in_bounds) { // BORDER_CONSTANT
                        ws += kernel->map[i][j] * src->map[rr][cc];
                    }
                }
            }
            dst->map[r][c] = ws;
        }
    }
}

// Convolve src with kernel using zero padding.
void cvl_convolve(Matrix *src, Matrix *dst, Matrix *kernel) {
    const int h = kernel->height;
    const int w = kernel->width;
    Matrix flipped = cvl_mat_create(h, w);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            flipped.map[i][j] = kernel->map[h - 1 - i][w - 1 - j];
        }
    }

    cvl_correlate(src, dst, &flipped);
    cvl_mat_free(flipped);
}

// Blurs an image/matrix using a uniform kernel.
void cvl_blur_box(Matrix *src, Matrix *dst, int ksize, bool normalize) {
    assert(src->height == dst->height && src->width == dst->width);

    const double norm = normalize ? 1.0 / (ksize * ksize) : 1.0;

    Matrix kernel = cvl_mat_create(ksize, ksize);

    for (int i = 0; i < kernel.height; ++i) {
        for (int j = 0; j < kernel.width; ++j) {
            kernel.map[i][j] = norm;
        }
    }

    cvl_convolve(src, dst, &kernel);

    cvl_mat_free(kernel);
}

// Blurs an image/matrix using a normalized uniform kernel.
void cvl_blur_mean(Matrix *src, Matrix *dst, int ksize) {
    cvl_blur_box(src, dst, ksize, true);
}

// Apply 1D horizontal convolution.
static void cvl_convolve_x(Matrix *src, Matrix *dst, Matrix *kernel) {
    const int h = src->height;
    const int w = src->width;
    const int ksize = kernel->width;
    const int radius = ksize / 2;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double sum = 0.0;
            for (int k = -radius; k <= radius; ++k) {
                int xx = _clamp(x + k, 0, w - 1);
                sum += src->map[y][xx] * kernel->map[0][k + radius];
            }
            dst->map[y][x] = sum;
        }
    }
}

// Apply 1D vertical convolution.
static void cvl_convolve_y(Matrix *src, Matrix *dst, Matrix *kernel) {
    const int h = src->height;
    const int w = src->width;
    const int ksize = kernel->width;
    const int radius = ksize / 2;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double sum = 0.0;
            for (int k = -radius; k <= radius; ++k) {
                int yy = _clamp(y + k, 0, h - 1);
                sum += src->map[yy][x] * kernel->map[0][k + radius];
            }
            dst->map[y][x] = sum;
        }
    }
}

// Returns a normalized 1D Gaussian kernel.
static Matrix cvl_gaussian_kernel(int ksize, double sigma) {
    if (ksize < 1 || ksize % 2 == 0) {
        ksize = 5;
    }
    if (sigma <= 0.0) { // compute from ksize
        sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8; // opencv heuristic
    }

    Matrix kernel = cvl_mat_create(1, ksize);

    const int radius = ksize / 2;
    double sum = 0.0;

    for (int x = -radius; x < radius; ++x) {
        double gx = exp(-(x * x) / (2.0 * sigma * sigma));
        kernel.map[0][x + radius] = gx;
        sum += gx;
    }

    for (int x = 0; x < ksize; ++x) {
        kernel.map[0][x] /= sum;
    }

    return kernel;
}

/**
 * Blurs an image/matrix using a Gaussian kernel defined by sigma.
 * 
 * @param src Input matrix.
 * @param dst Output matrix (blurred).
 * @param sigma Gaussian kernel standard deviation.
 * 
 * @todo ksize?
 * I don't like handling both...
 * but I don't like not having kszie...
 * but it makes more sense (I think) to specify sigma)
 * @todo sigma_x & sigma_y?
 */
void cvl_blur_gauss(Matrix *src, Matrix *dst, double sigma) {
    const double radius = ceil(3.0 * sigma);
    const int ksize = 2 * radius + 1;

    Matrix kernel = cvl_gaussian_kernel(ksize, sigma);

    // todo: sepFilter2D?
    Matrix tmp = cvl_mat_create(src->height, src->width);
    cvl_convolve_x(src, &tmp, &kernel);
    cvl_convolve_y(&tmp, dst, &kernel);

    cvl_mat_free(tmp);
    cvl_mat_free(kernel);
}

// Apply median blur using replicated outlier pixel values.
void cvl_blur_median(Matrix *src, Matrix *dst, int ksize) {
    assert(src->height == dst->height && src->width == dst->width);
    assert(ksize % 2 == 1);

    int h = src->height;
    int w = src->width;

    int ar = floor(ksize / 2.0); // anchor (row)
    int ac = floor(ksize / 2.0); // anchor (column)
    double *kernel = calloc(ksize * ksize, sizeof(double));
    assert(kernel);

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            for (int i = 0; i < ksize; ++i) {
                for (int j = 0; j < ksize; ++j) {
                    int rr = _clamp(r + i - ar, 0, h - 1);
                    int cc = _clamp(c + j - ac, 0, w - 1);
                    int k = (i * ksize) + j;

                    kernel[k] = src->map[rr][cc]; // BORDER_REPLICATE

                    /*
                    bool in_bounds = (0 <= rr && rr < h) && (0 <= cc && cc < w);
                    if (in_bounds) { // BORDER_CONSTANT
                        kernel[k] = src->map[rr][cc];
                    } else {
                        kernel[k] = 0.0;
                    }
                    */
                }
            }
            dst->map[r][c] = _median(kernel, ksize * ksize);
        }
    }

    free(kernel);
}

void cvl_sobel(Matrix *src, Matrix *gx, Matrix *gy) {
    double xvals[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1,
    };
    double yvals[] = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    };

    Matrix xkernel = cvl_mat_create_from(xvals, 3, 3);
    Matrix ykernel = cvl_mat_create_from(yvals, 3, 3);

    cvl_convolve(src, gx, &xkernel);
    cvl_convolve(src, gy, &ykernel);
}

void cvl_laplacian(const Matrix *src, Matrix *dst) {
    // double vals[] = { // 4-neighborhood
    //     0,  1, 0,
    //     1, -4, 1,
    //     0,  1, 0,
    // };
    double vals[] = { // 8-neighborhood
        1,  1, 1,
        1, -8, 1,
        1,  1, 1,
    };

    Matrix kernel = cvl_mat_create_from(vals, 3, 3);

    cvl_convolve(src, dst, &kernel);

    cvl_mat_free(kernel);
}

static void cvl_mag(Matrix *g, Matrix *gx, Matrix *gy) {
    for (int i = 0; i < g->height; ++i) {
        for (int j = 0; j < g->width; ++j) {
            g->map[i][j] = sqrt(SQR(gx->map[i][j]) + SQR(gy->map[i][j]));
        }
    }
}

static void cvl_ang(Matrix *a, Matrix *gx, Matrix *gy) {
    for (int i = 0; i < a->height; ++i) {
        for (int j = 0; j < a->width; ++j) {
            a->map[i][j] = atan2(gy->map[i][j], gx->map[i][j]);
        }
    }
}

// Non-maximum Suppression.
static void cvl_nms(Matrix *m, Matrix *a, Matrix *e) {
    assert(m && m->map);
    assert(a && a->map);
    assert(e && e->map);

    assert(m->height == a->height);
    assert(m->height == e->height);
    assert(m->width == m->width);
    assert(m->width == e->width);

    const int h = m->height;
    const int w = m->width;

    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            double angle = a->map[i][j];
            if (angle < 0) angle += M_PI; // [0, pi]

            int sector = (int)((angle + M_PI / 8) / (M_PI / 4)) % 4;
            // sectors
            // 3 2 1
            // 0   0
            // 1 2 3

            switch (sector) {
                case 0: { // 0°, 180° (horizontal)
                    // double n1 = (j - 1 >= 0) ? m->map[i][j - 1] : 0.0;
                    // double n2 = (j + 1 <  w) ? m->map[i][j + 1] : 0.0;
                    double n1 = m->map[i][j - 1];
                    double n2 = m->map[i][j + 1];
                    if (m->map[i][j] >= n1 && m->map[i][j] >= n2) {
                        e->map[i][j] = m->map[i][j];
                        break;
                    }
                    e->map[i][j] = 0;
                    break;
                }
                case 1: { // 45°, 225° (diagonal)
                    double n1 = m->map[i - 1][j + 1];
                    double n2 = m->map[i + 1][j - 1];
                    if (m->map[i][j] >= n1 && m->map[i][j] >= n2) {
                        e->map[i][j] = m->map[i][j];
                        break;
                    }
                    e->map[i][j] = 0;
                    break;
                }
                case 2: { // 90°, 270° (vertical)
                    double n1 = m->map[i - 1][j];
                    double n2 = m->map[i + 1][j];
                    if (m->map[i][j] >= n1 && m->map[i][j] >= n2) {
                        e->map[i][j] = m->map[i][j];
                        break;
                    }
                    e->map[i][j] = 0;
                    break;
                }
                case 3: { // 135°, 315° (diagonal)
                    double n1 = m->map[i - 1][j - 1];
                    double n2 = m->map[i + 1][j + 1];
                    if (m->map[i][j] >= n1 && m->map[i][j] >= n2) {
                        e->map[i][j] = m->map[i][j];
                        break;
                    }
                    e->map[i][j] = 0;
                    break;
                }
                default: assert(false);
            }
        }
    }
}

// Hysteresis Thresholding.
static void cvl_ht(Matrix *src, Matrix *dst, int lo, int hi) {
    assert(src && src->map);
    assert(dst && dst->map);
    assert(src->height == dst->height);
    assert(src->width == dst->width);

    const int h = src->height;
    const int w = src->width;

    enum { N = 0, C = 1, E = 2 }; // non-edge, candidate, edge

    Matrix tmp = cvl_mat_create(h, w);

    // Classify Pixels.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            double v = src->map[i][j];
            if (v < lo)      tmp.map[i][j] = N;
            else if (v > hi) tmp.map[i][j] = E;
            else             tmp.map[i][j] = C;
        }
    }

    // BFS - flood fill candidates (C) from strong edges (E).
    const int n_neighbors = 8;
    int dh[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dw[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    typedef struct { int i, j; } Point;
    Point *queue = malloc(h * w * sizeof(Point));
    assert(queue);
    int front = 0, back = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (tmp.map[i][j] == E) {
                queue[back++] = (Point){i, j};
            }
        }
    }

    while (front < back) {
        if (back >= h * w) perror("queue overflow\n");
        Point p = queue[front++];

        for (int k = 0; k < n_neighbors; ++k) {
            int ni = p.i + dh[k];
            int nj = p.j + dw[k];

            bool in_bounds = (0 <= ni && ni < h) && (0 <= nj && nj < w); 
            if (!in_bounds) continue;

            if (tmp.map[ni][nj] == C) {
                tmp.map[ni][nj] = E;
                if (back >= h * w) perror("queue overflow 1\n");
                queue[back++] = (Point){ni, nj};
                if (back >= h * w) perror("queue overflow 2\n");
            }
        }
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            dst->map[i][j] = (tmp.map[i][j] == E) ? src->map[i][j] : 0.0;
        }
    }

    free(queue);
    cvl_mat_free(tmp);
}

void cvl_canny(Matrix *src, Matrix *dst, double sigma, int lo, int hi) {
    const int h = src->height;
    const int w = src->width;

    // 1 - Gaussian(less) Filter.
    Matrix smoothed = cvl_blur_gauss_new(src, sigma);

    // 2 - Magnitude(Gx, Gy) & Orientation(Gx, Gy) from Sobel.
    Matrix gx = cvl_mat_create(h, w);
    Matrix gy = cvl_mat_create(h, w);
    cvl_sobel(&smoothed, &gx, &gy);

    Matrix mag = cvl_mat_create(h, w);
    Matrix ang = cvl_mat_create(h, w);
    cvl_mag(&mag, &gx, &gy);
    cvl_ang(&ang, &gx, &gy);

    // 3 - Non-max Suppression.
    Matrix nms = cvl_mat_create(h, w);
    cvl_nms(&mag, &ang, &nms);

    // 4 - Hysteresis Thresholding.
    cvl_ht(&nms, dst, lo, hi);

    cvl_mat_free(nms);
    cvl_mat_free(ang);
    cvl_mat_free(mag);
    cvl_mat_free(gy);
    cvl_mat_free(gx);
    cvl_mat_free(smoothed);
}

// Convenience "_new" wrappers

Image cvl_threshold_new(Image *src, int thresh, int maxval, int type) {
    Image dst = cvl_img_create(src->height, src->width);
    cvl_threshold(src, &dst, thresh, maxval, type);
    return dst;
}

Image cvl_binarize_new(Image *src, int thresh) {
    Image dst = cvl_img_copy(src);
    cvl_binarize(&dst, thresh);
    return dst;
}

Matrix cvl_correlate_new(Matrix *src, Matrix *kernel) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_correlate(src, &dst, kernel);
    return dst;
}

Matrix cvl_convolve_new(Matrix *src, Matrix *kernel) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_convolve(src, &dst, kernel);
    return dst;
}

Matrix cvl_blur_box_new(Matrix *src, int ksize, bool normalize) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_blur_box(src, &dst, ksize, normalize);
    return dst;
}

Matrix cvl_blur_mean_new(Matrix *src, int ksize) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_blur_mean(src, &dst, ksize);
    return dst;
}

Matrix cvl_blur_gauss_new(Matrix *src, double sigma) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_blur_gauss(src, &dst, sigma);
    return dst;
}

Matrix cvl_blur_median_new(Matrix *src, int ksize) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_blur_median(src, &dst, ksize);
    return dst;
}

Matrix cvl_sobel_mag(Matrix *src) {
    Matrix g = cvl_mat_create(src->height, src->width);
    Matrix gx = cvl_mat_create(src->height, src->width);
    Matrix gy = cvl_mat_create(src->height, src->width);
    cvl_sobel(src, &gx, &gy);
    cvl_mag(&g, &gx, &gy);

    return g;
}

Matrix cvl_sobel_angle(Matrix *src) {
    Matrix a = cvl_mat_create(src->height, src->width);
    Matrix gx = cvl_mat_create(src->height, src->width);
    Matrix gy = cvl_mat_create(src->height, src->width);
    cvl_sobel(src, &gx, &gy);
    cvl_ang(&a, &gx, &gy);

    return a;
}

Matrix cvl_laplacian_new(const Matrix *src) {
    Matrix dst = cvl_mat_create(src->height, src->width);
    cvl_laplacian(src, &dst);
    return dst;
}

Matrix cvl_canny_new(Matrix *src, double sigma, int lo, int hi) {
    Matrix canny_edges = cvl_mat_create(src->height, src->width);
    cvl_canny(src, &canny_edges, sigma, lo, hi);

    return canny_edges;
}

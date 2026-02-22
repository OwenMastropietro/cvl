#include "cvl.h"
#include <assert.h>
#include <stdbool.h>
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

// Changes all pixels below thresh to black (0), otherwise to white (255).
void cvl_threshold(Image *img, int thresh) {
    unsigned char r, g, b;

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            r = img->map[i][j].r;
            g = img->map[i][j].g;
            b = img->map[i][j].b;

            int gray = 0.299 * r + 0.587 * g + 0.114 * b; // ITU-R BT.601-7
            unsigned char bw = (gray < thresh) ? BLACK : WHITE;
            // setPixel(*img, i, j, bw, bw, bw, bw);
            img->map[i][j] = (Pixel){bw, bw, bw, bw};
        }
    }
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
                unsigned char bw = (pixel == BLACK) ? WHITE : BLACK;
                img->map[i][j] = (Pixel){bw, bw, bw, bw};
            }
        }
    }
}

// Rotates the image 180º.
void cvl_rotate(Image *img) {
    int h = img->height;
    int w = img->width;

    Image rotated = createImage(h, w);
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

    deleteImage(rotated);
}

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

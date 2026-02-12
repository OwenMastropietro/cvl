#include "cvl.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

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

// Changes all pixels with a pixel_value neighbor to pixel_value.
static void _morph(Image *img, int pixel_value) {
    int h = img->height;
    int w = img->width;

    bool (*mask)[w] = calloc(h, sizeof(*mask));
    assert(mask);

    const int NUM_NEIGHBORS = 8;
    int dh[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dw[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Set Mask - Marking Pixels to Change.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (img->map[i][j].i == pixel_value) {
                continue;
            }

            for (int k = 0; k < NUM_NEIGHBORS; ++k) {
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

// Changes all pixels with black neighbors to black.
void cvl_expand(Image *img) { _morph(img, BLACK); }

// Changes all pixels with white neighbors to white.
void cvl_shrink(Image *img) { _morph(img, WHITE); }

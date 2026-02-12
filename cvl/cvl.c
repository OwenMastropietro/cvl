#include "cvl.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Converts the image to black and white according to the threshold,
 * (black < threshold <= white)
 */
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

void cvl_add_noise(Image *img, double p) {
    assert(0.0 <= p && p <= 1.0);
    srand(42);

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int pixel = img->map[i][j].i; // assuming rgb irrelevant
            assert(pixel == BLACK || pixel == WHITE);

            float r = (float)rand() / RAND_MAX;
            if (r < p) {
                img->map[i][j].i = (pixel == BLACK) ? WHITE : BLACK;
            }
        }
    }
}

static void _morph(Image *img, int pixel_value) {
    int h = img->height;
    int w = img->width;

    bool (*mask)[w] = calloc(h, sizeof(*mask));
    assert(mask);

    const int NUM_NEIGHBORS = 8;
    int dh[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dw[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Set Mask.
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

    // Expand Image (according to mask).
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (mask[i][j]) {
                img->map[i][j].i = pixel_value;
            }
        }
    }

    free(mask);
}

void cvl_expand(Image *img) { _morph(img, BLACK); }

void cvl_shrink(Image *img) { _morph(img, WHITE); }

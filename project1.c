/**
 * project1.c
 *
 * Homework 1 - Exploring a Simple Image Processing Library.
 */

#include "netpbm/netpbm.h"
#include <assert.h>
#include <stdio.h>

#define BLACK 0
#define WHITE 255

/**
 * Converts the image to a black and white image,
 * forcing pixel values below the threshold to black, and the rest to white.
 */
void cvt2binary(Image *img, int threshold) {
    for (int r = 0; r < img->height; ++r) {
        for (int c = 0; c < img->width; ++c) {
            int intensity = ( // ITU-R BT.601-7 standard (formerly CCIR 601)
                0.299 * img->map[r][c].r
                + 0.587 * img->map[r][c].g
                + 0.114 * img->map[r][c].b
            );
            intensity = (intensity < threshold) ? BLACK : WHITE;
            setPixel(*img, r, c, NO_CHANGE, NO_CHANGE, NO_CHANGE, intensity);
        }
    }
}

int main(void) {
    Image img = readImage("./data/original/car.ppm");

    // Note: I think writeImage automatically does this... but for the sake of
    // defining a conversion function...

    cvt2binary(&img, 128);
    writeImage(img, "./data/modified/black_white.pbm");

    deleteImage(img);

    return 0;
}
// Example Program: Interfacing with CVL and NetPBM.

#include "cvl/cvl.h"
#include "netpbm/netpbm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    Image img = readImage("./data/original/car.ppm");

    // cvl_rotate(&img);
    // writeImage(img, "./data/modified/car_rotated.ppm");

    // cvl_invert(&img, 255);
    // writeImage(img, "./data/modified/car_inverted.ppm");

    cvl_threshold(&img, 128);
    writeImage(img, "./data/modified/car_bw.pbm");

    srand(42);
    cvl_add_noise(&img, 0.1);
    writeImage(img, "./data/modified/car_bw_noisy.pbm");

    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_expand(&img);
    cvl_shrink(&img);
    writeImage(img, "./data/modified/car_bw_cleared.pbm");

    deleteImage(img);

    return 0;
}

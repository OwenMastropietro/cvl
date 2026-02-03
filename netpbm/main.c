/**
 * main.c
 *
 * CS136
 * Created by nha2 on 8/27/24.
 * Modified by Owen Mastropietro on 02/03/26.
 *
 * Test and demo program for netpbm. Reads a sample image and creates several
 * output images.
 */

#include "netpbm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
    Image inputImage = readImage("./data/original/car.ppm");

    Image rotatedImage = createImage(inputImage.height, inputImage.width);
    Image invertedImage = createImage(inputImage.height, inputImage.width);

    for (int x = 0; x < inputImage.width; x++) {
        for (int y = 0; y < inputImage.height; y++) {
            rotatedImage.map[y][x] = inputImage.map[inputImage.height - y - 1][inputImage.width - x - 1];

            invertedImage.map[y][x].r = 255 - inputImage.map[y][x].r;
            invertedImage.map[y][x].g = 255 - inputImage.map[y][x].g;
            invertedImage.map[y][x].b = 255 - inputImage.map[y][x].b;
            // Ignore 'i' here; it's irrelevant if we want to save image as PPM.
        }
    }

    writeImage(rotatedImage, "./data/modified/rotated.pbm");
    writeImage(invertedImage, "./data/modified/inverted.ppm");
    writeImage(inputImage, "./data/modified/gray.pgm");
    writeImage(inputImage, "./data/modified/black-white.pbm");

    // ------------------------------------------------------------------------
    // create blackWhiteImage:

    // create noiseImage:

    // create expandImage:

    // create shrinkImage:

    // ------------------------------------------------------------------------
    // Uncomment this after you finish your homework
    // Function that does threshold, noise and numbers of spanding and shrinking
    // COMMENT THIS FUNCTION IF YOU DON'T WANT IT TO RUN EVEY TIME
    // function_readImage();

    /* Delete black and white, noise, Expand and Shrink */
    deleteImage(inputImage);
    deleteImage(rotatedImage);
    deleteImage(invertedImage);

    printf("Program ends ... ");

    return 0;
}

// ---------------- function_imageBlackWhite ----------------------------------
/* function that receives an Image structure and an intensity threshold
 to convert each pixel in the image to either black (intensity = 0)
 or white (intensity = 255). The function should return an Image structure
 containing the result. */

// ---------------- Expand function -------------------------------------------
/* Expand operation */

// ---------------- Shrink function -------------------------------------------
/* Shrink operation */

// ---------------- noise function --------------------------------------------
/* function that adds binary noise to an image. This function receives an
 image and a floating point number p that indicates the probability
 (in percent) that each pixel in the image will be flipped, i.e.,
 turned from black to white or vice versa.
 */

/**
 * Functions for reading and writing binary PBM, PGM, and PPM image files.
 *
 * Based on template from (V2.2 by Marc Pomplun on 10/19/2013).
 */

#pragma once

#define BLACK 0   // off - "absense of all color"
#define WHITE 255 // on - "presence of all color"

#define SQR(x) ((x) * (x))
#define PI 3.14159265358979323846
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

// Additional color options for drawing lines and shapes.
#define NO_CHANGE -1
#define INVERT -2

typedef struct {
    unsigned char r, g, b, i;
} Pixel;

typedef struct {
    int height, width;
    Pixel **map;
} Image;

typedef struct {
    int height, width;
    double **map;
} Matrix;

// The supported file types using 1, 8, and 24 bits per pixel, respectively.
typedef enum format { PBM, PGM, PPM } Format;

Image createImage(int height, int width);

void deleteImage(Image img);

Image readImage(char *filename);

int cvl_imwrite(const char *filename, Image *img);

Matrix createMatrix(int height, int width);

Matrix createMatrixFromArray(double *entry, int height, int width);

void deleteMatrix(Matrix mx);

Matrix image2Matrix(Image img);

Image matrix2Image(Matrix mx, int scale, double gamma);

// todo: move these somewhere else?

void setPixel(Image img, int vPos, int hPos, int r, int g, int b, int i);

void filledEllipse(Image img, int vCenter, int hCenter, int vRadius, int hRadius, int r, int g, int b, int i);

void filledRectangle(Image img, int v1, int h1, int v2, int h2, int r, int g, int b, int i);

void line(Image img, int v1, int h1, int v2, int h2, int width, int dash, int gap, int r, int g, int b, int i);

void rectangle(Image img, int v1, int h1, int v2, int h2, int width, int dash, int gap, int r, int g, int b, int i);

void ellipse(Image img, int vCenter, int hCenter, int vRadius, int hRadius, int width, int dash, int gap, int r, int g, int b, int i);

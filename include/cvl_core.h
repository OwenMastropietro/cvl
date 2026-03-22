#pragma once

#define BLACK 0   // off - "absense of all color"
#define WHITE 255 // on - "presence of all color"
#define SQR(x) ((x) * (x))
#define PI 3.14159265358979323846
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
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

// image formats
typedef enum cvl_format {
    CVL_FMT_PBM,
    CVL_FMT_PGM,
    CVL_FMT_PPM,
} cvl_format;

Image cvl_img_create(int height, int width);

void cvl_img_free(Image img);

Matrix cvl_mat_create(int height, int width);

Matrix cvl_mat_create_from(double *entry, int height, int width);

void cvl_mat_free(Matrix mx);

Matrix cvl_img2mat(Image img);

Image cvl_mat2img(Matrix mx, int scale, double gamma);

// todo: idk where to put these

void setPixel(Image img, int vPos, int hPos, int r, int g, int b, int i);

void filledEllipse(Image img, int vCenter, int hCenter, int vRadius,
                   int hRadius, int r, int g, int b, int i);

void filledRectangle(Image img, int v1, int h1, int v2, int h2, int r, int g,
                     int b, int i);

void line(Image img, int v1, int h1, int v2, int h2, int width, int dash,
          int gap, int r, int g, int b, int i);

void rectangle(Image img, int v1, int h1, int v2, int h2, int width, int dash,
               int gap, int r, int g, int b, int i);

void ellipse(Image img, int vCenter, int hCenter, int vRadius, int hRadius,
             int width, int dash, int gap, int r, int g, int b, int i);

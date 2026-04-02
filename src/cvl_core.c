#define _CRT_SECURE_NO_WARNINGS

#include <cvl/cvl_core.h>
#include <cvl/cvl_io.h>

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new image of the given size and fill it with white pixels.
Image cvl_img_create(int height, int width) {
    // todo: zero-fill
    return cvl_img_create_fill(height, width, WHITE);
}

// Create a new image of the given size and fill it with pixel value.
Image cvl_img_create_fill(int height, int width, int pixel_value) {
    int i, j;
    Image img;

    img.height = height;
    img.width = width;

    img.map = (Pixel **)malloc(sizeof(Pixel *) * height);
    for (i = 0; i < height; i++) {
        img.map[i] = (Pixel *)malloc(sizeof(Pixel) * width);
        for (j = 0; j < width; j++) {
            img.map[i][j].r = pixel_value;
            img.map[i][j].g = pixel_value;
            img.map[i][j].b = pixel_value;
            img.map[i][j].i = pixel_value;
        }
    }

    return img;
}

// Copy an input image.
Image cvl_img_copy(Image *src) {
    Image dst = cvl_img_create(src->height, src->width);

    for (int i = 0; i < src->height; ++i) {
        for (int j = 0; j < src->width; ++j) {
            dst.map[i][j] = src->map[i][j];
        }
    }

    return dst;
}

// Delete a previously created image and free its allocated memory on the heap.
void cvl_img_free(Image img) {
    for (int i = 0; i < img.height; ++i) {
        free(img.map[i]);
    }
    free(img.map);
}

// Create a new matrix of the given size and fill it with zeroes.
Matrix cvl_mat_create(int height, int width) {
    int i, j;
    Matrix mx;

    mx.height = height;
    mx.width = width;

    mx.map = (double **)malloc(sizeof(double *) * height);
    for (i = 0; i < height; i++) {
        mx.map[i] = (double *)malloc(sizeof(double) * width);
        for (j = 0; j < width; j++) {
            mx.map[i][j] = 0.0;
        }
    }

    return mx;
}

// Create a new matrix of the given size and fill it with content of 2D double array.
Matrix cvl_mat_create_from(double *entry, int height, int width) {
    int i, j;
    Matrix mx;

    mx.height = height;
    mx.width = width;

    mx.map = (double **)malloc(sizeof(double *) * height);
    for (i = 0; i < height; i++) {
        mx.map[i] = (double *)malloc(sizeof(double) * width);
        for (j = 0; j < width; j++) {
            mx.map[i][j] = *(entry++);
        }
    }

    return mx;
}

// Delete a previously created matrix and free its allocated memory on the heap.
void cvl_mat_free(Matrix mat) {
    for (int i = 0; i < mat.height; ++i) {
        free(mat.map[i]);
    }
    free(mat.map);
}

// Convert the intensity components of an image into a matrix of identical size.
Matrix cvl_img2mat(Image img) {
    int m, n;
    Matrix result = cvl_mat_create(img.height, img.width);

    for (m = 0; m < img.height; m++) {
        for (n = 0; n < img.width; n++) {
            result.map[m][n] = (double)img.map[m][n].i;
        }
    }

    return result;
}

// Converts a matrix to an image with scaling and gamma correction.
// - scale == 0: values are 1/255 normalized before applying gamma.
// - scale != 0: values are min/max normalized before applying gamma.
// - gamma == 1.0: linear scaling (no change in contrast).
// - gamma < 1.0: enhances darker values (brightens the image).
// - gamma > 1.0: suppressess darker values (darkens the image).
//
// The final result is clamped to [0, 255].
Image cvl_mat2img(Matrix mat, int scale, double gamma) {
    const int h = mat.height;
    const int w = mat.width;

    double vmin = DBL_MAX;
    double vmax = -DBL_MAX;

    if (scale) {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                double v = mat.map[i][j];

                if (v < vmin) vmin = v;
                if (v > vmax) vmax = v;
            }
        }
        if (vmax - vmin < 1e-10) vmax += 1.0;
    }

    double range = vmax - vmin;

    Image img = cvl_img_create(h, w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Pixel *p = &img.map[i][j];
            double v = mat.map[i][j];

            double norm = scale ? (v - vmin) / range : v / 255.0;
            double out  = 255.0 * pow(norm, gamma);

            out = fmin(fmax(out, 0.0), 255.0); // clamp to [0, 255]

            uint8_t val = (uint8_t)(out + 0.5);

            p->r = p->g = p->b = p->i = val;
        }
    }

    return img;
}

/**
 * Set the color for pixel (row=y, col=x) in the image.
 * 
 * Params r, g, b, and i accept:
 * - -2 to ignore - keeps previously stored channel value.
 * - -1 to invert - inverts channel value (255 - v).
 * - [0, 255] to change color - sets channel value to new value.
 */
void cvl_set_pixel(Image img, int row, int col, int r, int g, int b, int i) {
    if (img.map == NULL) return;
    if ((unsigned)row >= img.height || (unsigned)col >= img.width) return;

    Pixel *p = &img.map[row][col];

    if (r == INVERT)             p->r = 255 - p->r;
    else if ((unsigned)r <= 255) p->r = r;

    if (g == INVERT)             p->g = 255 - p->g;
    else if ((unsigned)g <= 255) p->g = g;

    if (b == INVERT)             p->b = 255 - p->b;
    else if ((unsigned)b <= 255) p->b = b;

    if (i == INVERT)             p->i = 255 - p->i;
    else if ((unsigned)i <= 255) p->i = i;
}

/**
 * Draw an ellipse at `center` with `radius`.
 *
 * @param center Ellipse center.
 * @param radius Exllipse radius ({0, 0} will draw an individual pixel).
 */
void cvl_draw_ellipse_filled(Image img, Point center, Point radius, int r, int g, int b, int i) {
    const int cx = center.x;
    const int cy = center.y;

    const int rx = radius.x;
    const int ry = radius.y;

    if (ry == 0 && rx == 0) {
        cvl_set_pixel(img, cy, cx, r, g, b, i);
        return;
    }

    int dx;

    for (int y = -ry; y <= ry; y++) {
        if (ry == 0) {
            dx = rx;
        } else {
            dx = (int)((double)rx * sqrt(1.0 - SQR((double)y / (double)ry)) +
                       0.5);
        }

        for (int x = -dx; x <= dx; x++) {
            cvl_set_pixel(img, cy + y, cx + x, r, g, b, i);
        }
    }
}

// Draw a rectangle with opposite corners `p1` and `p2`.
void cvl_draw_rectangle_filled(Image img, Point p1, Point p2, int r, int g, int b, int i) {
    int x1 = MIN(p1.x, p2.x);
    int y1 = MIN(p1.y, p2.y);

    int x2 = MAX(p1.x, p2.x);
    int y2 = MAX(p1.y, p2.y);

    for (int y = y1; y <= y2; y++) {
        for (int x = x1; x <= x2; x++) {
            cvl_set_pixel(img, y, x, r, g, b, i);
        }
    }
}

static inline int _should_draw_dash(int d, int total_distance, int dash,
                                    int gap) {
    if (dash * gap == 0)
        return 1;

    int cycle = dash + gap;
    int in_dash = (d % cycle < dash);
    int in_final_dash = (total_distance % cycle >= dash &&
                         d > total_distance - (total_distance % cycle));

    return in_dash || in_final_dash;
}

/**
 * Draw a straight line between `p1` and `p2`.
 *
 * @param width Width of the line (0 for single-pixel width).
 * @param dash Length (in pixels) of the dashes (0 for solid line).
 * @param gap Length (in pixels) of the gaps between dashes (0 for solid line).
 */
void cvl_draw_line(Image img, Point p1, Point p2, int width, int dash, int gap, int r, int g, int b, int i) {
    const int x1 = p1.x;
    const int y1 = p1.y;

    const int x2 = p2.x;
    const int y2 = p2.y;

    const int dx = x2 - x1;
    const int dy = y2 - y1;

    if (dx == 0 && dy == 0) {
        cvl_draw_ellipse_filled(img, p1, (Point){width, width}, r, g, b, i);
        return;
    }

    int total_distance = (int)sqrt((double)SQR(dy) + SQR(dx));

    int x, y;
    int step;
    double slope;

    if (abs(dx) > abs(dy)) { // col-major
        slope = (double)(dy) / (double)(dx);
        step = (dx > 0) ? 1 : -1;

        for (x = x1; x != x2 + step; x += step) {
            y = y1 + (int)(slope * (double)(x - x1) + 0.5);

            int d = (int)sqrt((double)(SQR(x - x1) + SQR(y - y1)));

            if (_should_draw_dash(d, total_distance, dash, gap)) {
                Point center = {x, y};
                Point radius = {width, width};
                cvl_draw_ellipse_filled(img, center, radius, r, g, b, i);
            }
        }

    } else { // row-major
        slope = (double)(dx) / (double)(dy);
        step = (dy > 0) ? 1 : -1;

        for (y = y1; y != y2 + step; y += step) {
            x = x1 + (int)(slope * (double)(y - y1) + 0.5);

            int d = (int)sqrt((double)(SQR(x - x1) + SQR(y - y1)));

            if (_should_draw_dash(d, total_distance, dash, gap)) {
                Point center = {x, y};
                Point radius = {width, width};
                cvl_draw_ellipse_filled(img, center, radius, r, g, b, i);
            }
        }
    }
}

void cvl_draw_line_solid(Image img, Point p1, Point p2, int r, int g, int b, int i) {
    cvl_draw_line(img, p1, p2, 0, 0, 0, r, g, b, i);
}

/**
 * Draw a rectangle with opposite corners `p1` and `p2`.
 * 
 * @param width Width of the line (0 for single-pixel width).
 * @param dash Length (in pixels) of the dashes (0 for solid line).
 * @param gap Length (in pixels) of the gaps between dashes (0 for solid line).
 */
void cvl_draw_rectangle(Image img, Point p1, Point p2, int width, int dash, int gap, int r, int g, int b, int i) {
    const int x1 = p1.x;
    const int y1 = p1.y;

    const int x2 = p2.x;
    const int y2 = p2.y;

    Point tl = {MIN(x1, x2), MIN(y1, y2)};
    Point br = {MAX(x1, x2), MAX(y1, y2)};
    Point tr = {br.x, tl.y};
    Point bl = {tl.x, br.y};

    cvl_draw_line(img, tl, bl, width, dash, gap, r, g, b, i); // left
    cvl_draw_line(img, bl, br, width, dash, gap, r, g, b, i); // bottom
    cvl_draw_line(img, br, tr, width, dash, gap, r, g, b, i); // right
    cvl_draw_line(img, tr, tl, width, dash, gap, r, g, b, i); // top
}

/**
 * Draw an ellipse at `center` with `radius`.
 * 
 * @param width Width of the line (0 for single-pixel width).
 * @param dash Length (in pixels) of the dashes (0 for solid line).
 * @param gap Length (in pixels) of the gaps between dashes (0 for solid line).
 */
void cvl_draw_ellipse(Image img, Point center, Point radius, int width, int dash, int gap, int r, int g, int b, int i) {
    const int cy = center.y;
    const int cx = center.x;
    const int ry = radius.y;
    const int rx = radius.x;

    int prev_y = -100;
    int prev_x = -100;
    int prev_prev_y = -100;
    int prev_prev_x = -100;

    int prev_was_drawn = 0;
    int distance = 0;

    double step = PI / 2.0 / (double)(ry + rx);

    for (double alpha = 0.0; alpha < 2.0 * PI; alpha += step) {
        int x = cx + (int)((double)rx * cos(alpha));
        int y = cy + (int)((double)ry * sin(alpha));

        if (x == prev_x && y == prev_y) continue;

        // Adjacent.
        if (abs(x - prev_prev_x) <= 1 && abs(y - prev_prev_y) <= 1) {
            if (dash * gap == 0 || distance % (dash + gap) < dash) {
                Point _center = {x, y};
                Point _radius = {width, width};
                cvl_draw_ellipse_filled(img, _center, _radius, r, g, b, i);
            }

            prev_prev_y = -1;
            prev_prev_x = -1;
            prev_was_drawn = 1;
            distance++;

        } else {
            if (!prev_was_drawn) {
                if ((dash * gap == 0 || distance % (dash + gap) < dash) && prev_y > -100) {
                    Point _center = {prev_x, prev_y};
                    Point _radius = {width, width};
                    cvl_draw_ellipse_filled(img, _center, _radius, r, g, b, i);
                }

                distance++;
            }

            prev_prev_y = prev_y;
            prev_prev_x = prev_x;
            prev_was_drawn = 0;
        }

        prev_y = y;
        prev_x = x;
    }
}

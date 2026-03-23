#define _CRT_SECURE_NO_WARNINGS

#include "cvl_io.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new image of the given size and fill it with white pixels.
Image cvl_img_create(int height, int width) {
    int i, j;
    Image img;

    img.height = height;
    img.width = width;

    img.map = (Pixel **)malloc(sizeof(Pixel *) * height);
    for (i = 0; i < height; i++) {
        img.map[i] = (Pixel *)malloc(sizeof(Pixel) * width);
        for (j = 0; j < width; j++) {
            img.map[i][j].r = 255;
            img.map[i][j].g = 255;
            img.map[i][j].b = 255;
            img.map[i][j].i = 255;
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

// Set color for pixel (vPos, hPos) in image img.
// If r, g, b, or i are set to NO_CHANGE, the corresponding color channels are
// left unchanged in img. If they are set to INVERT, the corresponding channels
// are inverted, i.e., set to 255 minus their original value
void setPixel(Image img, int vPos, int hPos, int r, int g, int b, int i) {
    if (
		vPos >= 0
		&& vPos < img.height
		&& hPos >= 0
		&& hPos < img.width
		&& img.map != NULL
	) {
        if (r == INVERT)
            img.map[vPos][hPos].r = 255 - img.map[vPos][hPos].r;
        else if (r >= 0 && r <= 255)
            img.map[vPos][hPos].r = r;

        if (g == INVERT)
            img.map[vPos][hPos].g = 255 - img.map[vPos][hPos].g;
        else if (g >= 0 && g <= 255)
            img.map[vPos][hPos].g = g;

        if (b == INVERT)
            img.map[vPos][hPos].b = 255 - img.map[vPos][hPos].b;
        else if (b >= 0 && b <= 255)
            img.map[vPos][hPos].b = b;

        if (i == INVERT)
            img.map[vPos][hPos].i = 255 - img.map[vPos][hPos].i;
        else if (i >= 0 && i <= 255)
            img.map[vPos][hPos].i = i;
    }
}

// Draw filled ellipse in image img centered at (vCenter, hCenter) with radii
// vRadius and hRadius. Radius (0, 0) will draw an individual pixel. For setting
// the r, g, b, and i color values, see setPixel function.
void filledEllipse(Image img, int vCenter, int hCenter, int vRadius, int hRadius, int r, int g, int b, int i) {
    int m, n, hSpan;
    if (vRadius == 0 && hRadius == 0) {
        setPixel(img, vCenter, hCenter, r, g, b, i);
    } else {
        for (m = -vRadius; m <= vRadius; m++) {
            if (vRadius == 0) {
                hSpan = hRadius;
            } else {
                hSpan = (int)(
					(double)hRadius * sqrt(1.0 - SQR((double)m / (double)vRadius))
					+ 0.5
				);
            }
            for (n = -hSpan; n <= hSpan; n++) {
                setPixel(img, vCenter + m, hCenter + n, r, g, b, i);
            }
        }
    }
}

// Draw filled rectangle in image img with opposite edges (v1, h1) and (v2, h2).
// For setting the r, g, b, and i color values, see setPixel function.
void filledRectangle(Image img, int v1, int h1, int v2, int h2, int r, int g, int b, int i) {
    int m, n, m1 = v1, n1 = h1, m2 = v2, n2 = h2;

    if (v1 > v2) {
        m1 = v2;
        m2 = v1;
    }
    if (h1 > h2) {
        n1 = h2;
        n2 = h1;
    }
    for (m = m1; m <= m2; m++) {
        for (n = n1; n <= n2; n++) {
            setPixel(img, m, n, r, g, b, i);
        }
    }
}

// Draw straight line in image img between (v1, h1) and (v2, h2) with a given
// width, dash pattern, and color. Width 0 indicates single-pixel width. The
// inputs dash and gap determine the length in pixels of the dashes and the gaps
// between them, resp. Use 0 for either input to draw a solid line. For setting
// the r, g, b, and i color values, see setPixel function.
void line(Image img, int v1, int h1, int v2, int h2, int width, int dash, int gap, int r, int g, int b, int i) {
    int h, v, direction, distance = (int)sqrt((double)SQR(v2 - v1) + SQR(h2 - h1)), distanceCovered;
    double slope;

    if (v1 == v2 && h1 == h2) {
        filledEllipse(img, v1, h1, width, width, r, g, b, i);
        return;
    }

    if (abs(h2 - h1) > abs(v2 - v1)) {
        slope = (double)(v2 - v1) / (double)(h2 - h1);
        direction = (h2 > h1) ? 1 : -1;
        for (h = h1; h != h2 + direction; h += direction) {
            v = v1 + (int)(slope * (double)(h - h1) + 0.5);
            distanceCovered = (int)sqrt((double)(SQR(h - h1) + SQR(v - v1)));
            if (
				dash * gap == 0
				|| distanceCovered % (dash + gap) < dash
				|| (
					distance % (dash + gap) >= dash
					&& distanceCovered > distance - distance % (dash + gap)
				)
			) {
                filledEllipse(img, v, h, width, width, r, g, b, i);
            }
        }
    } else {
        slope = (double)(h2 - h1) / (double)(v2 - v1);
        direction = (v2 > v1) ? 1 : -1;
        for (v = v1; v != v2 + direction; v += direction) {
            h = h1 + (int)(slope * (double)(v - v1) + 0.5);
            distanceCovered = (int)sqrt((double)(SQR(h - h1) + SQR(v - v1)));
            if (
				dash * gap == 0
				|| distanceCovered % (dash + gap) < dash
				|| (
					distance % (dash + gap) >= dash
					&& distanceCovered > distance - distance % (dash + gap)
				)
			) {
                filledEllipse(img, v, h, width, width, r, g, b, i);
            }
        }
    }
}

// Draw rectangle in image img with opposite corners (v1, h1) and (v2, h2) with
// a given width, dash pattern, and color. Inputs are otherwise identical to the
// line function.
void rectangle(Image img, int v1, int h1, int v2, int h2, int width, int dash, int gap, int r, int g, int b, int i) {
    line(img, v1, h1, v1, h2, width, dash, gap, r, g, b, i);
    line(img, v1, h2, v2, h2, width, dash, gap, r, g, b, i);
    line(img, v2, h2, v2, h1, width, dash, gap, r, g, b, i);
    line(img, v2, h1, v1, h1, width, dash, gap, r, g, b, i);
}

// Draw ellipse in image img centered at (vCenter, hCenter) and radii (vRadius,
// hRadius) with a given width, dash pattern, and color. Width 0 indicates
// single-pixel width. The inputs dash and gap determine the length in pixels of
// the dashes and the gaps between them, resp. Use 0 for either input to draw a
// solid line. For setting the r, g, b, and i color values, see setPixel
// function.
void ellipse(Image img, int vCenter, int hCenter, int vRadius, int hRadius, int width, int dash, int gap, int r, int g, int b, int i) {
    int v, h, last_v = -100, last_h = -100, secondlast_v = -100, secondlast_h = -100, last_shown = 0, distanceCovered = 0;
    double alpha, stepsize = PI / 2.0 / (double)(vRadius + hRadius);

    for (alpha = 0.0; alpha < 2.0 * PI; alpha += stepsize) {
        v = vCenter + (int)((double)vRadius * sin(alpha));
        h = hCenter + (int)((double)hRadius * cos(alpha));
        if (v != last_v || h != last_h) {
            if (abs(v - secondlast_v) <= 1 && abs(h - secondlast_h) <= 1) {
                if (dash * gap == 0 || distanceCovered % (dash + gap) < dash) {
                    filledEllipse(img, v, h, width, width, r, g, b, i);
                }
                secondlast_v = -1;
                secondlast_h = -1;
                last_shown = 1;
                distanceCovered++;
            } else {
                if (!last_shown) {
                    if (
						(dash * gap == 0 || distanceCovered % (dash + gap) < dash)
						&& last_v > -100
					) {
                        filledEllipse(img, last_v, last_h, width, width, r, g, b, i);
                    }
                    distanceCovered++;
                }
                secondlast_v = last_v;
                secondlast_h = last_h;
                last_shown = 0;
            }
            last_v = v;
            last_h = h;
        }
    }
}

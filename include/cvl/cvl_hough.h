// Hough Transform.

#pragma once

#include <cvl/cvl_core.h>
#include <stddef.h>

// Defines a line in parameter space (rho, theta).
typedef struct cvl_hough_line_t {
    double rho;   // perpendicular distance from origin to line
    double theta; // angle of the normal (radians, [0, pi])
    double votes; // todo
} cvl_hough_line_t;

// Defines lines in polar form.
typedef struct cvl_hough_lines_t {
    cvl_hough_line_t *lines;
    size_t size;
} cvl_hough_lines_t;

// void cvl_hough_accumulate();

// void cvl_hough_peaks();

int cvl_hough_lines(const Matrix *edges, cvl_hough_lines_t *dst, double drho, double dtheta, int thresh);

cvl_hough_lines_t cvl_hough_lines_new(const Matrix *edges, double drho, double dtheta, int thresh);

void cvl_hough_lines_free(cvl_hough_lines_t *lines);

// int cvl_hough_circles(...);

// int cvl_hough_ellipses(...);

void cvl_draw_hough_lines(Image *img, const cvl_hough_lines_t *lines);

void cvl_draw_hough_line(Image *img, double rho, double theta, int r, int g, int b, int i);

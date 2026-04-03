#include <cvl/cvl_hough.h>
#include <cvl/cvl_draw.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Compares two Hough line's number of votes for qsort ordering.
static int _cmp(const void *a, const void *b) {
    double va = ((const cvl_hough_line_t *)a)->votes;
    double vb = ((const cvl_hough_line_t *)b)->votes;
    return (vb > va) - (vb < va);
}

int cvl_hough_lines(const Matrix *edges, cvl_hough_lines_t *dst, double drho, double dtheta, int thresh) {
    assert(edges && edges->map);

    const int h = edges->height;
    const int w = edges->width;

    double rho_max = sqrt((double)(h * h + w * w));

    const int nrho   = (int)ceil((2.0 * rho_max) / drho);
    const int ntheta = (int)ceil(M_PI / dtheta);

    Matrix acc = cvl_mat_create(nrho, ntheta);

    // Precompute sin/cos Tables.
    double *_cos = malloc(ntheta * sizeof(double)); // cos table
    double *_sin = malloc(ntheta * sizeof(double)); // sin table
    assert(_cos && _sin);

    for (int t = 0; t < ntheta; ++t) {
        double theta = t * dtheta;
        _cos[t] = cos(theta);
        _sin[t] = sin(theta);
    }

    // Voting.
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (edges->map[i][j] <= 0.0)
                continue;

            for (int t = 0; t < ntheta; ++t) {
                double rho = j * _cos[t] + i * _sin[t];
                int r = (int)((rho + rho_max) / drho);

                if (0 <= r && r < nrho) {
                    acc.map[r][t] += 1.0;
                }
            }
        }
    }

    // Find Peaks.
    size_t cap = nrho * ntheta;
    cvl_hough_line_t *candidates = malloc(cap * sizeof(cvl_hough_line_t));
    assert(candidates);

    size_t count = 0;

    for (int r = 1; r < nrho - 1; ++r) {
        for (int t = 1; t < ntheta - 1; ++t) {
            double v = acc.map[r][t];
            if (v < thresh) continue;

            // 4-neighborhood local max
            if (v >= acc.map[r - 1][t] && v >= acc.map[r + 1][t] &&
                v >= acc.map[r][t - 1] && v >= acc.map[r][t + 1]) {
                double rho = r * drho - rho_max;
                double theta = t * dtheta;

                candidates[count++] = (cvl_hough_line_t){
                    .rho = rho,
                    .theta = theta,
                    .votes = v,
                };
            }
        }
    }

    qsort(candidates, count, sizeof(cvl_hough_line_t), _cmp);

    size_t nlines = count;
    // size_t nlines = (count < max_lines) ? count : max_lines;

    dst->size = nlines;
    dst->lines = malloc(nlines * sizeof(cvl_hough_line_t));
    assert(dst->lines);

    memcpy(dst->lines, candidates, nlines * sizeof(cvl_hough_line_t));

    cvl_mat_free(acc);
    free(_cos);
    free(_sin);
    free(candidates);

    return (int)nlines;
}

cvl_hough_lines_t cvl_hough_lines_new(const Matrix *edges, double drho, double dtheta, int thresh) {
    cvl_hough_lines_t lines = {0};
    cvl_hough_lines(edges, &lines, drho, dtheta, thresh);
    return lines;
}

void cvl_hough_lines_free(cvl_hough_lines_t *lines) {
    free(lines->lines);
    lines->lines = NULL;
    lines->size = 0;
}

// Draws red lines over the given image using the given Hough lines.
void cvl_draw_hough_lines(Image *img, const cvl_hough_lines_t *lines) {
    for (size_t i = 0; i < lines->size; ++i) {
        cvl_hough_line_t line = lines->lines[i];

        // printf("\nline %zu\n", i);
        // printf("  rho: %f\n", line.rho);
        // printf("  theta: %f\n", line.theta);
        // printf("  votes: %f\n", line.votes);

        cvl_draw_hough_line(img, line.rho, line.theta, 255, 0, 0, 255);
    }
}

void cvl_draw_hough_line(Image *img, double rho, double theta, int r, int g, int b, int i) {
    // https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html

    double a_cos = cos(theta);
    double b_sin = sin(theta);

    double x0 = a_cos * rho;
    double y0 = b_sin * rho;

    // Direction vector perpendicular to normal
    double dx = -b_sin;
    double dy = a_cos;

    Point p1 = {
        (int)(x0 + 1000 * dx),
        (int)(y0 + 1000 * dy),
    };

    Point p2 = {
        .x = (int)(x0 - 1000 * dx),
        .y = (int)(y0 - 1000 * dy),
    };

    cvl_draw_line_solid(*img, p1, p2, r, g, b, i);
}

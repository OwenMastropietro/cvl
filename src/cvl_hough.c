#include <cvl/cvl_hough.h>
#include <cvl/cvl_draw.h>
#include <cvl/cvl_imgproc.h>
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

// Compares two Hough circle's number of votes for qsort ordering.
static int _cmp_circle_votes(const void *a, const void *b) {
    double va = ((const cvl_hough_circle_t *)a)->votes;
    double vb = ((const cvl_hough_circle_t *)b)->votes;
    return (vb > va) - (vb < va);
}

static int _lines_are_similar(cvl_hough_line_t a, cvl_hough_line_t b, double rho_thresh, double theta_thresh) {
    double drho = fabs(a.rho - b.rho);
    double dtheta = fabs(a.theta - b.theta);

    if(dtheta > M_PI) dtheta = 2.0 * M_PI - dtheta;
    dtheta = fmin(dtheta, M_PI - dtheta);

    return (drho < rho_thresh) && (dtheta < theta_thresh);
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
            if (edges->map[i][j] <= 0.0) continue;

            for (int t = 0; t < ntheta; ++t) {
                double rho = (double)j * _cos[t] + (double)i * _sin[t];
                int r = (int)round((rho + rho_max) / drho);

                if (0 <= r && r < nrho) {
                    acc.map[r][t] += 1.0;
                }
            }
        }
    }

    // Find Peaks.
    size_t cap = (size_t)nrho * ntheta / 4;
    cvl_hough_line_t *candidates = malloc(cap * sizeof(cvl_hough_line_t));
    assert(candidates);

    size_t count = 0;

    for (int r = 1; r < nrho - 1; ++r) {
        for (int t = 1; t < ntheta - 1; ++t) {
            double v = acc.map[r][t];
            if (v < thresh) continue;

            if ( // 8-neighborhood local max
                v >= acc.map[r - 1][t - 1] && v >= acc.map[r - 1][t] && v >= acc.map[r - 1][t + 1] &&
                v >= acc.map[r]    [t - 1] && v >= acc.map[r]    [t] && v >= acc.map[r]    [t + 1] &&
                v >= acc.map[r - 1][t - 1] && v >= acc.map[r + 1][t] && v >= acc.map[r + 1][t + 1]
            ) {
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

    double thresh_rho = drho * 2.0;
    double thresh_theta = dtheta * 2.0;

    int *suppressed = calloc(count, sizeof(int));
    cvl_hough_line_t *lines = malloc(count * sizeof(cvl_hough_line_t));
    size_t nlines = 0;

    for (size_t i = 0; i < count; ++i) {
        if (suppressed[i]) continue;
    
        lines[nlines++] = candidates[i];

        for (size_t j = i + 1; j < count; ++j) {
            if (!suppressed[j] && _lines_are_similar(candidates[i], candidates[j], thresh_rho, thresh_theta)) {
                suppressed[j] = 1;
            }
        }
    }

    dst->size = nlines;
    dst->lines = malloc(nlines * sizeof(cvl_hough_line_t));
    assert(dst->lines);

    memcpy(dst->lines, lines, nlines * sizeof(cvl_hough_line_t));

    cvl_mat_free(acc);
    free(_cos);
    free(_sin);
    free(candidates);
    free(lines);

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

/**
 * Detects circles in a grayscale image/matrix using the Hough Transform.
 * 
 * @param src Input grayscale image/matrix.
 * @param dst Output structure representing the detected circles.
 * @param dp Inverse ratio of the accumulator resolution to the image resolution
 * (i.e., 1 denotes same resolution, 2 denotes accumulator is half ther size of the input image, etc.).
 * @param min_dist Minimum distance between the centers of detected circles.
 * @param thresh_canny High threshold for Canny (low = 0.5 * high).
 * @param thresh_accumulator Minimum number of votes required to recognize a circle.
 * @param min_radius Minimum radius to consider (in piexls).
 * @param max_radius Maximum radius to consider (in piexls).
 */
int cvl_hough_circles(
    const Matrix *src,
    cvl_hough_circles_t *dst,
    double dp,
    double min_dist,
    double thresh_canny,
    double thresh_accumulator,
    int min_radius,
    int max_radius
) {
    if (min_radius > max_radius) return 0;
    if (dp <= 0) return 0;

    const int h = src->height;
    const int w = src->width;

    // Canny Edge Detection.
    Matrix edges = cvl_canny_new(src, 1, (int)(thresh_canny * 0.5), (int)thresh_canny);

    // DEBUG
    // Image edges_img = cvl_mat2img(edges, 0, 1.0);
    // cvl_imwrite("./data/modified/circle-edges.ppm", &edges_img);
    // DEBUG

    // Sobel Gradient.
    Matrix gx = cvl_mat_create(h, w);
    Matrix gy = cvl_mat_create(h, w);
    cvl_sobel(src, &gx, &gy);

    // Accumulator.
    const int ah = (int)ceil(h / dp);
    const int aw = (int)ceil(w / dp);
    const double scale = 1.0 / dp;
    Matrix accum = cvl_mat_create(ah, aw);

    // Center Voting.
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (edges.map[y][x] <= 0.0) continue;

            double dx = gx.map[y][x];
            double dy = gy.map[y][x];
            double mag = sqrt(dx*dx + dy*dy);
            if (mag < 1e-6) continue;

            dx /= mag;
            dy /= mag;

            for (int dir = -1; dir <= 1; dir += 2) {
                for (double r = min_radius; r <= max_radius; r += 1.0) {
                    int cx = (int)round((x + dir * dx * r) * scale);
                    int cy = (int)round((y + dir * dy * r) * scale);

                    if (0 <= cx && cx < aw && 0 <= cy && cy < ah) {
                        accum.map[cy][cx] += 1.0;
                    }
                }
            }
        }
    }

    // Find Candidate Centers.
    size_t cap = (size_t)ah * aw / 4;
    cvl_hough_circle_t *candidates = malloc(cap * sizeof(cvl_hough_circle_t));
    size_t count = 0;

    for (int cy = 1; cy < ah - 1; ++cy) {
        for (int cx = 1; cx < aw - 1; ++cx) {
            double v = accum.map[cy][cx];
            if (v < thresh_accumulator) continue;

            if ( // 3x3 local max
                v >= accum.map[cy - 1][cx - 1] && v >= accum.map[cy - 1][cx] && v >= accum.map[cy - 1][cx + 1] &&
                v >= accum.map[cy]    [cx - 1] && v >= accum.map[cy]    [cx] && v >= accum.map[cy]    [cx + 1] &&
                v >= accum.map[cy + 1][cx - 1] && v >= accum.map[cy + 1][cx] && v >= accum.map[cy + 1][cx + 1]
            ) {
                candidates[count++] = (cvl_hough_circle_t){
                    .x = cx / scale,
                    .y = cy / scale,
                    .radius = 0.0, // set below
                    .votes = v,
                };
            }
        }
    }

    qsort(candidates, count, sizeof(cvl_hough_circle_t), _cmp_circle_votes);

    // NMS on centers.
    int *suppressed = calloc(count, sizeof(int));
    cvl_hough_circle_t *circles = malloc(count * sizeof(cvl_hough_circle_t));
    size_t ncircles = 0;

    for (size_t i = 0; i < count; ++i) {
        if (suppressed[i]) continue;

        circles[ncircles++] = candidates[i];

        for (size_t j = i + 1; j < count; ++j) {
            if (suppressed[j]) continue;

            double dx = circles[ncircles - 1].x - candidates[j].x;
            double dy = circles[ncircles - 1].y - candidates[j].y;

            if (sqrt(dx*dx + dy*dy) < min_dist) {
                suppressed[j] = 1;
            }
        }
    }

    // Radius Estimation (via histogram).
    int nr = max_radius - min_radius + 1;

    for (size_t i = 0; i < ncircles; ++i) {
        int cx = (int)round(circles[i].x);
        int cy = (int)round(circles[i].y);

        if (cx < min_radius || cx >= w - min_radius || cy < min_radius || cy >= h - min_radius) {
            continue; // outside boarder... gtfo
        }

        int *hist = calloc(nr, sizeof(int));

        int x0 = fmax(0, cx - max_radius);
        int y0 = fmax(0, cy - max_radius);
        int x1 = fmin(w - 1, cx + max_radius);
        int y1 = fmin(h - 1, cy + max_radius);

        for (int y = y0; y <= y1; ++y) {
            for (int x = x0; x <= x1; ++x) {
                if (edges.map[y][x] <= 0.0) continue;

                // and this...
                double gx_val = gx.map[y][x];
                double gy_val = gy.map[y][x];
                double gmag = sqrt(gx_val*gx_val + gy_val*gy_val);
                if (gmag < 1e-6) continue;

                gx_val /= gmag;
                gy_val /= gmag;

                double rx = x - cx;
                double ry = y - cy;
                double rmag = sqrt(rx*rx + ry*ry);
                if (rmag < 1e-6) continue;

                rx /= rmag;
                ry /= rmag;

                double dot = gx_val * rx + gy_val * ry;

                if (fabs(dot) < 0.85) continue;

                int r = (int)round(rmag);

                if (min_radius <= r && r <= max_radius) {
                    hist[r - min_radius]++;
                }
            }
        }

        int max_r = min_radius;
        int max_votes = 0;
        for (int r = 0; r < nr; ++r) {
            if (hist[r] > max_votes) {
                max_votes = hist[r];
                max_r = r + min_radius;
            }
        }

        if (cx - max_r < 0 || w <= cx + max_r
            || cy - max_r < 0 || h <= cy + max_r
        ) {
            circles[i].radius = 0;
            continue;
        }

        if (max_votes < 0.15  * (2 * M_PI * max_r)) continue;
    
        circles[i].radius = max_r;
    
        free(hist);
    }

    size_t valid = 0;
    for (size_t i = 0; i < ncircles; ++i) {
        if (circles[i].radius > 0) {
            circles[valid++] = circles[i];
        }
    }
    ncircles = valid;

    // DEBUG
    // double max_vote = 0.0;
    // for (int y = 0; y < ah; ++y) {
    //     for (int x = 0; x < aw; ++x) {
    //         if (accum.map[y][x] > max_vote) {
    //             max_vote = accum.map[y][x];
    //         }
    //     }
    // }
    // printf("max vote: %f\n", max_vote);
    // DEBUG

    dst->size = ncircles;
    dst->circles = malloc(ncircles * sizeof(cvl_hough_circle_t));
    memcpy(dst->circles, circles, ncircles * sizeof(cvl_hough_circle_t));

    cvl_mat_free(edges);
    cvl_mat_free(gx);
    cvl_mat_free(gy);
    cvl_mat_free(accum);
    free(candidates);
    free(suppressed);
    free(circles);

    return (int)ncircles;
}

cvl_hough_circles_t cvl_hough_circles_new(
    const Matrix *src,
    double dp,
    double min_dist,
    double thresh_canny,
    double thresh_accumulator,
    int min_radius,
    int max_radius
) {
    cvl_hough_circles_t dst = {0};
    cvl_hough_circles(
        src,
        &dst,
        dp,
        min_dist,
        thresh_canny,
        thresh_accumulator,
        min_radius,
        max_radius
    );
    return dst;
}

void cvl_hough_circles_free(cvl_hough_circles_t *circles) {
    free(circles->circles);
    circles->circles = NULL;
    circles->size = 0;
}

void cvl_draw_hough_circles(Image *img, const cvl_hough_circles_t *circles) {
    for (size_t i = 0; i < circles->size; ++i) {
        cvl_hough_circle_t circle = circles->circles[i];

        Point center = {
            (int)round(circle.x),
            (int)round(circle.y),
        };
        Point radius = {
            (int)round(circle.radius),
            (int)round(circle.radius),
        };

        cvl_draw_ellipse(*img, center, radius, 0, 0, 0, 255, 0, 0, 255);
    }
}

// Example: Assignment checkpoints/deliverables for CS136.

#include <cvl/cvl.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Randomly flip binary pixels with probability p.
static void add_noise(cvl_Mat *img, double p) {
    uint8_t *data = img->data;

    for (int i = 0; i < img->height; ++i) {
        uint8_t *row = data + i * img->stride;

        for (int j = 0; j < img->width; ++j) {
            uint8_t pixel = row[j];
            assert(pixel == BLACK || pixel == WHITE);

            double r = (double)rand() / RAND_MAX;
            if (r < p) {
                row[j] = (pixel == BLACK) ? WHITE : BLACK;
            }
        }
    }
}


// Add and Remove Salt and Pepper Noise (via shrink-expand pipeline).
void p1_i(void) {
    // Load.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Add Noise.
    srand(42);
    cvl_Mat noisy = cvl_threshold_new(&img, 128, 255, CVL_THRESH_BINARY);
    add_noise(&noisy, 0.02);

    // Remove Noise.
    cvl_Mat clear = cvl_mat_copy(&img);
    cvl_Mat tmp = cvl_mat_copy(&img);
    cvl_open(&noisy, &tmp, 3);
    cvl_close(&tmp, &clear, 3);

    // Write.
    cvl_imwrite("./data/modified/1-text.pgm", &img);
    cvl_imwrite("./data/modified/2-text_noisy.pbm", &noisy);
    cvl_imwrite("./data/modified/3-text_clear.pbm", &clear);

    // Free.
    cvl_mat_free(&img);
    cvl_mat_free(&noisy);
    cvl_mat_free(&clear);
    cvl_mat_free(&tmp);
}

// CCL - Count Components.
void p1_ii(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Initialize Labels Container.
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_32S);

    // Label Components.
    int num_components = 0;
    num_components = cvl_connected_components(&img, &labels, 4);

    printf("\nNumber of Components: %d\n", num_components);

    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// CCL - Count & Color Components (according to a threshold).
void p1_iii(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Initialize Labels Container.
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_32S);

    // Label Components.
    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    // Color Labeled Components.
    cvl_Mat components = cvl_cvt_color_new(&img, CVL_COLOR_GRAY2RGB);
    num_components = cvl_color_components(&components, &labels, 100);
    printf("\nNumber of Components: %d\n", num_components);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.pgm", &img);
    cvl_imwrite("./data/modified/2-components.ppm", &components);

    // Cleanup.
    cvl_mat_free(&components);
    cvl_mat_free(&labels);
    cvl_mat_free(&img);
}

// Perform Mean & Median Blurring (via Convolution).
void p2(void) {
    // Load input image.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Apply Mean Blur.
    cvl_Mat mean_blur = cvl_blur_mean_new(&img, 3);

    // Apply Median Blur.
    cvl_Mat median_blur = cvl_blur_median_new(&img, 3);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-mean.ppm", &mean_blur);
    cvl_imwrite("./data/modified/3-median.ppm", &median_blur);

    // Cleanup.
    cvl_mat_free(&median_blur);
    cvl_mat_free(&mean_blur);
    cvl_mat_free(&img);
}

int main(void) {

    p1_i();
    // p1_ii();
    // p1_iii();

    // p2();

    return 0;
}

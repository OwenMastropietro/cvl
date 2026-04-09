// Example: Blurring / Smoothing Images.

#include <cvl/cvl.h>

int main(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Apply Mean Blur.
    cvl_Mat mean_blur = cvl_blur_mean_new(&img, 3);

    // Apply Gaussian Blur.
    cvl_Mat gauss_blur = cvl_blur_gauss_new(&img, 0.0, 1.0);

    // Apply Median Blur.
    cvl_Mat median_blur = cvl_blur_median_new(&img, 3);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-mean.ppm", &mean_blur);
    cvl_imwrite("./data/modified/3-gauss.ppm", &gauss_blur);
    cvl_imwrite("./data/modified/4-median.ppm", &median_blur);

    // Cleanup.
    cvl_mat_free(&median_blur);
    cvl_mat_free(&gauss_blur);
    cvl_mat_free(&mean_blur);
    cvl_mat_free(&img);

    return 0;
}

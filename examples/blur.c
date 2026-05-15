// Example: Blurring / Smoothing Images.

#include <cvl/cvl.h>

int main(void) {
    // Load.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Blur.
    cvl_Mat mean = cvl_blur_mean_new(&img, 3);
    cvl_Mat gauss = cvl_blur_gauss_new(&img, 0.0, 1.0);
    cvl_Mat median = cvl_blur_median_new(&img, 3);

    // Save.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-mean.ppm",     &mean);
    cvl_imwrite("./data/modified/3-gauss.ppm",    &gauss);
    cvl_imwrite("./data/modified/4-median.ppm",   &median);

    // Free.
    cvl_mat_free(&median);
    cvl_mat_free(&gauss);
    cvl_mat_free(&mean);
    cvl_mat_free(&img);

    return 0;
}

// Example: Thresholding.

#include <cvl/cvl.h>

int main(void) {
    // Load RGB Image.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Apply Truncating Threshold.
    cvl_Mat truncated = cvl_threshold_new(&img, 128, 255, CVL_THRESH_TRUNC);

    // Apply Binary Threshold.
    cvl_Mat binary = cvl_threshold_new(&img, 128, 255, CVL_THRESH_BINARY);

    // Apply Zero Threshold.
    cvl_Mat suppressed = cvl_threshold_new(&img, 128, 255, CVL_THRESH_TOZERO);

    // Note: most thresholds also have corresponding inverse.
    cvl_Mat binary_inv = cvl_threshold_new(&img, 128, 255, CVL_THRESH_BINARY_INV);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-truncation.ppm", &truncated);
    cvl_imwrite("./data/modified/3-binary.ppm", &binary);
    cvl_imwrite("./data/modified/4-suppressed.ppm", &suppressed);
    cvl_imwrite("./data/modified/5-binary_inv.ppm", &binary_inv);

    // Cleanup.
    cvl_mat_free(&binary_inv);
    cvl_mat_free(&suppressed);
    cvl_mat_free(&binary);
    cvl_mat_free(&truncated);
    cvl_mat_free(&img);

    return 0;
}

// Example: Color Conversion.

#include <cvl/cvl.h>

int main(void) {
    // Load RGB Image.
    cvl_Mat img = cvl_imread("./data/original/sample.ppm");

    // Convert RGB to Grayscale.
    cvl_Mat img_gray = cvl_cvt_color_new(&img, CVL_COLOR_RGB2GRAY);

    // Save RGB and Grayscale versions.
    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-grayscale.pgm", &img_gray);

    // Cleanup.
    cvl_mat_free(&img);
    cvl_mat_free(&img_gray);

    return 0;
}

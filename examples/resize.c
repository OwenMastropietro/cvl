// Example: Resizing Images.

#include <cvl/cvl.h>

int main(void) {
    // Load.
    cvl_Mat img = cvl_imread("./data/original/halo-1.jpg");

    // Resize.
    const int h = img.height;
    const int w = img.width;

    cvl_Mat shrink_near = cvl_resize_new(&img, h*0.8, w*0.8, CVL_INTER_NEAREST);
    cvl_Mat expand_near = cvl_resize_new(&img, h*1.2, w*1.2, CVL_INTER_NEAREST);

    cvl_Mat shrink_line = cvl_resize_new(&img, h*0.8, w*0.8, CVL_INTER_LINEAR);
    cvl_Mat expand_line = cvl_resize_new(&img, h*1.2, w*1.2, CVL_INTER_LINEAR);

    // Save.
    cvl_imwrite("./data/modified/1-original.png", &img);
    cvl_imwrite("./data/modified/2-shrink-near.png", &shrink_near);
    cvl_imwrite("./data/modified/3-shrink-line.png", &shrink_line);
    cvl_imwrite("./data/modified/4-expand-near.png", &expand_near);
    cvl_imwrite("./data/modified/4-expand-line.png", &expand_line);

    // Free.
    cvl_mat_free(&img);
    cvl_mat_free(&shrink_near);
    cvl_mat_free(&shrink_line);
    cvl_mat_free(&expand_near);
    cvl_mat_free(&expand_line);

    return 0;
}

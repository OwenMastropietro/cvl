// Example: Rotating Images.

#include <cvl/cvl.h>

int main(void) {
    // Load.
    cvl_Mat img = cvl_imread("./data/original/halo-1.jpg");

    // Rotate.
    cvl_Mat rot    = cvl_rotate_new(&img, 22.5, CVL_INTER_NEAREST); // affine
    cvl_Mat rot45  = cvl_rotate_bound_new(&img, 45, CVL_INTER_NEAREST); // affine
    cvl_Mat rot90  = cvl_rotate_new(&img, 90,   CVL_INTER_NEAREST); // orthogonal
    cvl_Mat rot180 = cvl_rotate_new(&img, 180,  CVL_INTER_LINEAR);  // orthogonal
    cvl_Mat rot225 = cvl_rotate_new(&img, 225,  CVL_INTER_LINEAR);  // affine

    // Save.
    cvl_imwrite("./data/modified/1-original.png", &img);
    cvl_imwrite("./data/modified/2-rot.png",      &rot);
    cvl_imwrite("./data/modified/3-rot45.png",    &rot45);
    cvl_imwrite("./data/modified/4-rot90.png",    &rot90);
    cvl_imwrite("./data/modified/5-rot180.png",   &rot180);
    cvl_imwrite("./data/modified/5-rot225.png",   &rot225);

    // Free.
    cvl_mat_free(&img);
    cvl_mat_free(&rot);
    cvl_mat_free(&rot45);
    cvl_mat_free(&rot90);
    cvl_mat_free(&rot180);
    cvl_mat_free(&rot225);

    return 0;
}

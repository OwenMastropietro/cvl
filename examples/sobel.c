// Example: Applying a Sobel Filter.

#include <cvl/cvl.h>

#include <math.h>

int main(void) {
    // Load Input Image.
    Image img = cvl_imread("./data/original/halo.ppm");
    Matrix mat = cvl_img2mat(img);
    Matrix blur = cvl_blur_gauss_new(&mat, 1);

    // Apply Sobel Filter (for directional gradients).
    Matrix gx = cvl_mat_create(mat.height, mat.width);
    Matrix gy = cvl_mat_create(mat.height, mat.width);
    cvl_sobel(&blur, &gx, &gy);

    // Apply Sobel Filter (for gradient magnitudes and angles).
    Matrix mags = cvl_sobel_mag(&blur);
    // Matrix angs = cvl_sobel_angle(&blur);

    // Save Results.
    Image gx_img = cvl_mat2img(gx, 0, 1);
    Image gy_img = cvl_mat2img(gy, 0, 1);
    Image mags_img = cvl_mat2img(mags, 0, 1);
    // cvl_binarize(&mags_img, 128);

    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-mags.pgm", &mags_img);
    cvl_imwrite("./data/modified/3-grads-horizontal.pgm", &gx_img);
    cvl_imwrite("./data/modified/4-grads-vertical.pgm", &gy_img);

    // Cleanup.
    cvl_img_free(mags_img);
    cvl_img_free(gy_img);
    cvl_img_free(gx_img);
    cvl_mat_free(mags);
    cvl_mat_free(gx);
    cvl_mat_free(gy);
    cvl_mat_free(blur);
    cvl_mat_free(mat);
    cvl_img_free(img);

    return 0;
}

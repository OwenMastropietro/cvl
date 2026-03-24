// Example: Applying a Sobel Filter.

#include <cvl/cvl.h>

#include <math.h>

int main(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    Image binary = cvl_binarize_new(&img, 128);

    Matrix lena = cvl_img2mat(binary);
    Matrix lena_smooth = cvl_blur_new(&lena, 3);

    Matrix gx = cvl_mat_create(lena.height, lena.width);
    Matrix gy = cvl_mat_create(lena.height, lena.width);
    cvl_sobel(&lena_smooth, &gx, &gy);

    // Compute G and θ from Sobel Gradients (Gx, Gy).
    Matrix mags = cvl_sobel_mag(&lena_smooth);
    Matrix angs = cvl_sobel_angle(&lena_smooth);

    // Convert back to Images.
    Image gx_img = cvl_mat2img(gx, 0, 1);
    Image gy_img = cvl_mat2img(gy, 0, 1);
    Image mags_img = cvl_mat2img(mags, 0, 1);
    Image angs_img = cvl_mat2img(angs, 0, 1);
    cvl_binarize(&angs_img, M_PI / 2);

    cvl_imwrite("./data/modified/original.ppm", &img);
    cvl_imwrite("./data/modified/binary.pbm", &binary);
    cvl_imwrite("./data/modified/mags.pgm", &mags_img);
    cvl_imwrite("./data/modified/angles.pgm", &angs_img);
    cvl_imwrite("./data/modified/grads-horizontal.pgm", &gx_img);
    cvl_imwrite("./data/modified/grads-vertical.pgm", &gy_img);

    cvl_img_free(angs_img);
    cvl_img_free(mags_img);
    cvl_img_free(gy_img);
    cvl_img_free(gx_img);
    cvl_mat_free(angs);
    cvl_mat_free(mags);
    cvl_mat_free(gx);
    cvl_mat_free(gy);
    cvl_mat_free(lena_smooth);
    cvl_mat_free(lena);
    cvl_img_free(binary);
    cvl_img_free(img);

    return 0;
}

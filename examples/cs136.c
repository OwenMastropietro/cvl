// Example: Assignment checkpoints/deliverables for CS136.

#include <cvl/cvl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Denoising via shrink-expand pipeline.
void p1_i(void) {
    Image img = cvl_imread("./data/original/text.pgm");

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/binary.pbm", &img);

    srand(42);
    cvl_add_noise(&img, 0.1);
    cvl_imwrite("./data/modified/noisy.pbm", &img);

    cvl_shrink(&img);
    cvl_expand(&img);
    cvl_expand(&img);
    cvl_shrink(&img);
    cvl_imwrite("./data/modified/cleared.pbm", &img);

    cvl_img_free(img);
}

// CCL - Count Connected Components.
void p1_ii(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    Matrix labels = cvl_mat_create(img.height, img.width);

    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("./data/modified/original.pgm", &img);

    cvl_mat_free(labels);
    cvl_img_free(img);
}

// CCL - Count & Color Components Components.
void p1_iii(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    Matrix labels = cvl_mat_create(img.height, img.width);

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/binary.pbm", &img);
    // todo: use threshold instead of assuming binary

    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    num_components = cvl_color_components(&img, &labels, 100);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("./data/modified/components.ppm", &img);

    cvl_mat_free(labels);
    cvl_img_free(img);
}

// Image Smoothing via Convolution.
void p2(void) {
    Image img = cvl_imread("./data/original/sample.ppm");

    Matrix src = cvl_img2mat(img);
    Image src_img = cvl_mat2img(src, 0, 1.0);

    Matrix mean = cvl_blur_mean_new(&src, 3);
    Image mean_img = cvl_mat2img(mean, 0, 1.0);

    Matrix median = cvl_blur_median_new(&src, 3);
    Image median_img = cvl_mat2img(median, 0, 1.0);

    cvl_imwrite("./data/modified/1-original.pgm", &src_img);
    cvl_imwrite("./data/modified/2-mean.pgm", &mean_img);
    cvl_imwrite("./data/modified/3-median.pgm", &median_img);

    // cleanup
    cvl_img_free(median_img);
    cvl_mat_free(median);
    cvl_img_free(mean_img);
    cvl_mat_free(mean);
    cvl_img_free(src_img);
    cvl_mat_free(src);
    cvl_img_free(img);
}

// Sobel Filtering.
void p3_i(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    Image binary = cvl_binarize_new(&img, 128);

    Matrix lena = cvl_img2mat(binary);
    Matrix lena_smooth = cvl_blur_gauss_new(&lena, 1);

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
}

// Canny Edge Detection.
void p3_ii(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    cvl_imwrite("./data/modified/lena.ppm", &img);

    cvl_binarize(&img, 128);
    cvl_imwrite("./data/modified/lena.pbm", &img);

    Matrix lena = cvl_img2mat(img);
    Matrix lena_edges = cvl_canny_new(&lena, -1, 50, 120);

    Image lena_edges_img = cvl_mat2img(lena_edges, 0, 1);
    cvl_imwrite("./data/modified/lena-canny.pgm", &lena_edges_img);

    cvl_img_free(lena_edges_img);
    cvl_mat_free(lena_edges);
    cvl_mat_free(lena);
    cvl_img_free(img);
}

int main(void) {

    p1_i();
    p1_ii();
    p1_iii();

    p2();

    p3_i();
    p3_ii();

    return 0;
}

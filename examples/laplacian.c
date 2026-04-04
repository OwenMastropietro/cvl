// Example: Laplacian Filter.

#include <cvl/cvl.h>

int main(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    Matrix mat = cvl_img2mat(img);

    double sigma = 1.0;
    Matrix smoothed = cvl_blur_gauss_new(&mat, sigma);

    Matrix edges = cvl_laplacian_new(&smoothed);

    Image res = cvl_mat2img(edges, 0, 0.5);

    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-laplacian.pgm", &res);

    cvl_img_free(res);
    cvl_mat_free(edges);
    cvl_mat_free(smoothed);
    cvl_mat_free(mat);
    cvl_img_free(img);

    return 0;
}
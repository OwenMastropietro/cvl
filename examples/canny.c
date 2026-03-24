// Example: Canny Edge Detection.

#include <cvl/cvl.h>

int main(void) {
    Image img = cvl_imread("./data/original/lena.ppm");
    Image binary = cvl_binarize_new(&img, 128);

    Matrix lena = cvl_img2mat(binary);

    const int sigma = 1;
    const int lo = 50;
    const int hi = 120;
    Matrix edges = cvl_canny_new(&lena, sigma, lo, hi);

    Image edges_img = cvl_mat2img(edges, 0, 1);

    cvl_imwrite("./data/modified/original.ppm", &img);
    cvl_imwrite("./data/modified/binary.pbm", &binary);
    cvl_imwrite("./data/modified/canny-edges.pgm", &edges_img);

    cvl_img_free(edges_img);
    cvl_mat_free(edges);
    cvl_mat_free(lena);
    cvl_img_free(binary);
    cvl_img_free(img);

    return 0;
}

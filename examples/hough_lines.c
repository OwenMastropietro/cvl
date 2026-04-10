// Example: Hough Line Detection.

#include <cvl/cvl.h>
#include <math.h>

int main(void) {
    Image img = cvl_imread("./data/original/lanes.ppm");
    // Image img = cvl_img_create_fill(200, 200, 0);
    // line(img, 0, 100, 199, 100, 2, 0, 0, 255, 255, 255, 255);

    Image binary = cvl_binarize_new(&img, 150);

    Matrix lena = cvl_img2mat(binary);

    // Canny Edges.
    const int sigma = 1;
    const int lo = 50;
    const int hi = 120;
    Matrix edges = cvl_canny_new(&lena, sigma, lo, hi);
    Image edges_img = cvl_mat2img(edges, 0, 1);

    // Hough Lines.
    double drho = 1.0;
    double dtheta = M_PI / 180.0;
    int thresh = 70;
    cvl_hough_lines_t lines = cvl_hough_lines_new(&edges, drho, dtheta, thresh);

    Image lines_img = cvl_img_copy(&edges_img);
    cvl_draw_hough_lines(&lines_img, &lines);

    cvl_imwrite("./data/modified/1-original.ppm", &img);
    cvl_imwrite("./data/modified/2-binary.pbm", &binary);
    cvl_imwrite("./data/modified/3-canny-edges.pgm", &edges_img);
    cvl_imwrite("./data/modified/4-hough-lines.ppm", &lines_img);

    // Cleanup.
    cvl_hough_lines_free(&lines);
    cvl_img_free(lines_img);
    cvl_img_free(edges_img);
    cvl_mat_free(edges);
    cvl_mat_free(lena);
    cvl_img_free(binary);
    cvl_img_free(img);

    return 0;
}

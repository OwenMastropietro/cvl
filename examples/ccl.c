// Example: Connected Component Labeling.

#include <cvl/cvl.h>

#include <stdio.h>

int main(void) {
    Image img = cvl_imread("./data/original/text.pgm");
    cvl_binarize(&img, 128);

    Matrix labels = cvl_mat_create(img.height, img.width);

    cvl_imwrite("original.pbm", &img);

    // Count connected components.
    int num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    // Count and color connected components.
    num_components = cvl_color_components(&img, &labels, 100);
    printf("\nNumber of Components: %d\n", num_components);

    cvl_imwrite("labeled-components.ppm", &img);

    cvl_mat_free(labels);
    cvl_img_free(img);

    return 0;
}

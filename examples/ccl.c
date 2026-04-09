// Example: Connected Component Labeling.

#include <cvl/cvl.h>
#include <stdio.h>
#include <assert.h>

int main(void) {
    // Load Input Image.
    cvl_Mat img = cvl_imread("./data/original/text.pgm");

    // Initialize Labels Container.
    cvl_Mat labels = cvl_mat_create(img.height, img.width, 1, CVL_32S);

    // Connected Component Labeling.
    int num_components = 0;

    // Label Components.
    num_components = cvl_connected_components(&img, &labels, 4);
    printf("\nNumber of Components: %d\n", num_components);

    // Color Labeled Components.
    cvl_Mat components = cvl_cvt_color_new(&img, CVL_COLOR_GRAY2RGB);
    num_components = cvl_color_components(&components, &labels, 100);
    printf("\nNumber of Components: %d\n", num_components);

    // Save Results.
    cvl_imwrite("./data/modified/1-original.pgm", &img);
    cvl_imwrite("./data/modified/2-components.ppm", &components);

    // Cleanup.
    cvl_mat_free(&components);
    cvl_mat_free(&labels);
    cvl_mat_free(&img);

    return 0;
}

// Example: Texture Segmentation.

#include <cvl/cvl.h>
#include <assert.h>

int main(void) {
    // Read Input.
    Image img = cvl_imread("./data/original/five-textures.ppm");
    assert(img.map);

    // Texture Segmentation.
    Image segmented = cvl_texture_segment_laws_kmeans(&img, 6, 15);
    cvl_imwrite("./data/modified/kmeans.ppm", &segmented);

    // Cleanup.
    cvl_img_free(img);
    cvl_img_free(segmented);

    return 0;
}

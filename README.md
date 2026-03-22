# CVL

_A Small **C**omputer **V**ision **L**ibrary in C._

[docs](https://owenmastropietro.github.io/projects/cvl/)

---

**CVL** implements a small computer vision library for raster images with support for reading and writing [Netpbm](https://netpbm.sourceforge.net/doc/ppm.html) image formats (PGM, PGM, PPM), along with thresholding, filtering, connected component labeling, and edge detection.

---

## Usage

```sh
make run  # run the example program
make test # run unit tests
```

## Example

> Canny Edge Detection on Lena.

```c
#include "cvl_imgproc.h"
#include "cvl_io.h"

int main(void) {
    Image lena_img = cvl_imread("./data/original/lena.ppm");
    cvl_threshold(&lena_img, 128);
    
    Matrix lena = cvl_img2mat(lena_img); // 8U --> 64F
    Matrix edges = cvl_canny_new(&lena, 1.0, 50, 120);

    Image edges_img = cvl_mat2img(lena_edges, 0, 1);
    cvl_imwrite("./data/modified/lena-canny.pgm", &lena_edges_img);

    cvl_img_free(edges_img);
    cvl_mat_free(edges);
    cvl_mat_free(lena);
    cvl_img_free(lena_img);

    return 0;
}
```

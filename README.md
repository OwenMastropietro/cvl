# <img src="./assets/cvl-logo.png" alt="logo" width="50" style="vertical-align: middle; margin-right: 10px;"> CVL

_A Small Computer Vision Library in C._

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
    Image img = cvl_imread("lena.ppm");
    Image binary = cvl_binarize_new(&img, 128);

    Matrix lena = cvl_img2mat(binary);

    const int sigma = 1;
    const int lo = 50;
    const int hi = 120;
    Matrix edges = cvl_canny_new(&lena, sigma, lo, hi);

    Image edges_img = cvl_mat2img(edges, 0, 1);

    cvl_imwrite("original.ppm", &img);
    cvl_imwrite("binary.pbm", &binary);
    cvl_imwrite("canny.pgm", &edges_img);

    // free memory...

    return 0;
}
```

|          Original          |             Binary Threshold             |              Canny Edges               |
| :------------------------: | :--------------------------------------: | :------------------------------------: |
| ![lena](./assets/lena.png) | ![lena-binary](./assets/lena-binary.png) | ![lena-edges](./assets/lena-canny.png) |

> see the [docs](https://owenmastropietro.github.io/projects/cvl/) for more examples and documentation.

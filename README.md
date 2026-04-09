# <img src="./assets/cvl-logo.png" alt="logo" width="50" style="vertical-align: middle; margin-right: 10px;"> CVL

_Computer Vision Library in C._

[docs](https://owenmastropietro.github.io/projects/cvl/)

---

**CVL** implements a small computer vision library for raster images with support for reading and writing [Netpbm](https://netpbm.sourceforge.net/doc/ppm.html) image formats (PGM, PGM, PPM), along with thresholding, filtering, connected component labeling, and edge detection.

---

## Usage

> Build and run example programs.

```sh
# Manual Compile & Execute.
gcc examples/<example>.c src/* -Iinclude -o <example>
./<example>

# With CMake.
cmake -B build -S .
cmake --build build

./build/<example>
```

> Build and run tests.

```sh
cmake -B build -S . -DBUILD_TESTS=ON
cmake --build build

ctest --test-dir build
```

## Example Programs

> See the [docs](https://owenmastropietro.github.io/projects/cvl/) for more examples and documentation.

| Executable      | Source File                                      | Description            |
| --------------- | ------------------------------------------------ | ---------------------- |
| `cvl_color`     | [`examples/color.c`](./examples/color.c)         | Color Conversion       |
| `cvl_threshold` | [`examples/threshold.c`](./examples/threshold.c) | Thresholding           |
| `cvl_blur`      | [`examples/blur.c`](./examples/blur.c)           | Blurring / Smoothing   |
| `cvl_cs136`     | [`examples/cs136.c`](./examples/cs136.c)         | Assignments from CS136 |

---

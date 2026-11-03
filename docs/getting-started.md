# Getting Started

This guide covers building CVL, running its tests and examples, and using the
library in a small C program.

## Build

CVL can be built from source using CMake:

```sh
git clone https://github.com/OwenMastropietro/cvl.git
cd cvl

cmake -S . -B build
cmake --build build
```

This builds the static library and the `cvl` command-line tool.

## Tests

To build and run tests:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

> Note: GoogleTest is fetched during CMake configuration, so building the tests
> requires network access.

## Examples

CVL includes example programs demonstrating common image-processing operations.

To build the example programs:

```sh
cmake -S . -B build -DBUILD_EXAMPLES=ON
cmake --build build
```

Example executables are built in `build/examples/`.

Run an example:

```sh
./build/examples/cvl_blur
```

See [Examples](examples.md) for an overview of the available demonstrations.

## Install

To install CVL:

```sh
cmake --install build --prefix /usr/local
```

This installs the library, public headers, CMake package configuration, and CLI if it was built.
Consumers can then use `find_package(cvl)` and link against `cvl::cvl_lib`.

## A minimal program

```c
#include <cvl/cvl.h>

int main(void) {
    cvl_Mat img = cvl_imread("input.ppm");
    if (!img.data) {
        return 1;
    }

    cvl_Mat gray = cvl_cvt_color_new(&img, CVL_COLOR_RGB2GRAY);
    cvl_Mat binary = cvl_threshold_new(&gray, 128, 255, CVL_THRESH_BINARY);
    cvl_imwrite("binary.pgm", &binary);

    cvl_mat_free(&binary);
    cvl_mat_free(&gray);
    cvl_mat_free(&img);
    return 0;
}
```

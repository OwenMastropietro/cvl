# cvl

[![Linux](https://github.com/OwenMastropietro/cvl/actions/workflows/linux.yml/badge.svg)](https://github.com/OwenMastropietro/cvl/actions/workflows/linux.yml)
[![macOS](https://github.com/OwenMastropietro/cvl/actions/workflows/macos.yml/badge.svg)](https://github.com/OwenMastropietro/cvl/actions/workflows/macos.yml)
[![Windows](https://github.com/OwenMastropietro/cvl/actions/workflows/windows.yml/badge.svg)](https://github.com/OwenMastropietro/cvl/actions/workflows/windows.yml)

An extremely small Computer Vision Library, written in C.

<!-- CVL uses a unified matrix representation, `cvl_Mat`, for image data. It supports common image formats and provides operations for color conversion, thresholding, filtering, morphology, connected component analysis, edge detection, and segmentation. -->

## Features

- **Image representation** &mdash; A unified type for grayscale and color image data.
- **Image I/O** &mdash; Read and write supported image formats.
- **Image Processing** &mdash; Color conversion, thresholding, filtering, morphological operations.
- **Analysis** &mdash; Connected component labeling, Sobel gradient magnitude.
- **Command-line interface** &mdash; A comanion `cvl` executable for working with images.

## Getting Started

Build CVL from source using CMake, or integrate it into an existing C project.

[Getting Started](./getting-started.md)

## Examples

Explore image-processing pipelines and see the results of applying CVL's operations to sample images.

[Browse Examples](./examples.md)

## API Reference

[Peruse the API](./api/index.md)

<!-- ## CLI Reference -->

<!-- [Peruse the CLI](todo) -->

<!-- ## Project structure

| Path           | Description                                          |
| -------------- | ---------------------------------------------------- |
| `include/cvl/` | Public headers. Include `cvl/cvl.h`.                 |
| `src/`         | Library implementation.                              |
| `cli/`         | `cvl` command-line tool, built by default.           |
| `examples/`    | Example programs. Enable with `-DBUILD_EXAMPLES=ON`. |
| `tests/`       | GoogleTest suite, Enable with `-DBUILD_TESTS=ON`.    |

## Supported formats

<div class="grid cards" markdown>

- :material-file-png-box: **PNG** Supported via [stb](https://github.com/nothings/stb).
- :material-file-jpg-box: **JPG** Supported via [stb](https://github.com/nothings/stb).
- :material-file-image: **PBM** Portable bitmap ([Netpbm](https://netpbm.sourceforge.net/)).
- :material-file-image: **PGM** Portable graymap ([Netpbm](https://netpbm.sourceforge.net/)).
- :material-file-image: **PPM** Portable pixmap ([Netpbm](https://netpbm.sourceforge.net/)).

</div> -->

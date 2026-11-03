# Core

File reference for [`core.h`](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h)
 / 
[`core.c`](https://github.com/OwenMastropietro/cvl/blob/main/src/core.c).

Core types, constants, and matrix operations.

## Headers
---
`#include <stddef.h>`  
`#include <stdint.h>`  
`#include <stdio.h>`  
`#include <stdlib.h>`  

## Types
---
<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_Mat

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L39)]

</div>

```c
typedef struct {
    int    height;
    int    width;
    int    channels;
    int    depth;
    size_t stride;
    void * data;
} cvl_Mat;
```

A unified matrix type for representing images, kernels, or other arbitrary matrix-like structures.

Rows are stored consecutively, with channels belonging to the same element stored next to one another. The address of row <code>r</code> is: <code>(uint8_t *)mat-&gt;data + r * mat-&gt;stride</code>.

The same type represents both images and other matrix data, such as floating-point kernels. For example, an 8-bit image and a 64-bit kernel are both <a href="#cvl_Mat"><code>cvl_Mat</code></a>, distinguished by <code>depth</code> and <code>channels</code>.


**Members**

<div class="param-list">
  <div class="param">
    <strong>height</strong>
    <span>Number of rows.</span>
  </div>

  <div class="param">
    <strong>width</strong>
    <span>Number of columns.</span>
  </div>

  <div class="param">
    <strong>channels</strong>
    <span>Number of channels per element. For example, an RGB image has three channels.</span>
  </div>

  <div class="param">
    <strong>depth</strong>
    <span>Data type used to represent each channel. See <a href="#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

  <div class="param">
    <strong>stride</strong>
    <span>Number of bytes from one row to the next.</span>
  </div>

  <div class="param">
    <strong>data</strong>
    <span>Pointer to the first byte of matrix storage.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_format_t

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L62)]

</div>

```c
typedef enum cvl_format_t {
    CVL_FMT_UNKNOWN,
    CVL_FMT_PBM,
    CVL_FMT_PGM,
    CVL_FMT_PPM,
    CVL_FMT_PNG,
    CVL_FMT_JPG,
} cvl_format_t;
```

Enumeration of supported image formats.

| Value | Extension | Description |
| --- | --- | --- |
| CVL_FMT_UNKNOWN | - | Unknown or unsupported image format. |
| CVL_FMT_PBM | .pbm | Netpbm Portable Bitmap format for binary images. |
| CVL_FMT_PGM | .pgm | Netpbm Portable Graymap format for grayscale images. |
| CVL_FMT_PPM | .ppm | Netpbm Portable Pixmap format for colored images. |
| CVL_FMT_PNG | .png | Portable Network Graphics, supported via stb. |
| CVL_FMT_JPG | .jpg | Joint Photographic Experts Group, supported via stb. <code>JPEG</code> is a redundant verbocity that will not be condoned. |

The PBM, PGM, and PPM formats are supported natively.


</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_depth_t

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L76)]

</div>

```c
typedef enum cvl_depth_t {
    CVL_UINT8,
    CVL_32S,
    CVL_FLOAT32,
    CVL_FLOAT64,
} cvl_depth_t;
```

Enumeration of supported data types used to represent each matrix channel.

See <a href="../image-processing#cvl_cvt_depth"><code>cvl_cvt_depth()</code></a> for converting between supported depths.


| Value | Description |
| --- | --- |
| `CVL_UINT8` | 8-bit unsigned integer (todo: rename: CVL_8U) |
| `CVL_32S` | 32-bit signed integer |
| `CVL_FLOAT32` | 32-bit floating point |
| `CVL_FLOAT64` | 64-bit floating point |

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_color_conversion

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L96)]

</div>

```c
typedef enum cvl_color_conversion {
    CVL_COLOR_NONE,
    CVL_COLOR_RGB2GRAY,
    CVL_COLOR_BGR2GRAY,
    CVL_COLOR_GRAY2RGB,
    CVL_COLOR_GRAY2BGR,
} cvl_color_conversion;
```

Enumeration of supported color-space conversions.

See <a href="../image-processing#cvl_cvt_color"><code>cvl_cvt_color()</code></a> for converting between supported color spaces.

| Value | Source Channels | Destination Channels | Description |
| --- | --- | --- | --- |
| CVL_COLOR_NONE | — | — | No color conversion. |
| CVL_COLOR_RGB2GRAY | 3 | 1 | Converts RGB color to grayscale. |
| CVL_COLOR_BGR2GRAY | 3 | 1 | Converts BGR color to grayscale. |
| CVL_COLOR_GRAY2RGB | 1 | 3 | Copies the grayscale value into each RGB channel. |
| CVL_COLOR_GRAY2BGR | 1 | 3 | Copies the grayscale value into each BGR channel. |


</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_thresh_type

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L117)]

</div>

```c
typedef enum cvl_thresh_type {
    CVL_THRESH_BINARY,
    CVL_THRESH_BINARY_INV,
    CVL_THRESH_TRUNC,
    CVL_THRESH_TOZERO,
    CVL_THRESH_TOZERO_INV,
} cvl_thresh_type;
```

Enumeration of supported thresholding operations.

See <a href="../image-processing#cvl_threshold"><code>cvl_threshold()</code></a> for applying thresholds.

| Mode | Operation |
| --- | --- |
| CVL_THRESH_BINARY | <code>src &gt; thresh ? maxval : 0</code> |
| CVL_THRESH_BINARY_INV | <code>src &gt; thresh ? 0 : maxval</code> |
| CVL_THRESH_TRUNC | <code>src &gt; thresh ? thresh : src</code> |
| CVL_THRESH_TOZERO | <code>src &gt; thresh ? src : 0</code> |
| CVL_THRESH_TOZERO_INV | <code>src &gt; thresh ? 0 : src</code> |


</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_interp_t

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L128)]

</div>

```c
typedef enum cvl_interp_t {
    CVL_INTER_NEAREST,
    CVL_INTER_LINEAR,
} cvl_interp_t;
```

Enumeration of supported interpolation methods.


| Value | Description |
| --- | --- |
| `CVL_INTER_NEAREST` | Nearest-neighbor interpolation. |
| `CVL_INTER_LINEAR` | Bilinear interpolation. |

</div>

## Functions
---
<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_elem_size()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L143)]

</div>

```c
size_t cvl_elem_size( cvl_depth_t     depth );
```

Returns the size, in bytes, of a matrix data type.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>depth</strong>
    <span>Data type. See <a href="#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

</div>

**Returns**: Size of one channel element in bytes.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_empty()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L152)]

</div>

```c
cvl_Mat cvl_mat_empty( void             );
```

Creates an empty matrix.

Equivalent to calling the designated initializer.

**Returns**: An empty <a href="#cvl_Mat"><code>cvl_Mat</code></a> with no allocated matrix storage.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_create()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L163)]

</div>

```c
cvl_Mat cvl_mat_create( int             height,
                        int             width,
                        int             channels,
                        cvl_depth_t     depth    );
```

Creates a zero-initialized matrix.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>height</strong>
    <span>Number of rows.</span>
  </div>

  <div class="param">
    <strong>width</strong>
    <span>Number of columns.</span>
  </div>

  <div class="param">
    <strong>channels</strong>
    <span>Number of channels per element.</span>
  </div>

  <div class="param">
    <strong>depth</strong>
    <span>Data type used to represent each channel. See <a href="#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

</div>

**Returns**: The newly allocated matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_create_fill()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L177)]

</div>

```c
cvl_Mat cvl_mat_create_fill( int             height,
                             int             width,
                             int             channels,
                             cvl_depth_t     depth,
                             const void *    fill_value );
```

Creates a matrix initialized with a fill value.

Initializes every matrix channel using the supplied fill value.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>height</strong>
    <span>Number of rows.</span>
  </div>

  <div class="param">
    <strong>width</strong>
    <span>Number of columns.</span>
  </div>

  <div class="param">
    <strong>channels</strong>
    <span>Number of channels per element.</span>
  </div>

  <div class="param">
    <strong>depth</strong>
    <span>Data type used to represent each channel. See <a href="#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

  <div class="param">
    <strong>fill_value</strong>
    <span>Pointer to the value used to initialize each channel.</span>
  </div>

</div>

**Returns**: The newly allocated matrix, or an empty matrix on failure.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_create_from()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L193)]

</div>

```c
cvl_Mat cvl_mat_create_from( int             height,
                             int             width,
                             int             channels,
                             cvl_depth_t     depth,
                             const void *    data     );
```

Creates a matrix initialized from existing data.

Initializes the matrix using the supplied data buffer.

!!! warning "todo"

    Fold into <a href="#cvl_mat_create_fill"><code>cvl_mat_create_fill()</code></a>.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>height</strong>
    <span>Number of rows.</span>
  </div>

  <div class="param">
    <strong>width</strong>
    <span>Number of columns.</span>
  </div>

  <div class="param">
    <strong>channels</strong>
    <span>Number of channels per element.</span>
  </div>

  <div class="param">
    <strong>depth</strong>
    <span>Data type used to represent each channel. See <a href="#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

  <div class="param">
    <strong>data</strong>
    <span>Pointer to the source data.</span>
  </div>

</div>

**Returns**: The newly allocated matrix, or an empty matrix on failure.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_copy()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L201)]

</div>

```c
cvl_Mat cvl_mat_copy( const cvl_Mat * src );
```

Creates a copy of an existing matrix.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Pointer to the source matrix.</span>
  </div>

</div>

**Returns**: The copied matrix, or an empty matrix on failure.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_free()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L210)]

</div>

```c
void cvl_mat_free( cvl_Mat *       mat );
```

Releases the storage associated with a matrix.

Frees the matrix&#x27;s allocated data and resets its state.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>mat</strong>
    <span>Pointer to the matrix to release.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_row()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L224)]

</div>

```c
uint8_t * cvl_mat_row( cvl_Mat *       mat,
                       int             r   );
```

Returns a pointer to the beginning of a matrix row.

The returned pointer addresses the first byte of row <code>r</code>. Use the matrix&#x27;s <code>stride</code> to determine the distance between rows.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>mat</strong>
    <span>Pointer to the matrix.</span>
  </div>

  <div class="param">
    <strong>r</strong>
    <span>Row index.</span>
  </div>

</div>

**Returns**: Pointer to the first byte of the requested row.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_mat_row_const()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/core.h#L236)]

</div>

```c
const uint8_t * cvl_mat_row_const( const cvl_Mat * mat,
                                   int             r   );
```

Returns a constant pointer to the beginning of a matrix row.

The returned pointer addresses the first byte of row <code>r</code>. Use the matrix&#x27;s <code>stride</code> to determine the distance between rows.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>mat</strong>
    <span>Pointer to the matrix.</span>
  </div>

  <div class="param">
    <strong>r</strong>
    <span>Row index.</span>
  </div>

</div>

**Returns**: Constant pointer to the first byte of the requested row.

</div>

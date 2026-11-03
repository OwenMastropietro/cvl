# Image Processing

File reference for [`imgproc.h`](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h)
 / 
[`imgproc.c`](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c).

Image processing operations.

Provides functions for common image processing tasks, including:

- **Color and type conversion:** color-space conversion, depth conversion, thresholding.
- **Geomatric Transformations:** rotation, cropping, resizing.
- **Morphological Transformations:** dilation, erosion, opening, closing.
- **Connected Component Analysis:** labeling and visualization.
- **Filtering and Convolution:** correlation, convolution, box filtering, Gaussian blur, median blur.
- **Edge Detection:** Sobel derivatives and gradient magnitudes.

## Headers
---
[`#include <cvl/core.h>`](core.md)  
`#include <stdbool.h>`  

## Functions
---
<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_color()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L35)]

</div>

```c
void cvl_cvt_color( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    int             code );
```

Converts a matrix to another color space.

Grayscale conversion uses the ITU-R BT.601 luminance coefficients.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have same size as <code>src</code> and the channel count required by <code>code</code>.</span>
  </div>

  <div class="param">
    <strong>code</strong>
    <span>Color conversion code. See <a href="../core#cvl_color_conversion"><code>cvl_color_conversion</code></a>.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_depth()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L49)]

</div>

```c
void cvl_cvt_depth( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    cvl_depth_t     ddepth,
                    double          alpha,
                    double          beta   );
```

Converts a matrix to another type with scaling.

Effectively: <code>dst(x, y) = saturate_cast&lt;ddepth&gt;(alpha * src(x, y) + beta)</code>

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Output matrix. Must have the same dimensions and number of channels as <code>src</code>.</span>
  </div>

  <div class="param">
    <strong>ddepth</strong>
    <span>Destination depth - type to convert to. See <a href="../core#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

  <div class="param">
    <strong>alpha</strong>
    <span>Multiplicative scale factor (1.0 for no change).</span>
  </div>

  <div class="param">
    <strong>beta</strong>
    <span>Additive offset (0 for no change).</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_threshold()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L59)]

</div>

```c
void cvl_threshold( cvl_Mat *       img,
                    int             thresh,
                    int             maxval,
                    int             type   );
```

Applies a fixed-level threshold to each element.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>img</strong>
    <span>Matrix to modify in place.</span>
  </div>

  <div class="param">
    <strong>thresh</strong>
    <span>Threshold value.</span>
  </div>

  <div class="param">
    <strong>maxval</strong>
    <span>Maximum output value used by binary thresholding.</span>
  </div>

  <div class="param">
    <strong>type</strong>
    <span>Thresholding operation. See <a href="../core#cvl_thresh_type"><code>cvl_thresh_type</code></a>.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L78)]

</div>

```c
void cvl_rotate( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 float           angle,
                 cvl_interp_t    interp );
```

Rotates a matrix clockwise around its center.

The dimensions of <code>dst</code> determine the output region. Source coordinates outside the input matrix are left unchanged in the destination matrix.

Orthogonal rotations use direct pixel rearrangement. Other angles use the specified interpolation method.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix.</span>
  </div>

  <div class="param">
    <strong>angle</strong>
    <span>Clockwise rotation angle, in degrees.</span>
  </div>

  <div class="param">
    <strong>interp</strong>
    <span>Interpolation method. See <a href="../core#cvl_interp_t"><code>cvl_interp_t</code></a>.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_invert()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L89)]

</div>

```c
void cvl_invert( cvl_Mat *       img,
                 double          maxval );
```

Inverts matrix values relative to a specified maximum.

Each element&#x27;s <code>value</code> becomes <code>maxval - value</code>. Values are processed independently for each channel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>img</strong>
    <span>Matrix to modify in place.</span>
  </div>

  <div class="param">
    <strong>maxval</strong>
    <span>Maximum value used for inversion.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_dilate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L103)]

</div>

```c
void cvl_dilate( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 int             ksize );
```

Applies morphological dilation to a matrix.

Replaces each element with the maximum value within its square neighborhood.

Border pixels are handled by replicating the nearest edge pixel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square structuring element.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_erode()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L117)]

</div>

```c
void cvl_erode( const cvl_Mat * src,
                cvl_Mat *       dst,
                int             ksize );
```

Applies morphological erosion to a matrix.

Replaces each element with the minimum value within its square neighborhood.

Border pixels are handled by replicating the nearest edge pixel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square structuring element.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_open()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L131)]

</div>

```c
void cvl_open( const cvl_Mat * src,
               cvl_Mat *       dst,
               int             ksize );
```

Applies morphological opening to a matrix.

Performs erosion followed by dilation using the same square structuring element.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square structuring element.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_close()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L145)]

</div>

```c
void cvl_close( const cvl_Mat * src,
                cvl_Mat *       dst,
                int             ksize );
```

Applies morphological closing to a matrix.

Performs dilation followed by erosion using the same square structuring element.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square structuring element.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_crop()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L158)]

</div>

```c
void cvl_crop( const cvl_Mat * src,
               cvl_Mat *       dst,
               int             r,
               int             c   );
```

Copies a rectangular region of interest into a destination matrix.

The region begins at (r, c) in the source matrix and has the dimensions of dst.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix defining the output dimensions.</span>
  </div>

  <div class="param">
    <strong>r</strong>
    <span>Starting row in the source matrix.</span>
  </div>

  <div class="param">
    <strong>c</strong>
    <span>Starting column in the source matrix.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_resize()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L170)]

</div>

```c
void cvl_resize( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 cvl_interp_t    type );
```

Resizes a matrix using the specified interpolation method.

The destination matrix determines the output dimensions. Interpolation is performed independently for each channel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix defining the output dimensions.</span>
  </div>

  <div class="param">
    <strong>type</strong>
    <span>Interpolation method. See <a href="../core#cvl_interp_t"><code>cvl_interp_t</code></a>.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_connected_components()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L189)]

</div>

```c
int cvl_connected_components( const cvl_Mat * src,
                              cvl_Mat *       labels,
                              int             connectivity );
```

Labels connected foreground components in a binary image.

WHITE pixels are treated as background; all other pixels are treated as foreground.

Labels are assigned positive integer values, with zero reserved for background pixels.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Single-channel CVL_UINT8 binary source matrix.</span>
  </div>

  <div class="param">
    <strong>labels</strong>
    <span>Single-channel CVL_32S destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>connectivity</strong>
    <span>Pixel connectivity: 4 or 8.</span>
  </div>

</div>

**Returns**: Number of connected foreground components.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_color_components()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L205)]

</div>

```c
int cvl_color_components( cvl_Mat *       src,
                          const cvl_Mat * labels,
                          int             thresh );
```

Colors connected components that meet a minimum size.

Calculates the size of each labeled component and assigns a unique color to components containing at least thresh pixels. Components smaller than the threshold are unchanged.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Three-channel CVL_UINT8 image to modify.</span>
  </div>

  <div class="param">
    <strong>labels</strong>
    <span>Single-channel CVL_32S component label matrix.</span>
  </div>

  <div class="param">
    <strong>thresh</strong>
    <span>Minimum component size, in pixels.</span>
  </div>

</div>

**Returns**: Number of components meeting the minimum size.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_correlate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L222)]

</div>

```c
void cvl_correlate( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    cvl_Mat *       kernel );
```

Applies a two-dimensional correlation operation.

The kernel is not flipped. Values outside the source matrix are treated as zero (constant-zero padding).

The output has the same dimensions, depth, and number of channels as the source matrix.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix.</span>
  </div>

  <div class="param">
    <strong>kernel</strong>
    <span>Single-channel CVL_FLOAT64 correlation kernel.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L239)]

</div>

```c
void cvl_convolve( const cvl_Mat * src,
                   cvl_Mat *       dst,
                   cvl_Mat *       kernel );
```

Applies a two-dimensional convolution operation.

Convolves the source matrix with the specified kernel. 
 Convolution flips the kernel by 180-degrees and then applies correlation.

Values outside the source matrix are treated as zero (constant-zero padding).

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix.</span>
  </div>

  <div class="param">
    <strong>kernel</strong>
    <span>Convolution kernel.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve_sep()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L256)]

</div>

```c
void cvl_convolve_sep( const cvl_Mat * src,
                       cvl_Mat *       dst,
                       cvl_Mat *       kx,
                       cvl_Mat *       ky  );
```

Applies a separable two-dimensional convolution.

First convolves each row using the horizontal kernel, then convolves each column of the intermediate result using the vertical kernel.

This operation is useful when a two-dimensional filter can be represented as the product of two one-dimensional kernels.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix.</span>
  </div>

  <div class="param">
    <strong>kx</strong>
    <span>Horizontal one-dimensional kernel.</span>
  </div>

  <div class="param">
    <strong>ky</strong>
    <span>Vertical one-dimensional kernel.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_box()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L272)]

</div>

```c
void cvl_blur_box( const cvl_Mat * src,
                   cvl_Mat *       dst,
                   int             ksize,
                   bool            normalize );
```

Applies box filtering using a uniform square kernel.

Each output element is calculated from the sum of its neighborhood values. If normalize is true, the kernel weights are scaled by 1/(ksize * ksize), producing a mean filter.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square kernel.</span>
  </div>

  <div class="param">
    <strong>normalize</strong>
    <span>Whether to normalize the kernel weights.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_mean()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L286)]

</div>

```c
void cvl_blur_mean( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    int             ksize );
```

Applies a normalized box filter (mean blur).

Each output element is the average of the values within its square neighborhood.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square kernel.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_gauss()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L304)]

</div>

```c
void cvl_blur_gauss( const cvl_Mat * src,
                     cvl_Mat *       dst,
                     int             ksize,
                     double          sigma );
```

Applies Gaussian blur to a matrix.

If ksize is zero or negative, the kernel size is derived from sigma. If sigma is zero or negative, the standard deviation is derived from ksize.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix containing the blurred result.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Gaussian kernel width and height. Must be positive when sigma is not specified.</span>
  </div>

  <div class="param">
    <strong>sigma</strong>
    <span>Standard deviation of the Gaussian distribution. Must be positive when ksize is not specified.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_median()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L318)]

</div>

```c
void cvl_blur_median( const cvl_Mat * src,
                      cvl_Mat *       dst,
                      int             ksize );
```

Applies median filtering to a matrix.

Replaces each element with the median value of its square neighborhood. Border pixels are handled by replicating the nearest edge pixel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Destination matrix. Must have the same dimensions as src.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square neighborhood, must be odd.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L338)]

</div>

```c
void cvl_sobel( const cvl_Mat * src,
                cvl_Mat *       gx,
                cvl_Mat *       gy  );
```

Computes horizontal and vertical Sobel derivatives.

Applies the 3x3 Sobel operators to a single-channel source matrix to estimate intensity gradients in the horizontal and vertical directions.

Both output matrices contain signed gradient values represented using CVL_FLOAT64.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Single-channel source matrix.</span>
  </div>

  <div class="param">
    <strong>gx</strong>
    <span>Destination matrix for horizontal gradients. Must be single-channel CVL_FLOAT64.</span>
  </div>

  <div class="param">
    <strong>gy</strong>
    <span>Destination matrix for vertical gradients. Must be single-channel CVL_FLOAT64.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel_mag()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L354)]

</div>

```c
void cvl_sobel_mag( const cvl_Mat * src,
                    cvl_Mat *       dst );
```

Computes Sobel gradient magnitude.

Calculates horizontal and vertical Sobel derivatives and combines them using the Euclidean magnitude:

<code>magnitude = sqrt(gx * gx + gy * gy)</code>.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Single-channel source matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Single-channel destination matrix containing gradient magnitudes.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_color_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L369)]

</div>

```c
cvl_Mat cvl_cvt_color_new( const cvl_Mat * src,
                           int             code );
```

Converts a matrix to another color space and returns the result.

Grayscale conversion uses the ITU-R BT.601 luminance coefficients.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>code</strong>
    <span>Color conversion code. See <a href="../core#cvl_color_conversion"><code>cvl_color_conversion</code></a>.</span>
  </div>

</div>

**Returns**: Newly allocated matrix containing the converted image.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_depth_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L382)]

</div>

```c
cvl_Mat cvl_cvt_depth_new( const cvl_Mat * src,
                           cvl_depth_t     ddepth,
                           double          alpha,
                           double          beta   );
```

Converts a matrix to another type with scaling and returns the result.

Effectively: <code>dst(x, y) = saturate_cast&lt;ddepth&gt;(alpha * src(x, y) + beta)</code>

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>ddepth</strong>
    <span>Destination depth - type to convert to. See <a href="../core#cvl_depth_t"><code>cvl_depth_t</code></a>.</span>
  </div>

  <div class="param">
    <strong>alpha</strong>
    <span>Multiplicative scale factor (1.0 for no change).</span>
  </div>

  <div class="param">
    <strong>beta</strong>
    <span>Additive offset (0 for no change).</span>
  </div>

</div>

**Returns**: Newly allocated matrix containing the converted data.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_threshold_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L395)]

</div>

```c
cvl_Mat cvl_threshold_new( const cvl_Mat * src,
                           int             thresh,
                           int             maxval,
                           int             type   );
```

Applies a fixed-level threshold to each element and returns the result.

Creates a copy of <code>src</code> and applies the specified thresholding operation.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>thresh</strong>
    <span>Threshold value.</span>
  </div>

  <div class="param">
    <strong>maxval</strong>
    <span>Maximum output value used by binary thresholding.</span>
  </div>

  <div class="param">
    <strong>type</strong>
    <span>Thresholding operation. See <a href="../core#cvl_thresh_type"><code>cvl_thresh_type</code></a>.</span>
  </div>

</div>

**Returns**: Newly allocated thresholded matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L411)]

</div>

```c
cvl_Mat cvl_rotate_new( const cvl_Mat * src,
                        float           angle,
                        cvl_interp_t    interp );
```

Rotates a matrix clockwise around its center and returns the result.

The output dimensions match the input dimensions, except for 90 and 270-degree rotations, which swap the dimensions. Non-orthogonal rotations may crop the corners of the image.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>angle</strong>
    <span>Clockwise rotation angle, in degrees.</span>
  </div>

  <div class="param">
    <strong>interp</strong>
    <span>Interpolation method. See <a href="../core#cvl_interp_t"><code>cvl_interp_t</code></a>.</span>
  </div>

</div>

**Returns**: Newly allocated rotated matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate_bound_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L426)]

</div>

```c
cvl_Mat cvl_rotate_bound_new( const cvl_Mat * src,
                              float           angle,
                              cvl_interp_t    interp );
```

Rotates a matrix clockwise while preserving the entire image.

Automatically expands the output dimensions to accommodate the rotated matrix without cropping its corners.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>angle</strong>
    <span>Clockwise rotation angle, in degrees.</span>
  </div>

  <div class="param">
    <strong>interp</strong>
    <span>Interpolation method. See <a href="../core#cvl_interp_t"><code>cvl_interp_t</code></a>.</span>
  </div>

</div>

**Returns**: Newly allocated rotated matrix with expanded dimensions.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_crop_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L438)]

</div>

```c
cvl_Mat cvl_crop_new( const cvl_Mat * src,
                      int             r,
                      int             c,
                      int             h,
                      int             w   );
```

Extracts a rectangular region of interest into a new matrix.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>r</strong>
    <span>Starting row in the source matrix.</span>
  </div>

  <div class="param">
    <strong>c</strong>
    <span>Starting column in the source matrix.</span>
  </div>

  <div class="param">
    <strong>h</strong>
    <span>Height of the output region.</span>
  </div>

  <div class="param">
    <strong>w</strong>
    <span>Width of the output region.</span>
  </div>

</div>

**Returns**: Newly allocated matrix containing the cropped region.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_resize_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L452)]

</div>

```c
cvl_Mat cvl_resize_new( const cvl_Mat * src,
                        int             height,
                        int             width,
                        cvl_interp_t    type   );
```

Resizes a matrix using the specified interpolation method and returns the result.

The destination matrix determines the output dimensions. Interpolation is performed independently for each channel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>height</strong>
    <span>Height of the output matrix.</span>
  </div>

  <div class="param">
    <strong>width</strong>
    <span>Width of the output matrix.</span>
  </div>

  <div class="param">
    <strong>type</strong>
    <span>Interpolation method. See <a href="../core#cvl_interp_t"><code>cvl_interp_t</code></a>.</span>
  </div>

</div>

**Returns**: Newly allocated resized matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_correlate_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L466)]

</div>

```c
cvl_Mat cvl_correlate_new( const cvl_Mat * src,
                           cvl_Mat *       kernel );
```

Applies two-dimensional correlation and returns the result.

Allocates a destination matrix with the same dimensions, depth, and number of channels as src.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>kernel</strong>
    <span>Correlation kernel.</span>
  </div>

</div>

**Returns**: Newly allocated matrix containing the correlation result.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L480)]

</div>

```c
cvl_Mat cvl_convolve_new( const cvl_Mat * src,
                          cvl_Mat *       kernel );
```

Applies two-dimensional convolution and returns the result.

Allocates a destination matrix with the same dimensions, depth, and number of channels as src.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>kernel</strong>
    <span>Convolution kernel.</span>
  </div>

</div>

**Returns**: Newly allocated matrix containing the convolution result.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_box_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L492)]

</div>

```c
cvl_Mat cvl_blur_box_new( const cvl_Mat * src,
                          int             ksize,
                          bool            normalize );
```

Applies box filtering and returns the result.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square kernel.</span>
  </div>

  <div class="param">
    <strong>normalize</strong>
    <span>Whether to normalize the kernel weights.</span>
  </div>

</div>

**Returns**: Newly allocated blurred matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_mean_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L506)]

</div>

```c
cvl_Mat cvl_blur_mean_new( const cvl_Mat * src,
                           int             ksize );
```

Applies mean blur and returns the result.

Each output element is the average of the values within its square neighborhood.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square kernel.</span>
  </div>

</div>

**Returns**: Newly allocated blurred matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_gauss_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L520)]

</div>

```c
cvl_Mat cvl_blur_gauss_new( const cvl_Mat * src,
                            int             ksize,
                            double          sigma );
```

Applies Gaussian blur and returns the result.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Gaussian kernel width and height, or zero to derive the size from sigma.</span>
  </div>

  <div class="param">
    <strong>sigma</strong>
    <span>Standard deviation of the Gaussian distribution, or zero to derive it from ksize.</span>
  </div>

</div>

**Returns**: Newly allocated blurred matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_median_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L531)]

</div>

```c
cvl_Mat cvl_blur_median_new( const cvl_Mat * src,
                             int             ksize );
```

Applies median filtering and returns the result.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Source matrix.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Width and height of the square neighborhood, must be odd.</span>
  </div>

</div>

**Returns**: Newly allocated median-filtered matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel_mag_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/imgproc.h#L542)]

</div>

```c
cvl_Mat cvl_sobel_mag_new( const cvl_Mat * src );
```

Computes Sobel gradient magnitude and returns the result.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Single-channel source matrix.</span>
  </div>

</div>

**Returns**: Newly allocated single-channel matrix containing gradient magnitudes.

</div>

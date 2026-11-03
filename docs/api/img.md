<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_color()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L56)]

</div>

```c
void cvl_cvt_color( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    int             code );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_depth()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L155)]

</div>

```c
void cvl_cvt_depth( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    cvl_depth_t     ddepth,
                    double          alpha,
                    double          beta   );
```

Converts a matrix to another type with scaling.

Effectively: dst(x, y) = saturate_cast<ddepth>(α * src(x, y) + β)

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Output matrix.</span>
  </div>

  <div class="param">
    <strong>ddepth</strong>
    <span>Type to convert to.</span>
  </div>

  <div class="param">
    <strong>alpha</strong>
    <span>Scale factor (1.0 for no change).</span>
  </div>

  <div class="param">
    <strong>beta</strong>
    <span>Value added to scaled values (0 for no change).</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_threshold()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L256)]

</div>

```c
void cvl_threshold( cvl_Mat *       img,
                    int             thresh,
                    int             maxval,
                    int             type   );
```

Applies a fixed-level threshold to each element — determined by type.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>img</strong>
    <span>Matrix to apply a threshold to.</span>
  </div>

  <div class="param">
    <strong>thresh</strong>
    <span>Threshold value.</span>
  </div>

  <div class="param">
    <strong>maxval</strong>
    <span>Maximum value.</span>
  </div>

  <div class="param">
    <strong>type</strong>
    <span>Type of thresholding operation to perform. See <a href="../core#cvl_thresh_type"><code>cvl_thresh_type</code></a>.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L676)]

</div>

```c
void cvl_rotate( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 float           angle,
                 cvl_interp_t    interp );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_invert()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L337)]

</div>

```c
void cvl_invert( cvl_Mat *       img,
                 double          maxval );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_dilate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L370)]

</div>

```c
void cvl_dilate( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_erode()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L401)]

</div>

```c
void cvl_erode( const cvl_Mat * src,
                cvl_Mat *       dst,
                int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_open()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L432)]

</div>

```c
void cvl_open( const cvl_Mat * src,
               cvl_Mat *       dst,
               int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_close()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L442)]

</div>

```c
void cvl_close( const cvl_Mat * src,
                cvl_Mat *       dst,
                int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_crop()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L452)]

</div>

```c
void cvl_crop( const cvl_Mat * src,
               cvl_Mat *       dst,
               int             r,
               int             c   );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_resize()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L558)]

</div>

```c
void cvl_resize( const cvl_Mat * src,
                 cvl_Mat *       dst,
                 cvl_interp_t    type );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_connected_components()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L759)]

</div>

```c
int cvl_connected_components( const cvl_Mat * src,
                              cvl_Mat *       labels,
                              int             connectivity );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_color_components()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L858)]

</div>

```c
int cvl_color_components( cvl_Mat *       dst,
                          const cvl_Mat * labels,
                          int             thresh );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_correlate()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1004)]

</div>

```c
void cvl_correlate( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    cvl_Mat *       kernel );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1024)]

</div>

```c
void cvl_convolve( const cvl_Mat * src,
                   cvl_Mat *       dst,
                   cvl_Mat *       kernel );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve_sep()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1128)]

</div>

```c
void cvl_convolve_sep( const cvl_Mat * src,
                       cvl_Mat *       dst,
                       cvl_Mat *       kx,
                       cvl_Mat *       ky  );
```

Applies a separable linear filter to an image.

First, every row of src is convolved with the horizontal kernel, then every column of the result is convolved with the vertical kernel.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Output matrix.</span>
  </div>

  <div class="param">
    <strong>kx</strong>
    <span>Horizontal kernel.</span>
  </div>

  <div class="param">
    <strong>ky</strong>
    <span>Vertical kernel.</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_box()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1144)]

</div>

```c
void cvl_blur_box( const cvl_Mat * src,
                   cvl_Mat *       dst,
                   int             ksize,
                   bool            normalize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_mean()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1165)]

</div>

```c
void cvl_blur_mean( const cvl_Mat * src,
                    cvl_Mat *       dst,
                    int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_gauss()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1212)]

</div>

```c
void cvl_blur_gauss( const cvl_Mat * src,
                     cvl_Mat *       dst,
                     int             ksize,
                     double          sigma );
```

Applies Gaussian blur using a kernel defined by ksize and/or sigma.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>dst</strong>
    <span>Output matrix (blurred).</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Kernel size (if 0, derived from sigma).</span>
  </div>

  <div class="param">
    <strong>sigma</strong>
    <span>Standard deviation of the Gaussian kernel (if 0, derived from ksize).</span>
  </div>

</div>

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_median()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1269)]

</div>

```c
void cvl_blur_median( const cvl_Mat * src,
                      cvl_Mat *       dst,
                      int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1340)]

</div>

```c
void cvl_sobel( const cvl_Mat * src,
                cvl_Mat *       gx,
                cvl_Mat *       gy  );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel_mag()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1371)]

</div>

```c
void cvl_sobel_mag( const cvl_Mat * src,
                    cvl_Mat *       dst );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_color_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L120)]

</div>

```c
cvl_Mat cvl_cvt_color_new( const cvl_Mat * src,
                           int             code );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_cvt_depth_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L234)]

</div>

```c
cvl_Mat cvl_cvt_depth_new( const cvl_Mat * src,
                           cvl_depth_t     ddepth,
                           double          alpha,
                           double          beta   );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_threshold_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L330)]

</div>

```c
cvl_Mat cvl_threshold_new( const cvl_Mat * src,
                           int             thresh,
                           int             maxval,
                           int             type   );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L717)]

</div>

```c
cvl_Mat cvl_rotate_new( const cvl_Mat * src,
                        float           angle,
                        cvl_interp_t    interp );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_rotate_bound_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L738)]

</div>

```c
cvl_Mat cvl_rotate_bound_new( const cvl_Mat * src,
                              float           angle,
                              cvl_interp_t    interp );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_crop_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L470)]

</div>

```c
cvl_Mat cvl_crop_new( const cvl_Mat * src,
                      int             r,
                      int             c,
                      int             h,
                      int             w   );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_resize_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L567)]

</div>

```c
cvl_Mat cvl_resize_new( const cvl_Mat * src,
                        int             height,
                        int             width,
                        cvl_interp_t    type   );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_correlate_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1017)]

</div>

```c
cvl_Mat cvl_correlate_new( const cvl_Mat * src,
                           cvl_Mat *       kernel );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_convolve_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1046)]

</div>

```c
cvl_Mat cvl_convolve_new( const cvl_Mat * src,
                          cvl_Mat *       kernel );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_box_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1158)]

</div>

```c
cvl_Mat cvl_blur_box_new( const cvl_Mat * src,
                          int             ksize,
                          bool            normalize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_mean_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1169)]

</div>

```c
cvl_Mat cvl_blur_mean_new( const cvl_Mat * src,
                           int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_gauss_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1248)]

</div>

```c
cvl_Mat cvl_blur_gauss_new( const cvl_Mat * src,
                            int             ksize,
                            double          sigma );
```

Applies Gaussian blur using a kernel defined by ksize and/or sigma.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>src</strong>
    <span>Input matrix.</span>
  </div>

  <div class="param">
    <strong>ksize</strong>
    <span>Kernel size (if 0, derived from sigma).</span>
  </div>

  <div class="param">
    <strong>sigma</strong>
    <span>Standard deviation of the Gaussian kernel (if 0, derived from ksize).</span>
  </div>

</div>

**Returns**: Blurred matrix.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_blur_median_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1306)]

</div>

```c
cvl_Mat cvl_blur_median_new( const cvl_Mat * src,
                             int             ksize );
```

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_sobel_mag_new()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/src/imgproc.c#L1391)]

</div>

```c
cvl_Mat cvl_sobel_mag_new( const cvl_Mat * src );
```

</div>

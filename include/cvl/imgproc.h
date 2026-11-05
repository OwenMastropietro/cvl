/**
 * @file imgproc.h
 * @brief Image processing operations.
 * 
 * Provides functions for common image processing tasks, including:
 * 
 * - **Color and type conversion:** color-space conversion, depth conversion, thresholding.
 * - **Geomatric Transformations:** rotation, cropping, resizing.
 * - **Morphological Transformations:** dilation, erosion, opening, closing.
 * - **Connected Component Analysis:** labeling and visualization.
 * - **Filtering and Convolution:** correlation, convolution, box filtering, Gaussian blur, median blur.
 * - **Edge Detection:** Sobel derivatives and gradient magnitudes.
 * 
 */

#pragma once

#include <cvl/core.h>
#include <cvl/error.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts a matrix to another color space.
 * 
 * Grayscale conversion uses the ITU-R BT.601 luminance coefficients.
 *
 * @param src Input matrix.
 * @param dst Destination matrix. Must have same size as `src` and the channel
 * count required by `code`.
 * @param code Color conversion code. See @ref cvl_color_conversion.
 */
void cvl_cvt_color(const cvl_Mat *src, cvl_Mat *dst, int code);

/**
 * @brief Converts a matrix to another type with scaling.
 *
 * Effectively: `dst(x, y) = saturate_cast<ddepth>(alpha * src(x, y) + beta)`
 *
 * @param src Input matrix.
 * @param dst Output matrix. Must have the same dimensions and number of
 * channels as `src`.
 * @param ddepth Destination depth - type to convert to. See @ref cvl_depth_t.
 * @param alpha Multiplicative scale factor (1.0 for no change).
 * @param beta Additive offset (0 for no change).
 */
void cvl_cvt_depth(const cvl_Mat *src, cvl_Mat *dst, cvl_depth_t ddepth, double alpha, double beta);

/**
 * @brief Applies a fixed-level threshold to each element.
 *
 * @param img Matrix to modify in place.
 * @param thresh Threshold value.
 * @param maxval Maximum output value used by binary thresholding.
 * @param type Thresholding operation. See @ref cvl_thresh_type.
 */
void cvl_threshold(cvl_Mat *img, int thresh, int maxval, int type);

/**
 * @brief Rotates a matrix clockwise around its center.
 *
 * The dimensions of `dst` determine the output region.
 * Source coordinates outside the input matrix are left unchanged
 * in the destination matrix.
 *
 * Orthogonal rotations use direct pixel rearrangement.
 * Other angles use the specified interpolation method.
 *
 * @param src Source matrix.
 * @param dst Destination matrix.
 * @param angle Clockwise rotation angle, in degrees.
 * @param interp Interpolation method. See @ref cvl_interp_t.
 *
 * @see cvl_rotate_new(), cvl_rotate_bound_new()
 */
void cvl_rotate(const cvl_Mat *src, cvl_Mat *dst, float angle, cvl_interp_t interp);

/**
 * @brief Inverts matrix values relative to a specified maximum.
 *
 * Each element's `value` becomes `maxval - value`.
 * Values are processed independently for each channel.
 *
 * @param img Matrix to modify in place.
 * @param maxval Maximum value used for inversion.
 */
void cvl_invert(cvl_Mat *img, double maxval);

/**
 * @brief Applies morphological dilation to a matrix.
 *
 * Replaces each element with the maximum value within its
 * square neighborhood.
 *
 * Border pixels are handled by replicating the nearest edge pixel.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square structuring element.
 */
void cvl_dilate(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Applies morphological erosion to a matrix.
 *
 * Replaces each element with the minimum value within its
 * square neighborhood.
 *
 * Border pixels are handled by replicating the nearest edge pixel.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square structuring element.
 */
void cvl_erode(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Applies morphological opening to a matrix.
 *
 * Performs erosion followed by dilation using the same
 * square structuring element.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square structuring element.
 *
 * @see cvl_erode(), cvl_dilate(), cvl_close()
 */
void cvl_open(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Applies morphological closing to a matrix.
 *
 * Performs dilation followed by erosion using the same
 * square structuring element.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square structuring element.
 *
 * @see cvl_dilate(), cvl_erode(), cvl_open()
 */
void cvl_close(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Copies a rectangular region of interest into a destination matrix.
 *
 * The region begins at (r, c) in the source matrix and has the
 * dimensions of dst.
 *
 * @param src Source matrix.
 * @param dst Destination matrix defining the output dimensions.
 * @param r Starting row in the source matrix.
 * @param c Starting column in the source matrix.
 */
void cvl_crop(const cvl_Mat *src, cvl_Mat *dst, int r, int c);

/**
 * @brief Resizes a matrix using the specified interpolation method.
 *
 * The destination matrix determines the output dimensions.
 * Interpolation is performed independently for each channel.
 *
 * @param src Source matrix.
 * @param dst Destination matrix defining the output dimensions.
 * @param type Interpolation method. See @ref cvl_interp_t.
 */
void cvl_resize(const cvl_Mat *src, cvl_Mat *dst, cvl_interp_t type);

/**
 * @brief Labels connected foreground components in a binary image.
 *
 * WHITE pixels are treated as background; all other
 * pixels are treated as foreground.
 *
 * Labels are assigned positive integer values, with zero reserved
 * for background pixels.
 *
 * @param src Single-channel CVL_UINT8 binary source matrix.
 * @param labels Single-channel CVL_32S destination matrix. Must have
 * the same dimensions as src.
 * @param connectivity Pixel connectivity: 4 or 8.
 * @return Number of connected foreground components.
 *
 * @see cvl_color_components()
 */
int cvl_connected_components(const cvl_Mat *src, cvl_Mat *labels, int connectivity);

/**
 * @brief Colors connected components that meet a minimum size.
 *
 * Calculates the size of each labeled component and assigns a
 * unique color to components containing at least thresh pixels.
 * Components smaller than the threshold are unchanged.
 *
 * @param src Three-channel CVL_UINT8 image to modify.
 * @param labels Single-channel CVL_32S component label matrix.
 * @param thresh Minimum component size, in pixels.
 * @return Number of components meeting the minimum size.
 *
 * @see cvl_connected_components()
 */
int cvl_color_components(cvl_Mat *src, const cvl_Mat *labels, int thresh);

/**
 * @brief Applies a two-dimensional correlation operation.
 *
 * The kernel is not flipped. Values outside the source matrix
 * are treated as zero (constant-zero padding).
 *
 * The output has the same dimensions, depth, and number of channels
 * as the source matrix.
 *
 * @param src Source matrix.
 * @param dst Destination matrix.
 * @param kernel Single-channel CVL_FLOAT64 correlation kernel.
 *
 * @see cvl_correlate_new(), cvl_convolve()
 */
void cvl_correlate(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

/**
 * @brief Applies a two-dimensional convolution operation.
 *
 * Convolves the source matrix with the specified kernel.  
 * Convolution flips the kernel by 180-degrees and then applies correlation.
 *
 * Values outside the source matrix are treated as zero
 * (constant-zero padding).
 *
 * @param src Source matrix.
 * @param dst Destination matrix.
 * @param kernel Convolution kernel.
 *
 * @see cvl_convolve_new(), cvl_correlate()
 */
void cvl_convolve(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kernel);

/**
 * @brief Applies a separable two-dimensional convolution.
 *
 * First convolves each row using the horizontal kernel, then
 * convolves each column of the intermediate result using the
 * vertical kernel.
 *
 * This operation is useful when a two-dimensional filter can
 * be represented as the product of two one-dimensional kernels.
 *
 * @param src Source matrix.
 * @param dst Destination matrix.
 * @param kx Horizontal one-dimensional kernel.
 * @param ky Vertical one-dimensional kernel.
 */
void cvl_convolve_sep(const cvl_Mat *src, cvl_Mat *dst, cvl_Mat *kx, cvl_Mat *ky);

/**
 * @brief Applies box filtering using a uniform square kernel.
 *
 * Each output element is calculated from the sum of its
 * neighborhood values. If normalize is true, the kernel weights
 * are scaled by 1/(ksize * ksize), producing a mean filter.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square kernel.
 * @param normalize Whether to normalize the kernel weights.
 *
 * @see cvl_blur_box_new(), cvl_blur_mean()
 */
void cvl_blur_box(const cvl_Mat *src, cvl_Mat *dst, int ksize, bool normalize);

/**
 * @brief Applies a normalized box filter (mean blur).
 *
 * Each output element is the average of the values within
 * its square neighborhood.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square kernel.
 *
 * @see cvl_blur_mean_new(), cvl_blur_box()
 */
void cvl_blur_mean(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Applies Gaussian blur to a matrix.
 *
 * If ksize is zero, the kernel size is derived from sigma.
 * If sigma is zero, the standard deviation is derived
 * from ksize.
 *
 * @param src Source matrix.
 * @param dst Destination matrix containing the blurred result.
 * @param ksize Gaussian kernel width and height. Must be positive
 *              when sigma is not specified.
 * @param sigma Standard deviation of the Gaussian distribution.
 *              Must be positive when ksize is not specified.
 * @return @ref CVL_OK on success, or an error code on failure. See @ref cvl_status.
 *
 * @see cvl_blur_gauss_new()
 */
cvl_status cvl_blur_gauss(const cvl_Mat *src, cvl_Mat *dst, int ksize, double sigma);

/**
 * @brief Applies median filtering to a matrix.
 *
 * Replaces each element with the median value of its square neighborhood.
 * Border pixels are handled by replicating the nearest edge pixel.
 *
 * @param src Source matrix.
 * @param dst Destination matrix. Must have the same dimensions as src.
 * @param ksize Width and height of the square neighborhood, must be odd.
 *
 * @see cvl_blur_median_new()
 */
void cvl_blur_median(const cvl_Mat *src, cvl_Mat *dst, int ksize);

/**
 * @brief Computes horizontal and vertical Sobel derivatives.
 *
 * Applies the 3x3 Sobel operators to a single-channel source matrix
 * to estimate intensity gradients in the horizontal and vertical
 * directions.
 *
 * Both output matrices contain signed gradient values represented
 * using CVL_FLOAT64.
 *
 * @param src Single-channel source matrix.
 * @param gx Destination matrix for horizontal gradients.
 *            Must be single-channel CVL_FLOAT64.
 * @param gy Destination matrix for vertical gradients.
 *            Must be single-channel CVL_FLOAT64.
 *
 * @see cvl_sobel_mag(), cvl_sobel_mag_new()
 */
void cvl_sobel(const cvl_Mat *src, cvl_Mat *gx, cvl_Mat *gy);

/**
 * @brief Computes Sobel gradient magnitude.
 *
 * Calculates horizontal and vertical Sobel derivatives and combines
 * them using the Euclidean magnitude:
 *
 * `magnitude = sqrt(gx * gx + gy * gy)`.
 *
 * @param src Single-channel source matrix.
 * @param dst Single-channel destination matrix containing gradient
 *            magnitudes.
 *
 * @see cvl_sobel(), cvl_sobel_mag_new()
 */
void cvl_sobel_mag(const cvl_Mat *src, cvl_Mat *dst);

// ================
// Convenience "_new" wrappers
// ================

/**
 * @brief Converts a matrix to another color space and returns the result.
 *
 * Grayscale conversion uses the ITU-R BT.601 luminance coefficients.
 *
 * @param src Input matrix.
 * @param code Color conversion code. See @ref cvl_color_conversion.
 * @return Newly allocated matrix containing the converted image.
 */
cvl_Mat cvl_cvt_color_new(const cvl_Mat *src, int code);

/**
 * @brief Converts a matrix to another type with scaling and returns the result.
 *
 * Effectively: `dst(x, y) = saturate_cast<ddepth>(alpha * src(x, y) + beta)`
 *
 * @param src Input matrix.
 * @param ddepth Destination depth - type to convert to. See @ref cvl_depth_t.
 * @param alpha Multiplicative scale factor (1.0 for no change).
 * @param beta Additive offset (0 for no change).
 * @return Newly allocated matrix containing the converted data.
 */
cvl_Mat cvl_cvt_depth_new(const cvl_Mat *src, cvl_depth_t ddepth, double alpha, double beta);

/**
 * @brief Applies a fixed-level threshold to each element and returns the result.
 *
 * Creates a copy of `src` and applies the specified thresholding operation.
 *
 * @param src Input matrix.
 * @param thresh Threshold value.
 * @param maxval Maximum output value used by binary thresholding.
 * @param type Thresholding operation. See @ref cvl_thresh_type.
 * @return Newly allocated thresholded matrix.
 */
cvl_Mat cvl_threshold_new(const cvl_Mat *src, int thresh, int maxval, int type);

/**
 * @brief Rotates a matrix clockwise around its center and returns the result.
 *
 * The output dimensions match the input dimensions, except for 90 and 270-degree
 * rotations, which swap the dimensions.
 * Non-orthogonal rotations may crop the corners of the image.
 *
 * @param src Source matrix.
 * @param angle Clockwise rotation angle, in degrees.
 * @param interp Interpolation method. See @ref cvl_interp_t.
 * @return Newly allocated rotated matrix.
 *
 * @see cvl_rotate(), cvl_rotate_bound_new()
 */
cvl_Mat cvl_rotate_new(const cvl_Mat *src, float angle, cvl_interp_t interp);

/**
 * @brief Rotates a matrix clockwise while preserving the entire image.
 *
 * Automatically expands the output dimensions to accommodate the
 * rotated matrix without cropping its corners.
 *
 * @param src Source matrix.
 * @param angle Clockwise rotation angle, in degrees.
 * @param interp Interpolation method. See @ref cvl_interp_t.
 * @return Newly allocated rotated matrix with expanded dimensions.
 *
 * @see cvl_rotate(), cvl_rotate_new()
 */
cvl_Mat cvl_rotate_bound_new(const cvl_Mat *src, float angle, cvl_interp_t interp);

/**
 * @brief Extracts a rectangular region of interest into a new matrix.
 *
 * @param src Source matrix.
 * @param r Starting row in the source matrix.
 * @param c Starting column in the source matrix.
 * @param h Height of the output region.
 * @param w Width of the output region.
 * @return Newly allocated matrix containing the cropped region.
 */
cvl_Mat cvl_crop_new(const cvl_Mat *src, int r, int c, int h, int w);

/**
 * @brief Resizes a matrix using the specified interpolation method and returns the result.
 *
 * The destination matrix determines the output dimensions.
 * Interpolation is performed independently for each channel.
 *
 * @param src Source matrix.
 * @param height Height of the output matrix.
 * @param width Width of the output matrix.
 * @param type Interpolation method. See @ref cvl_interp_t.
 * @return Newly allocated resized matrix.
 */
cvl_Mat cvl_resize_new(const cvl_Mat *src, int height, int width, cvl_interp_t type);

/**
 * @brief Applies two-dimensional correlation and returns the result.
 *
 * Allocates a destination matrix with the same dimensions, depth,
 * and number of channels as src.
 *
 * @param src Source matrix.
 * @param kernel Correlation kernel.
 * @return Newly allocated matrix containing the correlation result.
 *
 * @see cvl_correlate(), cvl_convolve_new()
 */
cvl_Mat cvl_correlate_new(const cvl_Mat *src, cvl_Mat *kernel);

/**
 * @brief Applies two-dimensional convolution and returns the result.
 *
 * Allocates a destination matrix with the same dimensions, depth,
 * and number of channels as src.
 *
 * @param src Source matrix.
 * @param kernel Convolution kernel.
 * @return Newly allocated matrix containing the convolution result.
 *
 * @see cvl_convolve(), cvl_correlate_new()
 */
cvl_Mat cvl_convolve_new(const cvl_Mat *src, cvl_Mat *kernel);

/**
 * @brief Applies box filtering and returns the result.
 *
 * @param src Source matrix.
 * @param ksize Width and height of the square kernel.
 * @param normalize Whether to normalize the kernel weights.
 * @return Newly allocated blurred matrix.
 *
 * @see cvl_blur_box()
 */
cvl_Mat cvl_blur_box_new(const cvl_Mat *src, int ksize, bool normalize);

/**
 * @brief Applies mean blur and returns the result.
 *
 * Each output element is the average of the values within
 * its square neighborhood.
 *
 * @param src Source matrix.
 * @param ksize Width and height of the square kernel.
 * @return Newly allocated blurred matrix.
 *
 * @see cvl_blur_mean()
 */
cvl_Mat cvl_blur_mean_new(const cvl_Mat *src, int ksize);

/**
 * @brief Applies Gaussian blur and returns the result.
 *
 * @param src Source matrix.
 * @param ksize Gaussian kernel width and height, or zero to derive
 *              the size from sigma.
 * @param sigma Standard deviation of the Gaussian distribution,
 *              or zero to derive it from ksize.
 * @return Newly allocated blurred matrix.
 *
 * @see cvl_blur_gauss()
 */
cvl_Mat cvl_blur_gauss_new(const cvl_Mat *src, int ksize, double sigma);

/**
 * @brief Applies median filtering and returns the result.
 *
 * @param src Source matrix.
 * @param ksize Width and height of the square neighborhood, must be odd.
 * @return Newly allocated median-filtered matrix.
 *
 * @see cvl_blur_median()
 */
cvl_Mat cvl_blur_median_new(const cvl_Mat *src, int ksize);

/**
 * @brief Computes Sobel gradient magnitude and returns the result.
 *
 * @param src Single-channel source matrix.
 * @return Newly allocated single-channel matrix containing
 *         gradient magnitudes.
 *
 * @see cvl_sobel_mag(), cvl_sobel()
 */
cvl_Mat cvl_sobel_mag_new(const cvl_Mat *src);

#ifdef __cplusplus
}
#endif

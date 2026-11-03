/**
 * @file io.h
 * @brief Reading and writing images.
 *
 * Based on template from (V2.2 by Marc Pomplun on 10/19/2013).
 */

#pragma once

#include <cvl/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads an image from the specified file.
 *
 * The image format is detected from the file header, not the file extension.
 *
 * @param filename Path to read the image from.
 * @return A @ref CVL_UINT8 matrix containing the decoded image, or an empty matrix on failure.
 *
 * See @ref cvl_format_t.
 */
cvl_Mat cvl_imread(const char *filename);

/**
 * Writes an image to the specified file.
 *
 * The image format is determined from the file extension.
 * 
 * | Format | Read        | Write  | Requirements                                            |
 * | ------ | ----------- | ------ | ------------------------------------------------------- |
 * | PBM    | binary `P4` | `.pbm` | 1 channel, `CVL_UINT8`, pixels must be `0` or `255`     |
 * | PGM    | binary `P5` | `.pgm` | 1 channel, `CVL_UINT8`                                  |
 * | PPM    | binary `P6` | `.ppm` | 3 channels, `CVL_UINT8`                                 |
 * | PNG    | stb         | `.png` | `CVL_UINT8`, 1 to 4 channels                            |
 * | JPEG   | stb         | `.jpg` | `CVL_UINT8`, 1 or 3 channels, no padding between pixels |
 * 
 * > JPEG is written at quality `90`.
 *
 * @param filename Path to write the image to.
 * @param img Image to write.
 * @return `0` on success, `1` on failure.
 *
 * See @ref cvl_format_t.
 */
int cvl_imwrite(const char *filename, cvl_Mat *img);

#ifdef __cplusplus
}
#endif

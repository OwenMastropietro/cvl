# I/O

File reference for [`io.h`](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/io.h)
 / 
[`io.c`](https://github.com/OwenMastropietro/cvl/blob/main/src/io.c).

Reading and writing images.

Based on template from (V2.2 by Marc Pomplun on 10/19/2013).

## Headers
---
[`#include <cvl/core.h>`](core.md)  

## Functions
---
<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_imread()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/io.h#L26)]

</div>

```c
cvl_Mat cvl_imread( const char * filename );
```

Reads an image from the specified file.

The image format is detected from the file header, not the file extension.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>filename</strong>
    <span>Path to read the image from.</span>
  </div>

</div>

**Returns**: A <code>CVL_UINT8</code> matrix containing the decoded image, or an empty matrix on failure.

</div>

<div class="api-card" markdown>

<div class="api-header" markdown>

### cvl_imwrite()

[[source](https://github.com/OwenMastropietro/cvl/blob/main/include/cvl/io.h#L49)]

</div>

```c
int cvl_imwrite( const char * filename,
                 cvl_Mat *    img      );
```

Writes an image to the specified file.

The image format is determined from the file extension.

| Format | Read | Write | Requirements |
| --- | --- | --- | --- |
| PBM | binary <code>P4</code> | <code>.pbm</code> | 1 channel, <code>CVL_UINT8</code>, pixels must be <code>0</code> or <code>255</code> |
| PGM | binary <code>P5</code> | <code>.pgm</code> | 1 channel, <code>CVL_UINT8</code> |
| PPM | binary <code>P6</code> | <code>.ppm</code> | 3 channels, <code>CVL_UINT8</code> |
| PNG | stb | <code>.png</code> | <code>CVL_UINT8</code>, 1 to 4 channels |
| JPEG | stb | <code>.jpg</code> | <code>CVL_UINT8</code>, 1 or 3 channels, no padding between pixels |

JPEG is written at quality <code>90</code>.

**Parameters**

<div class="param-list">
  <div class="param">
    <strong>filename</strong>
    <span>Path to write the image to.</span>
  </div>

  <div class="param">
    <strong>img</strong>
    <span>Image to write.</span>
  </div>

</div>

**Returns**: <code>0</code> on success, <code>1</code> on failure.

</div>

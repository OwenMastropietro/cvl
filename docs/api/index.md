# API

Include one header:

```c
#include <cvl/cvl.h>
```

This includes CVL's core, image I/O, and image-processing interfaces.  
The headers provide `extern "C"` guards and can also be included from C++.

| Page                                    | What it covers                                                                  |
| --------------------------------------- | ------------------------------------------------------------------------------- |
| [Core](core.md)                         | `cvl_Mat`, depths, allocation, and element access                               |
| [I/O](io.md)                            | `cvl_imread`, `cvl_imwrite`, and the formats they accept                        |
| [Image Processing](image-processing.md) | Color conversion, thresholding, filtering, morphology, labeling, edge detection, etc. |

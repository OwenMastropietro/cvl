#include <cvl/cvl_error.h>
#include <cvl/cvl_io.h>

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cvl_format_t detect_format(FILE *f) {
    assert(f);

    unsigned char buf[8];

    fscanf(f, "%s", buf);

    // ascii PNM (not implemented)
    // if (buf[0] == 'P' && buf[1] == '1') return CVL_FMT_PBM;
    // if (buf[0] == 'P' && buf[1] == '2') return CVL_FMT_PGM;
    // if (buf[0] == 'P' && buf[1] == '3') return CVL_FMT_PPM;    

    // binary PNM
    if (buf[0] == 'P' && buf[1] == '4') return CVL_FMT_PBM;
    if (buf[0] == 'P' && buf[1] == '5') return CVL_FMT_PGM;
    if (buf[0] == 'P' && buf[1] == '6') return CVL_FMT_PPM;

    // cvl_perrorf(CVL_ERR_FILE_IO, "CVL only supports binary PNM formats (i.e., PBM, PGM, PPM).");

    return CVL_FMT_UNKNOWN;
}

static Image read_pnm(const char *filename) {
    Image img = { .height = 0, .width = 0, .map = NULL };

    FILE *f = fopen(filename, "rb");
    if (!f) return img;

    cvl_format_t fmt = detect_format(f);

    // stride = width * channels * sizeof(uint8_t) (1)
    // bpp = depth * channels
    int bpp = 0; // bits per pixel (e.g., 8 bits x 3 channels = 24 bpp)
    switch (fmt) {
        case CVL_FMT_PBM: bpp = 1; break;
        case CVL_FMT_PGM: bpp = 8; break;
        case CVL_FMT_PPM: bpp = 24; break;
        case CVL_FMT_UNKNOWN: return img; // error
    }

    // skip comments
    char line[200];
    line[0] = '#';
    while (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
        fgets(line, 200, f);
    }

    int width, height;
    sscanf(line, "%d %d", &width, &height);
    if (width <= 0 || height <= 0) {
        fclose(f);
        return img;
    }

    int maxval = 0;  // max intensity value in file
    if (fmt == CVL_FMT_PGM || fmt == CVL_FMT_PPM) {
        fgets(line, 200, f);
        sscanf(line, "%d", &maxval);
    }

    int mapsize = (bpp * width + 7) / 8 * height;
    
    uint8_t *data = malloc(mapsize);
    if (!data) return img;

    int bytes_read = (int)fread(data, 1, mapsize, f);
    if (bytes_read != mapsize) {
        free(data);
        fclose(f);
        return img;
    }

    fclose(f);
    
    int stride = 0;
    switch (fmt) {
        case CVL_FMT_PBM: stride = (width + 7) / 8; break;
        case CVL_FMT_PGM: stride = width;           break;
        case CVL_FMT_PPM: stride = width * 3;       break;
        case CVL_FMT_UNKNOWN: assert(0);
    }

    img = cvl_img_create(height, width);

    // Read data into image
    for (int i = 0; i < height; i++) {
        const uint8_t *row = data + i * stride;
            
        for (int j = 0; j < width; j++) {
            Pixel *p = &img.map[i][j];

            switch (fmt) {
                case CVL_FMT_PBM: { // bit (un)pack and swap black/white
                    int bit = (row[j / 8] >> (7 - (j % 8))) & 1;
                    uint8_t v = bit ? 0 : 255;
                    p->r = p->g = p->b = p->i = v;
                    break;
                }
                case CVL_FMT_PGM: {
                    uint8_t v = (uint8_t)((int)row[j] * 255 / maxval);
                    p->r = p->g = p->b = p->i = v;
                    break;
                }
                case CVL_FMT_PPM: {
                    uint8_t r = (uint8_t)((int)row[j * 3 + 0] * 255 / maxval);
                    uint8_t g = (uint8_t)((int)row[j * 3 + 1] * 255 / maxval);
                    uint8_t b = (uint8_t)((int)row[j * 3 + 2] * 255 / maxval);
                    // int gray = 0.299*r + 0.587*g + 0.114*b; // ITU-R BT.601-7
                    uint16_t sum = (uint16_t)r + g + b;
                    uint8_t gray = (uint8_t)(sum / 3);

                    p->r = r;
                    p->g = g;
                    p->b = b;
                    p->i = gray;
                    break;
                }
                case CVL_FMT_UNKNOWN: assert(0);
            }

            // todo: cvl_imread_modes / cvl_imread(filename, mode)
            // but Image struct has rgbi so don't really need...
        }
    }

    free(data);

    return img;
}

static cvl_status_t write_pbm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) return CVL_ERR_FILE_IO;

    fprintf(f, "P4\n%d %d\n", img->width, img->height);

    int rowbytes = (img->width + 7) / 8;
    uint8_t *row = calloc(rowbytes, 1);
    if (!row) return CVL_ERR_OUT_OF_MEMORY;

    for (int i = 0; i < img->height; ++i) {
        memset(row, 0, rowbytes);
        for (int j = 0; j < img->width; ++j) {
            if (!(img->map[i][j].i == BLACK || img->map[i][j].i == WHITE)) {
                // todo: apply threshold instead?
                return cvl_perrorf(CVL_ERR_INTERNAL, "PBM is binary, fool");
            }

            if (img->map[i][j].i == BLACK) { // hawk tuah, BLACK is 0 fool
                row[j / 8] |= 0x80 >> (j % 8);
            }
        }
        fwrite(row, 1, rowbytes, f);
    }
    free(row);
    fclose(f);

    return CVL_OK;
}

static cvl_status_t write_pgm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) return CVL_ERR_FILE_IO;

    fprintf(f, "P5\n%d %d\n255\n", img->width, img->height);

    uint8_t *row = malloc(img->width);
    if(!row) return CVL_ERR_OUT_OF_MEMORY;

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            row[j] = img->map[i][j].i;
        }
        fwrite(row, 1, img->width, f);
    }
    free(row);
    fclose(f);

    return CVL_OK;
}

static cvl_status_t write_ppm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) return CVL_ERR_FILE_IO;

    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);

    uint8_t *row = malloc(3 * img->width);
    if(!row) return CVL_ERR_OUT_OF_MEMORY;

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            row[3 * j + 0] = img->map[i][j].r;
            row[3 * j + 1] = img->map[i][j].g;
            row[3 * j + 2] = img->map[i][j].b;
        }
        fwrite(row, 1, 3 * img->width, f);
    }
    free(row);
    fclose(f);

    return CVL_OK;
}

// Reads an image from a specified file.
Image cvl_imread(const char *filename) {
    return read_pnm(filename);
}

// Saves an image to a specified file. The image format is determined by the file extension.
cvl_status_t cvl_imwrite(const char *filename, Image *img) {
    if (!img) return CVL_ERR_INVALID_ARG;
    if (!img->map) return CVL_ERR_EMPTY;
    if(img->height <= 0 || img->width <= 0) return CVL_ERR_INVALID_DIM;

    const char *ext = strrchr(filename, '.');
    if (!ext) return CVL_ERR_INTERNAL;

    if (strcmp(ext, ".pbm") == 0) {
        return write_pbm(filename, img);
    }

    if (strcmp(ext, ".pgm") == 0) {
        return write_pgm(filename, img);
    }

    if (strcmp(ext, ".ppm") == 0) {
        return write_ppm(filename, img);
    }

    return CVL_ERR_FILE_IO;
}

#define _CRT_SECURE_NO_WARNINGS

#include "cvl_io.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int detect_format(FILE *f) {
    unsigned char buf[8];

    fread(buf, 1, 8, f);
    rewind(f);

    if (buf[0] == 'P' && buf[1] == '4') return CVL_FMT_PBM;
    if (buf[0] == 'P' && buf[1] == '5') return CVL_FMT_PGM;
    if (buf[0] == 'P' && buf[1] == '6') return CVL_FMT_PPM;

    // other formats...

    return CVL_FMT_UNKNOWN;
}

static cvl_Mat read_pnm(FILE *f) {
    assert(f);

    int format = detect_format(f);
    int height, width, maxval = 255;
    assert((format == CVL_FMT_PBM)
        || (format == CVL_FMT_PGM)
        || (format == CVL_FMT_PPM));

    char magic[3]; // consume format specifier
    assert(fscanf(f, "%2s", magic) == 1);

    // skip comments
    char line[200];
    *line = '#';
    while (*line == '#' || *line == '\n' || *line == '\r') {
        fgets(line, sizeof(line), f);
    }

    assert(sscanf(line, "%d %d", &width, &height) == 2);
    assert(height > 0 && width > 0);
    
    int channels = (format == CVL_FMT_PPM) ? 3 : 1;

    cvl_Mat img = cvl_mat_create(height, width, channels, CVL_UINT8);

    size_t pixels = (size_t)width * height;

    if (format == CVL_FMT_PGM || format == CVL_FMT_PPM) {
        fscanf(f, "%d", &maxval);
        fgetc(f); // newline
        size_t bytes = pixels * channels;
        assert(fread(img.data, 1, bytes, f) == bytes);

        if (maxval != 255) { // normalize
            uint8_t *d = img.data;
            for (size_t i = 0; i < bytes; ++i) {
                d[i] = (uint8_t)(d[i] * 255 / maxval);
            }
        }
    
    } else if (format == CVL_FMT_PBM) { // (bit aligned) unpack bits
        size_t rowbytes = (width + 7) / 8;
        uint8_t *row = malloc(rowbytes);
        assert(row);
        uint8_t *d = img.data;

        for (int i = 0; i < height; ++i) {
            assert(fread(row, 1, rowbytes, f) == rowbytes);

            for (int j = 0; j < width; ++j) {
                int byte = j / 8;
                int bit = 7 - (j % 8);
                int value = (row[byte] >> bit) & 1;
                d[i * width + j] = value ? 0 : 255;
            }
        }
        free(row);
        row = NULL;
    }
    
    return img;
}

static int write_pbm(const char *filename, cvl_Mat *img) {
    assert(img->channels == 1 && img->depth == CVL_UINT8);

    FILE *f = fopen(filename, "wb");
    assert(f);

    int height = img->height;
    int width = img->width;
    const uint8_t *data = img->data;

    fprintf(f, "P4\n%d %d\n", width, height);

    int rowbytes = (width + 7) / 8;
    uint8_t *row = calloc(rowbytes, 1);
    assert(row);
    for (int i = 0; i < height; ++i) {
        memset(row, 0, rowbytes);
        for (int j = 0; j < width; ++j) {
            uint8_t p = data[i * width + j];
            assert(p == BLACK || p == WHITE);

            if (p == BLACK) { // hawk tuah, BLACK is 0 fool
                row[j / 8] |= 0x80 >> (j % 8);
            }
        }
        fwrite(row, 1, rowbytes, f);
    }
    free(row);
    fclose(f);

    return 0;
}

static int write_pgm(const char *filename, cvl_Mat *img) {
    assert(img->channels == 1 && img->depth == CVL_UINT8);

    FILE *f = fopen(filename, "wb");
    assert (f);

    fprintf(f, "P5\n%d %d\n255\n", img->width, img->height);
    fwrite(img->data, 1, img->height * img->width, f);

    fclose(f);

    return 0;
}

static int write_ppm(const char *filename, cvl_Mat *img) {
    FILE *f = fopen(filename, "wb");
    assert(f);

    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);
    fwrite(img->data, 1, img->height * img->width * img->channels, f);

    fclose(f);

    return 0;
}

// Reads an image from a specified file.
cvl_Mat cvl_imread(const char *filename) {
    FILE *f = fopen(filename, "rb");
    assert(f);

    int format = detect_format(f);

    cvl_Mat img;

    switch (format) {
        case CVL_FMT_PBM:
        case CVL_FMT_PGM:
        case CVL_FMT_PPM:
            img = read_pnm(f);
            break;
        // case CVL_FMT_PNG:
        //     img = cvl_read_png(f);
        //     break;
        // ...
    }

    fclose(f);

    return img;
}

// Saves an image to a specified file. The image format is determined by the file extension.
int cvl_imwrite(const char *filename, cvl_Mat *img) {
    assert(filename && img && img->data);
    assert(img->height > 0 && img->width > 0 && img->channels > 0);

    const char *ext = strrchr(filename, '.');
    assert(ext);

    if (strcmp(ext, ".pbm") == 0) {
        return write_pbm(filename, img);
    }
    
    if (strcmp(ext, ".pgm") == 0) {
        return write_pgm(filename, img);
    }
    
    if (strcmp(ext, ".ppm") == 0) {
        return write_ppm(filename, img);
    }

    fprintf(stderr, "Unsupported format\n");
    return -1;
}

#include <cvl/io.h>

#include <stb_image.h>
#include <stb_image_write.h>

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

    long pos = ftell(f);
    if (pos < 0) {
        return CVL_FMT_UNKNOWN;
    }

    size_t n = fread(buf, 1, sizeof(buf), f);
    fseek(f, pos, SEEK_SET);

    if (n < 2) {
        return CVL_FMT_UNKNOWN;
    }

    // Binary PNM
    if (buf[0] == 'P' && buf[1] == '4') return CVL_FMT_PBM;
    if (buf[0] == 'P' && buf[1] == '5') return CVL_FMT_PGM;
    if (buf[0] == 'P' && buf[1] == '6') return CVL_FMT_PPM;

    // PNG
    if (n >= 8 && memcmp(buf, "\x89PNG\r\n\x1a\n", 8) == 0) {
        return CVL_FMT_PNG;
    }

    // JPG
    if (buf[0] == 0xFF && buf[1] == 0xD8) {
        return CVL_FMT_JPG;
    }

    return CVL_FMT_UNKNOWN;
}

static int stb_read(void *user, char *data, int size) {
    FILE *f = (FILE *)user;
    return (int)fread(data, 1, size, f);
}

static void stb_skip(void *user, int n) {
    FILE *f = (FILE *)user;
    fseek(f, n, SEEK_CUR);
}

static int stb_eof(void *user) {
    FILE *f = (FILE *)user;
    return feof(f);
}

static cvl_Mat read_stb(FILE *f) {
    cvl_Mat img = {0};

    if (f == NULL) {
        return img;
    }

    stbi_io_callbacks cb = {
        .read = stb_read,
        .skip = stb_skip,
        .eof  = stb_eof,
    };

    int width;
    int height;
    int channels;

    uint8_t *data = stbi_load_from_callbacks(&cb, f, &width, &height, &channels, 0);
    if (!data) {
        return img;
    }

    img = cvl_mat_create_from(height, width, channels, CVL_UINT8, data);

    stbi_image_free(data);

    return img;
}

static cvl_Mat read_pnm(FILE *f) {
    assert(f);

    cvl_Mat img = {0};

    int format = detect_format(f);
    if (format == CVL_FMT_UNKNOWN) return img;

    int height = 0;
    int width = 0;
    int maxval = 255;

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

    img = cvl_mat_create(height, width, channels, CVL_UINT8);

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

static int write_png(const char *filename, cvl_Mat *img) {
    const int height = img->height;
    const int width = img->width;
    const int channels = img->channels;
    const int stride = width * channels; // in bytes

    int ok = stbi_write_png(filename, width, height, channels, img->data, stride);

    return ok ? 0 : -1;
}

static int write_jpg(const char *filename, cvl_Mat *img) {
    const int height = img->height;
    const int width = img->width;
    const int channels = img->channels;

    int ok = stbi_write_jpg(filename, width, height, channels, img->data, 90);

    return ok ? 0 : -1;
}

// Reads an image from a specified file.
cvl_Mat cvl_imread(const char *filename) {
    cvl_Mat img = {0};

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return img;
    }

    int fmt = detect_format(f);

    switch (fmt) {
        case CVL_FMT_UNKNOWN: break;
        case CVL_FMT_PBM:
        case CVL_FMT_PGM:
        case CVL_FMT_PPM: img = read_pnm(f); break;
        case CVL_FMT_JPG:
        case CVL_FMT_PNG: img = read_stb(f); break;
    }

    fclose(f);

    return img;
}

// Saves an image to a specified file. The image format is determined by the file extension.
int cvl_imwrite(const char *filename, cvl_Mat *img) {
    if (!img || !img->data || img->height <= 0 || img->width <= 0 || img->channels <= 0) {
        return -1;
    }

    const char *ext = strrchr(filename, '.');
    assert(ext);

    if (strncmp(ext, ".pbm", 3) == 0) return write_pbm(filename, img);
    if (strncmp(ext, ".pgm", 3) == 0) return write_pgm(filename, img);
    if (strncmp(ext, ".ppm", 3) == 0) return write_ppm(filename, img);
    if (strncmp(ext, ".png", 3) == 0) return write_png(filename, img);
    if (strncmp(ext, ".jpg", 3) == 0) return write_jpg(filename, img);

    return -1;
}

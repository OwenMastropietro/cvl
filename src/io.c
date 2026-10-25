#include <cvl/io.h>

#include <stb_image.h>
#include <stb_image_write.h>

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static cvl_format_t detect_format(FILE *f) {
    if (!f) {
        return CVL_FMT_UNKNOWN;
    }

    unsigned char buf[8];

    long pos = ftell(f);
    if (pos < 0) {
        return CVL_FMT_UNKNOWN;
    }

    size_t n = fread(buf, 1, sizeof(buf), f);
    if (fseek(f, pos, SEEK_SET) != 0) {
        return CVL_FMT_UNKNOWN;
    }
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
    if (!f) {
        return cvl_mat_empty();
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
        return cvl_mat_empty();
    }

    cvl_Mat img = cvl_mat_create_from(height, width, channels, CVL_UINT8, data);

    stbi_image_free(data);

    return img;
}

static int pnm_token(FILE *f, char *out, size_t out_size) {
    int c;
    do {
        c = fgetc(f);
        if (c == '#') {
            while (c != '\n' && c != EOF) {
                c = fgetc(f);
            }
        }
    } while (c != EOF && isspace(c));

    if (c == EOF) {
        return 0;
    }

    size_t n = 0;
    while (c != EOF && !isspace(c)) {
        if (n + 1 < out_size) {
            out[n++] = (char)c;
        }
        c = fgetc(f);
    }
    out[n] = '\0';
    return n > 0;
}

static int pnm_int(FILE *f, int *out) {
    char tok[64];
    if (!pnm_token(f, tok, sizeof(tok))) {
        return 0;
    }
    char *end = NULL;
    long v = strtol(tok, &end, 10);
    if (end == tok || *end != '\0' || v > INT32_MAX) {
        return 0;
    }
    *out = (int)v;
    return 1;
}

static cvl_Mat read_pnm(FILE *f, cvl_format_t format) {
    if (!f) {
        return cvl_mat_empty();
    }

    char magic[8];
    if (!pnm_token(f, magic, sizeof(magic))) {
        return cvl_mat_empty();
    }
    if ((format == CVL_FMT_PBM && strcmp(magic, "P4") != 0) ||
        (format == CVL_FMT_PGM && strcmp(magic, "P5") != 0) ||
        (format == CVL_FMT_PPM && strcmp(magic, "P6") != 0)) {
        return cvl_mat_empty();
    }

    int width = 0;
    int height = 0;
    if (!pnm_int(f, &width) || !pnm_int(f, &height) || width <= 0 || height <= 0) {
        return cvl_mat_empty();
    }

    int channels = (format == CVL_FMT_PPM) ? 3 : 1;
    int maxval = 1;
    if (format != CVL_FMT_PBM) {
        if (!pnm_int(f, &maxval) || maxval <= 0 || maxval > 255) {
            return cvl_mat_empty();
        }
    }

    cvl_Mat img = cvl_mat_create(height, width, channels, CVL_UINT8);
    if (!img.data) {
        return img;
    }

    if (format == CVL_FMT_PGM || format == CVL_FMT_PPM) {
        size_t row_bytes = (size_t)width * (size_t)channels;
        for (int y = 0; y < height; ++y) {
            uint8_t *row = cvl_mat_row(&img, y);
            if (fread(row, 1, row_bytes, f) != row_bytes) {
                cvl_mat_free(&img);
                return cvl_mat_empty();
            }
            if (maxval != 255) {
                for (size_t i = 0; i < row_bytes; ++i) {
                    row[i] = (uint8_t)((int)row[i] * 255 / maxval);
                }
            }
        }
        return img;
    }

    size_t packed = ((size_t)width + 7) / 8;
    uint8_t *packed_row = malloc(packed);
    if (!packed_row) {
        cvl_mat_free(&img);
        return cvl_mat_empty();
    }

    for (int y = 0; y < height; ++y) {
        if (fread(packed_row, 1, packed, f) != packed) {
            free(packed_row);
            cvl_mat_free(&img);
            return cvl_mat_empty();
        }
        uint8_t *row = cvl_mat_row(&img, y);
        for (int x = 0; x < width; ++x) {
            int bit = (packed_row[x / 8] >> (7 - (x % 8))) & 1;
            row[x] = bit ? 0 : 255;
        }
    }
    free(packed_row);
    return img;
}

static int write_row(FILE *f, const void *row, size_t n) {
    return fwrite(row, 1, n, f) == n;
}

static int write_pbm(const char *filename, const cvl_Mat *img) {
    if (img->channels != 1 || img->depth != CVL_UINT8) {
        return -1;
    }

    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }

    if (fprintf(f, "P4\n%d %d\n", img->width, img->height) < 0) {
        return -1;
    }

    size_t rowbytes = ((size_t)img->width + 7) / 8;
    uint8_t *packed = calloc(rowbytes, 1);
    if (!packed) {
        return -1;
    }

    for (int y = 0; y < img->height; ++y) {
        const uint8_t *row = cvl_mat_row_const(img, y);
        memset(packed, 0, rowbytes);
        for (int x = 0; x < img->width; ++x) {
            uint8_t p = row[x];
            if (p != BLACK && p != WHITE) {
                free(packed);
                return -1;
            }
            if (p == BLACK) {
                packed[x / 8] |= (uint8_t)(0x80 >> (x % 8));
            }
        }
        if (!write_row(f, packed, rowbytes)) {
            free(packed);
            return -1;
        }
    }
    free(packed);

    if (fclose(f) != 0) {
        return -1;
    }

    return 0;
}

static int write_pgm(const char *filename, const cvl_Mat *img) {
    if (img->channels != 1 || img->depth != CVL_UINT8) {
        return -1;
    }

    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }

    if (fprintf(f, "P5\n%d %d\n255\n", img->width, img->height) < 0) {
        return -1;
    }

    for (int y = 0; y < img->height; ++y) {
        if (!write_row(f, cvl_mat_row_const(img, y), (size_t)img->width)) {
            return -1;
        }
    }

    if (fclose(f) != 0) {
        return -1;
    }

    return 0;
}

static int write_ppm(const char *filename, const cvl_Mat *img) {
    if (img->channels != 3 || img->depth != CVL_UINT8) {
        return -1;
    }

    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }

    if (fprintf(f, "P6\n%d %d\n255\n", img->width, img->height) < 0) {
        return -1;
    }

    size_t row_bytes = (size_t)img->width * 3;
    for (int y = 0; y < img->height; ++y) {
        if (!write_row(f, cvl_mat_row_const(img, y), row_bytes)) {
            return -1;
        }
    }

    if (fclose(f) != 0) {
        return -1;
    }

    return 0;
}

static int write_png(const char *filename, const cvl_Mat *img) {
    if (img->depth != CVL_UINT8 || img->channels < 1 || img->channels > 4) {
        return -1;
    }
    int ok = stbi_write_png(filename, img->width, img->height, img->channels, img->data, (int)img->stride);
    return ok ? 0 : -1;
}

static int write_jpg(const char *filename, const cvl_Mat *img) {
    if (img->depth != CVL_UINT8 || (img->channels != 1 && img->channels != 3)) {
        return -1;
    }
    if (img->stride != (size_t)img->width * (size_t)img->channels) {
        return -1;
    }
    int ok = stbi_write_jpg(filename, img->width, img->height, img->channels, img->data, 90);
    return ok ? 0 : -1;
}

// Reads an image from a specified file. Returns an empty matrix on failure.
cvl_Mat cvl_imread(const char *filename) {
    if (!filename) {
        return cvl_mat_empty();
    }

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return cvl_mat_empty();
    }

    cvl_format_t fmt = detect_format(f);
    cvl_Mat img = cvl_mat_empty();
    switch (fmt) {
        case CVL_FMT_PBM:
        case CVL_FMT_PGM:
        case CVL_FMT_PPM:
            img = read_pnm(f, fmt);
            break;
        case CVL_FMT_JPG:
        case CVL_FMT_PNG:
            img = read_stb(f);
            break;
        case CVL_FMT_UNKNOWN:
            break;
    }

    fclose(f);
    return img;
}

// Writes an image to a specified file. The image format is determined by the file extension.
int cvl_imwrite(const char *filename, cvl_Mat *img) {
    if (!filename || !img || !img->data || img->height <= 0 || img->width <= 0 || img->channels <= 0) {
        return -1;
    }
    if (img->stride < (size_t)img->width * (size_t)img->channels * cvl_elem_size((cvl_depth_t)img->depth)) {
        return -1;
    }

    const char *ext = strrchr(filename, '.');
    if (!ext) {
        return -1;
    }

    if (strncmp(ext, ".pbm", 3) == 0) return write_pbm(filename, img);
    if (strncmp(ext, ".pgm", 3) == 0) return write_pgm(filename, img);
    if (strncmp(ext, ".ppm", 3) == 0) return write_ppm(filename, img);
    if (strncmp(ext, ".png", 3) == 0) return write_png(filename, img);
    if (strncmp(ext, ".jpg", 3) == 0) return write_jpg(filename, img); // screw jpeg

    return -1;
}

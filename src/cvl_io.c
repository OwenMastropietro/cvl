#define _CRT_SECURE_NO_WARNINGS

#include "cvl_io.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int write_pbm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    assert(f);

    fprintf(f, "P4\n%d %d\n", img->width, img->height);
    
    int rowbytes = (img->width + 7) / 8;
    unsigned char *row = calloc(rowbytes, 1);
    assert(row);
    for (int i = 0; i < img->height; ++i) {
        memset(row, 0, rowbytes);
        for (int j = 0; j < img->width; ++j) {
            assert(img->map[i][j].i == BLACK || img->map[i][j].i == WHITE);

            if (img->map[i][j].i == BLACK) { // hawk tuah, BLACK is 0 fool
                row[j / 8] |= 0x80 >> (j % 8);
            }
        }
        fwrite(row, 1, rowbytes, f);
    }
    free(row);
    fclose(f);

    return 0;
}

static int write_pgm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    assert (f);

    fprintf(f, "P5\n%d %d\n255\n", img->width, img->height);

    unsigned char *row = malloc(img->width);
    assert(row);
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            row[j] = img->map[i][j].i;
        }
        fwrite(row, 1, img->width, f);
    }
    free(row);
    fclose(f);

    return 0;
}

static int write_ppm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    assert(f);

    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);

    unsigned char *row = malloc(3 * img->width);
    assert(row);
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

    return 0;
}

// Saves an image to a specified file.
// The image format is determined by the file extension.
int cvl_imwrite(const char *filename, Image *img) {
    assert(img->height > 0 && img->width > 0);

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

// Read an image from a file and allocate the required heap memory for it.
// Notice that only binary Netpbm files are supported. Regardless of the
// file type, all fields r, g, b, and i are filled in, with values from 0 to
// 255.
Image readImage(char *filename) {
    FILE *f;
    int i, j, width, height, imax = 0, bitsPerPixel, filesize, mapsize, mempos;
    char type[200], line[200];
    unsigned char *temp, output;
    Image img;
    Format filetype;

    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Can't open input file %s.\n", filename);
        exit(1);
    }

    fscanf(f, "%s", type);
    if (type[0] != 'P' || type[1] < '4' || type[1] > '6') {
        fprintf(stderr, "Error in %s: Only binary PBM, PGM, and PPM files are supported.\n", filename);
        exit(1);
    }
    switch (type[1]) {
    case '4':
        filetype = PBM;
        bitsPerPixel = 1;
        break;
    case '5':
        filetype = PGM;
        bitsPerPixel = 8;
        break;
    default:
        filetype = PPM;
        bitsPerPixel = 24;
    }

    line[0] = '#';
    while (line[0] == '#' || line[0] == 10 || line[0] == 13) {
        fgets(line, 200, f);
    }
    sscanf(line, "%d %d", &width, &height);
    if (filetype != PBM) {
        fgets(line, 200, f);
        sscanf(line, "%d", &imax);
    }
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Invalid image size in input file %s.\n", filename);
        exit(1);
    }

    // Notice: In PBM files, every row starts with a new byte.
    mapsize = (bitsPerPixel * width + 7) / 8 * height;
    temp = (unsigned char *)malloc(mapsize);
    filesize = (int)fread((void *)temp, 1, mapsize, f);
    fclose(f);
    if (filesize != mapsize) {
        fprintf(stderr, "Data missing in file %s.\n", filename);
        exit(1);
    }

    img = createImage(height, width);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            mempos = (
				(bitsPerPixel * width + 7) / 8 * i
				+ (bitsPerPixel * j) / 8
			);
            switch (filetype) {
            case PBM:
                output = 255 * (((temp[mempos] & (128 >> j % 8)) == 0));
                img.map[i][j].r = output;
                img.map[i][j].g = output;
                img.map[i][j].b = output;
                img.map[i][j].i = output;
                break;
            case PGM:
                output = (unsigned char)((int)temp[mempos] * 255 / imax);
                img.map[i][j].r = output;
                img.map[i][j].g = output;
                img.map[i][j].b = output;
                img.map[i][j].i = output;
                break;
            case PPM:
                img.map[i][j].r = (unsigned char)((int)temp[mempos] * 255 / imax);
                img.map[i][j].g = (unsigned char)((int)temp[mempos + 1] * 255 / imax);
                img.map[i][j].b = (unsigned char)((int)temp[mempos + 2] * 255 / imax);
                img.map[i][j].i = (unsigned char)(
					((int)temp[mempos] + (int)temp[mempos + 1] + (int)temp[mempos + 2]) * 255
					/ (3 * imax)
				);
            }
        }
    }
	free(temp);

	return img;
}

// Write an image to a file. The file format (binary PBM, PGM, or PPM) is
// automatically chosen based on the given file name. For PBM and PGM files,
// only the intensity (i) information is used, and for PPM files, only r, g, and
// b are relevant.
void writeImage(Image img, char *filename) {
    Format filetype;
    FILE *f;
    int i, j, mapsize, bitsPerPixel, bitsum, mempos;
    unsigned char *temp;

    switch (filename[strlen(filename) - 2]) {
    case 'b':
    case 'B':
        filetype = PBM;
        bitsPerPixel = 1;
        break;
    case 'g':
    case 'G':
        filetype = PGM;
        bitsPerPixel = 8;
        break;
    case 'p':
    case 'P':
        filetype = PPM;
        bitsPerPixel = 24;
        break;
    default:
        fprintf(stderr, "Invalid output file name: %s.\n", filename);
        exit(1);
    }

    if (img.width <= 0 || img.height <= 0) {
        fprintf(stderr, "Invalid image size in output file %s.\n", filename);
        exit(1);
    }

    // Notice: In PBM files, every row starts with a new byte.
    mapsize = (bitsPerPixel * img.width + 7) / 8 * img.height;
    temp = (unsigned char *)malloc(mapsize);
    mempos = 0;
    bitsum = 0;
    for (i = 0; i < img.height; i++) {
        for (j = 0; j < img.width; j++) {
            switch (filetype) {
            case PBM:
                if (img.map[i][j].i < 128)
                    bitsum += 128 >> (j % 8);
                if (j % 8 == 7 || j == img.width - 1) {
                    temp[mempos++] = (unsigned char)bitsum;
                    bitsum = 0;
                }
                break;
            case PGM:
                temp[mempos++] = img.map[i][j].i;
                break;
            case PPM:
                temp[mempos++] = img.map[i][j].r;
                temp[mempos++] = img.map[i][j].g;
                temp[mempos++] = img.map[i][j].b;
            }
        }
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Can't open output file %s.\n", filename);
        exit(1);
    }
    switch (filetype) {
    case PBM:
        fprintf(f, "P4\n# Created by netpbm.c\n%d %d\n", img.width, img.height);
        break;
    case PGM:
        fprintf(f, "P5\n# Created by netpbm.c\n%d %d\n255\n", img.width, img.height);
        break;
    case PPM:
        fprintf(f, "P6\n# Created by netpbm.c\n%d %d\n255\n", img.width, img.height);
    }
    fwrite((void *)temp, 1, mapsize, f);
    fclose(f);
    free(temp);
}

#include <cvl/cvl.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    BLUR_MEAN,
    BLUR_GAUSS,
    BLUR_MEDIAN,
} blur_t;

static blur_t parse_type(const char *s) {
    if (!s) return BLUR_MEAN;

    if (strncmp(s, "mean", 4) == 0)   return BLUR_MEAN;
    if (strncmp(s, "gauss", 5) == 0)  return BLUR_GAUSS;
    if (strncmp(s, "median", 6) == 0) return BLUR_MEDIAN;

    fprintf(stderr, "Unknown blur type.\n");

    return BLUR_MEAN;
}

static int parse_ksize(const char *s) {
    if (!s) return 3;

    return atoi(s);
}

int cmd_blur(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: cvl blur <input> <output> [--type mean|gauss|median] [--ksize N]\n");
        return 1;
    }

    // Parse.
    const char *infile  = argv[2];
    const char *outfile = argv[3];
    
    blur_t type = BLUR_MEAN;
    int ksize = 3;

    for (int i = 4; i < argc; i++) {
        if (strncmp(argv[i], "--type", 6) == 0 && i + 1 < argc) {
            type = parse_type(argv[++i]);
        }
        else if (strncmp(argv[i], "--ksize", 7) == 0 && i + 1 < argc) {
            ksize = parse_ksize(argv[++i]);
        }
        else {
            fprintf(stderr, "Unknown arg.\n");
            return 1;
        }
    }

    // Read.
    cvl_Mat img = cvl_imread(infile);
    if (!img.data) {
        fprintf(stderr, "bad read\n");
        return 1;
    }

    // Blur.
    cvl_Mat res = {0};
    switch (type) {
        case BLUR_MEAN:   res = cvl_blur_mean_new(&img, ksize);       break;
        case BLUR_GAUSS:  res = cvl_blur_gauss_new(&img, ksize, 0.0); break; // todo: sigma
        case BLUR_MEDIAN: res = cvl_blur_median_new(&img, ksize);     break;
    }

    // Write.
    int err = cvl_imwrite(outfile, &res);
    if (err) {
        fprintf(stderr, "bad write\n");
    }

    // Free.
    cvl_mat_free(&img);
    cvl_mat_free(&res);

    return err;
}

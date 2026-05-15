#include <stdio.h>
#include <string.h>

static void usage(void) {
    printf("Usage: cvl <command> [options]\n");

    printf("    cvl blur <input> <output> [--type mean|gauss|median] [--ksize N]\n");
}

int cmd_blur(int argc, char **argv);

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
        return 1;
    }

    const char *cmd = argv[1];

    if (strncmp(cmd, "info", 4) == 0) {
        printf("info command\n");
        return 0;
    }

    if (strncmp(cmd, "blur", 4) == 0) {
        return cmd_blur(argc, argv);
    }

    if (strncmp(cmd, "threshold", 9) == 0) {
        printf("threshold command\n");
        return 0;
    }

    fprintf(stderr, "Uknown command.\n");

    return 1;
}

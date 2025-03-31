#define _XOPEN_SOURCE 500 // Enable mknod() declaration [[1]]
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Required for mknod()
#include <sys/types.h>
#include <sys/sysmacros.h> // Required for makedev()
#include <unistd.h> // Required for getopt(), optarg, optind
#include <fcntl.h>
#include <errno.h>

void usage() {
    fprintf(stderr, "Usage: mknod [-m MODE] NAME TYPE [MAJOR MINOR]\n");
    fprintf(stderr, "  TYPE:\n");
    fprintf(stderr, "    p   create a FIFO (named pipe)\n");
    fprintf(stderr, "    b   create a block device\n");
    fprintf(stderr, "    c   create a character device\n");
    exit(1);
}

int main(int argc, char **argv) {
    mode_t mode = 0666; // Default permissions
    char type = '\0';
    dev_t dev = 0;

    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
            case 'm':
                mode = strtol(optarg, NULL, 8); // Parse mode in octal
                break;
            default:
                usage();
        }
    }

    // Ensure correct number of arguments
    if (optind + 2 > argc || optind + 3 < argc) {
        usage();
    }

    const char *name = argv[optind];
    type = argv[optind + 1][0];

    // Parse major and minor numbers if provided
    if (type == 'b' || type == 'c') {
        if (optind + 3 != argc) {
            usage();
        }
        int major = atoi(argv[optind + 2]);
        int minor = atoi(argv[optind + 3]);
        dev = makedev(major, minor);
    } else if (type != 'p') {
        usage();
    }

    // Create the special file
    if (mknod(name, (type == 'p' ? S_IFIFO : (type == 'b' ? S_IFBLK : S_IFCHR)) | mode, dev) == -1) {
        perror("mknod");
        return 1;
    }

    return 0;
}

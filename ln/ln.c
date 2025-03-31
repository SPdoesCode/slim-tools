#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for link() and symlink()
#include <errno.h>

void usage() {
    fprintf(stderr, "Usage: ln [-s] TARGET LINK_NAME\n");
    exit(1);
}

int main(int argc, char **argv) {
    int opt;
    int symbolic = 0; // Default: create a hard link

    // Parse options
    while ((opt = getopt(argc, argv, "s")) != -1) {
        switch (opt) {
            case 's':
                symbolic = 1; // Create a symbolic link
                break;
            default:
                usage();
        }
    }

    // Ensure we have exactly two arguments: TARGET and LINK_NAME
    if (optind + 2 != argc) {
        usage();
    }

    const char *target = argv[optind];
    const char *link_name = argv[optind + 1];

    // Create the link
    if (symbolic) {
        // Create a symbolic link
        if (symlink(target, link_name) == -1) {
            perror("ln: symlink");
            return 1;
        }
    } else {
        // Create a hard link
        if (link(target, link_name) == -1) {
            perror("ln: link");
            return 1;
        }
    }

    return 0;
}

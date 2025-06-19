#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for link() and unlink()
#include <errno.h>

void usage() {
    printf("link from SlimTools\n");
    fprintf(stderr, "Usage: link TARGET LINK_NAME\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
    }

    const char *target = argv[1];
    const char *link_name = argv[2];

    // Create a hard link
    if (link(target, link_name) == -1) {
        perror("link");
        return 1;
    }

    return 0;
}

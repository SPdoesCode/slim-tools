#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Required for mkfifo()
#include <sys/types.h>
#include <errno.h>

void usage() {
    fprintf(stderr, "Usage: mkfifo NAME...\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    int ret = 0;

    // Create each FIFO
    for (int i = 1; i < argc; i++) {
        const char *name = argv[i];

        // Create the FIFO with default permissions (0666)
        if (mkfifo(name, 0666) == -1) {
            perror("mkfifo");
            ret = 1;
        }
    }

    return ret;
}

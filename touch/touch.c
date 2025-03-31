#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>

void usage() {
    fprintf(stderr, "Usage: touch FILE...\n");
    fprintf(stderr, "Update the access and modification times of FILE(s).\n");
    fprintf(stderr, "If FILE does not exist, create an empty file.\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];

        // Check if the file exists
        struct stat sb;
        if (stat(filename, &sb) == 0) {
            // File exists; update its access and modification times
            if (utime(filename, NULL) == -1) {
                perror("touch: utime");
                return 1;
            }
        } else {
            // File does not exist; create an empty file
            int fd = open(filename, O_CREAT | O_WRONLY, 0644);
            if (fd == -1) {
                perror("touch: open");
                return 1;
            }
            close(fd);
        }
    }

    return 0;
}

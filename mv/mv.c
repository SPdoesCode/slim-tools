#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Required for stat()
#include <unistd.h>   // Required for unlink(), rmdir(), etc.
#include <errno.h>
#include <string.h>
#include <limits.h>   // Required for PATH_MAX [[1]]

void usage() {
    fprintf(stderr, "Usage: mv SOURCE DEST\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
    }

    const char *source = argv[1];
    const char *dest = argv[2];

    struct stat st;

    // Check if the source exists
    if (stat(source, &st) == -1) {
        perror("mv: stat");
        return 1;
    }

    // Check if the destination exists
    if (stat(dest, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            // If the destination is a directory, move the source into it
            char new_path[PATH_MAX]; // Declare new_path here [[1]]
            snprintf(new_path, sizeof(new_path), "%s/%s", dest, source);

            if (rename(source, new_path) == -1) {
                perror("mv: rename");
                return 1;
            }
        } else {
            // If the destination is a file, overwrite it
            if (unlink(dest) == -1 && errno != ENOENT) {
                perror("mv: unlink");
                return 1;
            }

            if (rename(source, dest) == -1) {
                perror("mv: rename");
                return 1;
            }
        }
    } else {
        // If the destination does not exist, simply rename/move the source
        if (rename(source, dest) == -1) {
            perror("mv: rename");
            return 1;
        }
    }

    return 0;
}

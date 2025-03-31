#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // Required for mkdir()
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h> // Required for getopt() and optind [[1]]

void usage() {
    fprintf(stderr, "Usage: mkdir [-p] DIRECTORY...\n");
    exit(1);
}

// Function to create a directory with error handling
int create_directory(const char *path, int parents) {
    struct stat st;

    // Check if the directory already exists
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            return 0; // Directory already exists
        } else {
            fprintf(stderr, "mkdir: cannot create directory '%s': File exists\n", path);
            return 1;
        }
    }

    // If -p is specified, create parent directories recursively
    if (parents) {
        char *parent = strdup(path);
        char *slash = strrchr(parent, '/');
        if (slash && slash != parent) {
            *slash = '\0'; // Truncate to parent directory
            if (stat(parent, &st) != 0) {
                create_directory(parent, 1); // Recursively create parent
            }
        }
        free(parent);
    }

    // Create the directory
    if (mkdir(path, 0777) == -1) { // 0777 is the default mode
        perror("mkdir");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int parents = 0; // -p option

    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                parents = 1;
                break;
            default:
                usage();
        }
    }

    // Ensure at least one directory argument is provided
    if (optind >= argc) {
        usage();
    }

    // Create each directory
    int ret = 0;
    for (int i = optind; i < argc; i++) {
        ret |= create_directory(argv[i], parents);
    }

    return ret;
}

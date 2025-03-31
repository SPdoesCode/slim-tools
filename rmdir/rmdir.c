#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for rmdir()
#include <sys/stat.h> // Required for stat()
#include <limits.h> // Required for PATH_MAX

void usage() {
    fprintf(stderr, "Usage: rmdir [-p] DIRECTORY...\n");
    fprintf(stderr, "  -p   Remove parent directories as well if they become empty\n");
    exit(1);
}

int remove_directory(const char *path) {
    struct stat st;

    // Check if the path exists
    if (stat(path, &st) == -1) {
        perror("rmdir: stat");
        return 1;
    }

    // Check if it's not a directory
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "rmdir: failed to remove '%s': Not a directory\n", path);
        return 1;
    }

    // Remove the directory
    if (rmdir(path) == -1) {
        perror("rmdir: rmdir");
        return 1;
    }

    return 0;
}

int remove_parent_directories(const char *path) {
    char current_path[PATH_MAX];
    strncpy(current_path, path, sizeof(current_path) - 1);
    current_path[sizeof(current_path) - 1] = '\0'; // Ensure null-termination

    while (1) {
        // Remove the current directory
        if (remove_directory(current_path) != 0) {
            break; // Stop if removal fails
        }

        // Move up to the parent directory
        char *last_slash = strrchr(current_path, '/');
        if (!last_slash || last_slash == current_path) {
            break; // Stop at root or invalid path
        }
        *last_slash = '\0'; // Truncate the path
    }

    return 0;
}

int main(int argc, char **argv) {
    int remove_parents = 0; // Default: Do not remove parent directories
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                remove_parents = 1;
                break;
            default:
                usage();
        }
    }

    // Ensure at least one directory is provided
    if (optind >= argc) {
        usage();
    }

    int ret = 0;

    // Remove each directory
    for (int i = optind; i < argc; i++) {
        const char *path = argv[i];

        if (remove_parents) {
            ret |= remove_parent_directories(path);
        } else {
            ret |= remove_directory(path);
        }
    }

    return ret;
}

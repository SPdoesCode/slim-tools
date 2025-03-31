#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for unlink() and rmdir()
#include <sys/stat.h> // Required for stat()
#include <dirent.h> // Required for opendir(), readdir(), closedir(), and struct dirent
#include <limits.h> // Required for PATH_MAX

void usage() {
    fprintf(stderr, "Usage: rm [-r] FILE...\n");
    fprintf(stderr, "  -r   Remove directories and their contents recursively\n");
    exit(1);
}

int remove_file(const char *path) {
    struct stat st;

    // Check if the path exists
    if (stat(path, &st) == -1) {
        perror("rm: stat");
        return 1;
    }

    // Check if it's a directory
    if (S_ISDIR(st.st_mode)) {
        fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", path);
        return 1;
    }

    // Remove the file
    if (unlink(path) == -1) {
        perror("rm: unlink");
        return 1;
    }

    return 0;
}

int remove_directory(const char *path) {
    struct stat st;

    // Check if the path exists
    if (stat(path, &st) == -1) {
        perror("rm: stat");
        return 1;
    }

    // Check if it's not a directory
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "rm: cannot remove '%s': Not a directory\n", path);
        return 1;
    }

    // Remove the directory
    if (rmdir(path) == -1) {
        perror("rm: rmdir");
        return 1;
    }

    return 0;
}

int remove_recursive(const char *path) {
    struct stat st;

    // Check if the path exists
    if (stat(path, &st) == -1) {
        perror("rm: stat");
        return 1;
    }

    // If it's a directory, remove its contents first
    if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) {
            perror("rm: opendir");
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue; // Skip "." and ".."
            }

            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            if (remove_recursive(full_path) != 0) {
                closedir(dir);
                return 1;
            }
        }

        closedir(dir);

        // Remove the now-empty directory
        if (rmdir(path) == -1) {
            perror("rm: rmdir");
            return 1;
        }
    } else {
        // Remove the file
        if (unlink(path) == -1) {
            perror("rm: unlink");
            return 1;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    int recursive = 0; // Default: Non-recursive removal
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "r")) != -1) {
        switch (opt) {
            case 'r':
                recursive = 1;
                break;
            default:
                usage();
        }
    }

    // Ensure at least one file or directory is provided
    if (optind >= argc) {
        usage();
    }

    int ret = 0;

    // Remove each file or directory
    for (int i = optind; i < argc; i++) {
        const char *path = argv[i];

        if (recursive) {
            ret |= remove_recursive(path);
        } else {
            ret |= remove_file(path);
        }
    }

    return ret;
}

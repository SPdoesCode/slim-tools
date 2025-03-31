#define _POSIX_C_SOURCE 200809L // Enable POSIX features, including lstat()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // For lstat()
#include <dirent.h>
#include <unistd.h>

#define MAX_PATH 4096

// Function to calculate the size of a file or directory
long calculate_size(const char *path, int summarize, int human_readable) {
    struct stat st;
    if (lstat(path, &st) < 0) {
        perror("du: lstat");
        return 0;
    }

    // If it's a regular file, return its size
    if (!S_ISDIR(st.st_mode)) {
        return st.st_blocks / 2; // Size in kilobytes
    }

    // If it's a directory, recursively calculate the size
    DIR *dir = opendir(path);
    if (!dir) {
        perror("du: opendir");
        return 0;
    }

    long total_size = st.st_blocks / 2; // Start with the directory's own size
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Recursively calculate the size of the entry
        total_size += calculate_size(full_path, summarize, human_readable);
    }

    closedir(dir);

    // Print the size if not summarizing
    if (!summarize) {
        if (human_readable) {
            printf("%ldK\t%s\n", total_size, path);
        } else {
            printf("%ld\t%s\n", total_size, path);
        }
    }

    return total_size;
}

int main(int argc, char **argv) {
    int summarize = 0;       // -s flag: summarize
    int human_readable = 0;  // -h flag: human-readable
    const char *path = ".";  // Default path is current directory

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            summarize = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            human_readable = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "du: invalid option '%s'\n", argv[i]);
            return 1;
        } else {
            path = argv[i];
        }
    }

    // Calculate and display disk usage
    long size = calculate_size(path, summarize, human_readable);

    // Print summary if requested
    if (summarize) {
        if (human_readable) {
            printf("%ldK\t%s\n", size, path);
        } else {
            printf("%ld\t%s\n", size, path);
        }
    }

    return 0;
}

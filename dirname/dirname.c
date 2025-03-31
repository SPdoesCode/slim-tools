#define _POSIX_C_SOURCE 200809L // Enable POSIX features, including strdup()
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strdup()
#include <ctype.h>

// Function to extract the directory portion of a path
void dirname(const char *path) {
    // Handle special cases
    if (strcmp(path, "/") == 0 || strcmp(path, ".") == 0 || strcmp(path, "..") == 0) {
        printf("%s\n", path);
        return;
    }

    // Duplicate the path to avoid modifying the original
    char *copy = strdup(path);
    if (!copy) {
        perror("dirname: strdup");
        exit(1);
    }

    // Remove trailing slashes
    size_t len = strlen(copy);
    while (len > 1 && copy[len - 1] == '/') {
        copy[--len] = '\0';
    }

    // Find the last slash
    char *last_slash = strrchr(copy, '/');
    if (!last_slash) {
        // No slashes found, return "."
        printf(".\n");
    } else if (last_slash == copy) {
        // Path starts with a single slash, return "/"
        printf("/\n");
    } else {
        // Null-terminate at the last slash and print
        *last_slash = '\0';
        printf("%s\n", copy);
    }

    free(copy);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: dirname PATH\n");
        return 1;
    }

    // Call the dirname function
    dirname(argv[1]);

    return 0;
}

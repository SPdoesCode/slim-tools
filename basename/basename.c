#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: basename NAME [SUFFIX]\n");
        return 1;
    }

    char *name = argv[1];
    char *suffix = argv[2];

    // Find the last '/' in the path
    char *base = strrchr(name, '/');
    if (base) {
        base++; // Move past the last '/'
    } else {
        base = name; // No '/' found, use the whole string
    }

    // Remove the suffix if provided and matches
    if (suffix) {
        size_t base_len = strlen(base);
        size_t suffix_len = strlen(suffix);
        if (base_len > suffix_len && strcmp(base + base_len - suffix_len, suffix) == 0) {
            base[base_len - suffix_len] = '\0';
        }
    }

    printf("%s\n", base);
    return 0;
}

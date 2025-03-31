#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void usage() {
    fprintf(stderr, "Usage: tr SET1 [SET2]\n");
    fprintf(stderr, "Translate or delete characters.\n");
    fprintf(stderr, "  -d   Delete characters in SET1\n");
    exit(1);
}

// Function to perform character translation
void translate(const char *set1, const char *set2) {
    int map[256];
    int set2_len = strlen(set2);

    // Initialize the translation map
    for (int i = 0; i < 256; i++) {
        map[i] = i;
    }

    // Build the translation map
    for (int i = 0; set1[i]; i++) {
        if (i < set2_len) {
            map[(unsigned char)set1[i]] = set2[i];
        } else {
            map[(unsigned char)set1[i]] = set2[set2_len - 1];
        }
    }

    // Translate characters from stdin to stdout
    int c;
    while ((c = getchar()) != EOF) {
        putchar(map[c]);
    }
}

// Function to delete characters
void delete_chars(const char *set1) {
    int delete_map[256] = {0};

    // Mark characters to be deleted
    for (int i = 0; set1[i]; i++) {
        delete_map[(unsigned char)set1[i]] = 1;
    }

    // Delete characters from stdin to stdout
    int c;
    while ((c = getchar()) != EOF) {
        if (!delete_map[c]) {
            putchar(c);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        usage();
    }

    int delete_mode = 0;

    // Check for delete option
    if (strcmp(argv[1], "-d") == 0) {
        delete_mode = 1;
        if (argc != 3) {
            usage();
        }
    } else if (argc != 3) {
        usage();
    }

    const char *set1 = argv[delete_mode ? 2 : 1];
    const char *set2 = delete_mode ? NULL : argv[2];

    if (delete_mode) {
        delete_chars(set1);
    } else {
        translate(set1, set2);
    }

    return 0;
}

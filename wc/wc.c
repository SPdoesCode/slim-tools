#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Required for isspace()

void usage() {
    fprintf(stderr, "Usage: wc [OPTION]... [FILE]...\n");
    fprintf(stderr, "Count lines, words, and characters.\n");
    fprintf(stderr, "With no FILE, read from standard input.\n");
    exit(1);
}

typedef struct {
    long lines;
    long words;
    long chars;
} Counts;

// Function to count lines, words, and characters
Counts count(FILE *file) {
    Counts counts = {0, 0, 0};
    int c;
    int in_word = 0; // Flag to track if we're inside a word

    while ((c = fgetc(file)) != EOF) {
        counts.chars++; // Count every character

        if (c == '\n') {
            counts.lines++;
        }

        if (isspace(c)) {
            in_word = 0; // End of a word
        } else {
            if (!in_word) {
                counts.words++; // Start of a new word
                in_word = 1;
            }
        }
    }

    return counts;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        usage();
    }

    Counts total_counts = {0, 0, 0};

    if (argc == 1) {
        // No files provided; read from stdin
        Counts counts = count(stdin);
        printf("%7ld %7ld %7ld\n", counts.lines, counts.words, counts.chars);
    } else {
        const char *filename = argv[1];
        FILE *file = fopen(filename, "r");
        if (!file) {
            perror("wc: fopen");
            return 1;
        }

        Counts counts = count(file);
        fclose(file);

        printf("%7ld %7ld %7ld %s\n", counts.lines, counts.words, counts.chars, filename);

        total_counts.lines += counts.lines;
        total_counts.words += counts.words;
        total_counts.chars += counts.chars;
    }

    return 0;
}

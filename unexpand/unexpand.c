#define _POSIX_C_SOURCE 200809L // Required for getopt(), optarg, and optind
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: unexpand [OPTION]... [FILE]...\n");
    fprintf(stderr, "Convert spaces to tabs.\n");
    fprintf(stderr, "  -t NUM   Use NUM spaces per tab (default is 8).\n");
    fprintf(stderr, "With no FILE, read from standard input.\n");
    exit(1);
}

// Function to unexpand spaces into tabs
void unexpand(FILE *file, int tab_size) {
    int c;
    int column = 0; // Current column position
    int space_count = 0; // Number of consecutive spaces

    while ((c = fgetc(file)) != EOF) {
        if (c == ' ') {
            // Count consecutive spaces
            space_count++;
            column++;

            // If spaces align with a tab stop, replace them with a tab
            if (column % tab_size == 0) {
                putchar('\t');
                space_count = 0;
            }
        } else {
            // Output any remaining spaces before the current character
            for (int i = 0; i < space_count; i++) {
                putchar(' ');
            }
            space_count = 0;

            // Output the current character
            putchar(c);

            // Update column position
            if (c == '\n') {
                column = 0; // Reset column on newline
            } else {
                column++;
            }
        }
    }

    // Output any remaining spaces at the end of the file
    for (int i = 0; i < space_count; i++) {
        putchar(' ');
    }
}

int main(int argc, char **argv) {
    int opt;
    int tab_size = 8; // Default tab size

    // Parse options
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't':
                tab_size = atoi(optarg);
                if (tab_size <= 0) {
                    fprintf(stderr, "unexpand: invalid tab size '%s'\n", optarg);
                    usage();
                }
                break;
            default:
                usage();
        }
    }

    // Process files or stdin
    if (optind >= argc) {
        // No files provided; read from stdin
        unexpand(stdin, tab_size);
    } else {
        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];
            FILE *file = fopen(filename, "r");
            if (!file) {
                perror("unexpand: fopen");
                return 1;
            }

            unexpand(file, tab_size);
            fclose(file);
        }
    }

    return 0;
}

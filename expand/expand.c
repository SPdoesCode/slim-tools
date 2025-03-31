#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_TABSTOP 8 // Default tab stop width

// Function to expand tabs into spaces
void expand_tabs(FILE *input, int tabstop) {
    int c;
    int column = 0; // Tracks the current column position

    while ((c = fgetc(input)) != EOF) {
        if (c == '\t') {
            // Calculate the number of spaces needed to reach the next tab stop
            int spaces = tabstop - (column % tabstop);
            for (int i = 0; i < spaces; i++) {
                putchar(' ');
            }
            column += spaces; // Update the column position
        } else if (c == '\n') {
            // Reset column position on a newline
            putchar(c);
            column = 0;
        } else {
            // Print the character and update the column position
            putchar(c);
            column++;
        }
    }
}

int main(int argc, char **argv) {
    int tabstop = DEFAULT_TABSTOP; // Default tab stop width
    int file_index = 1;

    // Parse options
    if (argc > 1 && argv[1][0] == '-') {
        if (argv[1][1] == 't' && argv[1][2] != '\0') {
            // Parse the tab stop value from the -t option
            tabstop = atoi(argv[1] + 2);
            if (tabstop <= 0) {
                fprintf(stderr, "expand: invalid tab stop value '%s'\n", argv[1] + 2);
                return 1;
            }
            file_index = 2; // Skip the -t option
        } else {
            fprintf(stderr, "expand: invalid option '%s'\n", argv[1]);
            return 1;
        }
    }

    // Process files or stdin
    if (file_index >= argc) {
        // No files provided, process stdin
        expand_tabs(stdin, tabstop);
    } else {
        // Process each file
        for (int i = file_index; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("expand: fopen");
                return 1;
            }
            expand_tabs(file, tabstop);
            fclose(file);
        }
    }

    return 0;
}

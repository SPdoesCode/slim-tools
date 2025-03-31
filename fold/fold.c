#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH 80 // Default line width

// Function to fold text
void fold(FILE *input, int width) {
    char buffer[1024]; // Buffer to hold input lines
    char output[1024]; // Buffer to hold folded output
    int out_len = 0;   // Length of current output line

    while (fgets(buffer, sizeof(buffer), input)) {
        char *line = buffer;

        while (*line) {
            // If adding the next character would exceed the width, flush the current line
            if (out_len + 1 > width) {
                printf("%s\n", output);
                out_len = 0;
                output[0] = '\0';
            }

            // Add the character to the output buffer
            output[out_len++] = *line;
            output[out_len] = '\0';

            // Move to the next character
            line++;
        }
    }

    // Print any remaining content in the output buffer
    if (out_len > 0) {
        printf("%s\n", output);
    }
}

int main(int argc, char **argv) {
    int width = DEFAULT_WIDTH; // Default line width
    int file_index = 1;

    // Parse options
    if (argc > 1 && argv[1][0] == '-') {
        if (argv[1][1] == 'w' && argv[1][2] != '\0') {
            // Parse the width value from the -w option
            width = atoi(argv[1] + 2);
            if (width <= 0) {
                fprintf(stderr, "fold: invalid width '%s'\n", argv[1] + 2);
                return 1;
            }
            file_index = 2; // Skip the -w option
        } else {
            fprintf(stderr, "fold: invalid option '%s'\n", argv[1]);
            return 1;
        }
    }

    // Process files or stdin
    if (file_index >= argc) {
        // No files provided, process stdin
        fold(stdin, width);
    } else {
        // Process each file
        for (int i = file_index; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("fold: fopen");
                return 1;
            }
            fold(file, width);
            fclose(file);
        }
    }

    return 0;
}

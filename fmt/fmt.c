#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WIDTH 75 // Default line width

// Function to format text
void fmt(FILE *input, int width) {
    char buffer[1024]; // Buffer to hold input lines
    char output[1024]; // Buffer to hold formatted output
    int out_len = 0;   // Length of current output line

    while (fgets(buffer, sizeof(buffer), input)) {
        char *word = strtok(buffer, " \t\n"); // Tokenize input into words

        while (word) {
            int word_len = strlen(word);

            // If adding the word exceeds the width, flush the current line
            if (out_len + word_len + (out_len > 0 ? 1 : 0) > width) {
                printf("%s\n", output);
                out_len = 0;
                output[0] = '\0';
            }

            // Add a space before the word if it's not the first word
            if (out_len > 0) {
                strcat(output, " ");
                out_len++;
            }

            // Add the word to the output buffer
            strcat(output, word);
            out_len += word_len;

            // Get the next word
            word = strtok(NULL, " \t\n");
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
                fprintf(stderr, "fmt: invalid width '%s'\n", argv[1] + 2);
                return 1;
            }
            file_index = 2; // Skip the -w option
        } else {
            fprintf(stderr, "fmt: invalid option '%s'\n", argv[1]);
            return 1;
        }
    }

    // Process files or stdin
    if (file_index >= argc) {
        // No files provided, process stdin
        fmt(stdin, width);
    } else {
        // Process each file
        for (int i = file_index; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("fmt: fopen");
                return 1;
            }
            fmt(file, width);
            fclose(file);
        }
    }

    return 0;
}

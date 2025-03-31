#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINES 10 // Default number of lines to display

// Function to print the first `n` lines of a file
void print_head(FILE *file, int n) {
    char buffer[1024]; // Buffer to hold each line
    int line_count = 0;

    while (line_count < n && fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
        line_count++;
    }
}

int main(int argc, char **argv) {
    int num_lines = DEFAULT_LINES; // Default number of lines
    int file_index = 1;

    // Parse options
    if (argc > 1 && argv[1][0] == '-') {
        if (argv[1][1] == 'n' && argv[1][2] != '\0') {
            // Parse the number of lines from the -n option
            num_lines = atoi(argv[1] + 2);
            if (num_lines <= 0) {
                fprintf(stderr, "head: invalid number of lines '%s'\n", argv[1] + 2);
                return 1;
            }
            file_index = 2; // Skip the -n option
        } else {
            fprintf(stderr, "head: invalid option '%s'\n", argv[1]);
            return 1;
        }
    }

    // Process files or stdin
    if (file_index >= argc) {
        // No files provided, process stdin
        print_head(stdin, num_lines);
    } else {
        // Process each file
        for (int i = file_index; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("head: fopen");
                return 1;
            }

            // Print file name if multiple files are provided
            if (argc - file_index > 1) {
                printf("==> %s <==\n", argv[i]);
            }

            print_head(file, num_lines);
            fclose(file);

            // Add a blank line between outputs if multiple files are processed
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }

    return 0;
}

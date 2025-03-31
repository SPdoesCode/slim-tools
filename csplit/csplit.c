#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 4096

// Function to split the file
void csplit(const char *filename, const char *pattern, int repeat) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("csplit: open");
        exit(1);
    }

    char line[MAX_LINE];
    int part = 0;
    FILE *output = NULL;

    while (fgets(line, sizeof(line), file)) {
        // Check if the current line matches the pattern
        if (strstr(line, pattern)) {
            // Close the previous output file if it exists
            if (output) {
                fclose(output);
                output = NULL;
            }

            // Create a new output file
            char outfile[32];
            snprintf(outfile, sizeof(outfile), "xx%02d", part++);
            output = fopen(outfile, "w");
            if (!output) {
                perror("csplit: fopen");
                fclose(file);
                exit(1);
            }
        }

        // Write the line to the current output file
        if (output) {
            fputs(line, output);
        }
    }

    // Close the last output file if it exists
    if (output) {
        fclose(output);
    }

    fclose(file);

    // If no files were created, exit with an error
    if (part == 0) {
        fprintf(stderr, "csplit: no match found for pattern '%s'\n", pattern);
        exit(1);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: csplit FILE PATTERN\n");
        printf("Splits FILE into sections based on PATTERN.\n");
        return 1;
    }

    const char *filename = argv[1];
    const char *pattern = argv[2];

    // Call the csplit function
    csplit(filename, pattern, 0);

    return 0;
}

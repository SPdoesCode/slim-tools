#define _POSIX_C_SOURCE 200809L // Required for getopt() and optind
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: uniq [OPTION]... [INPUT [OUTPUT]]\n");
    fprintf(stderr, "Filter out repeated lines.\n");
    fprintf(stderr, "  -c   Prefix lines with the count of occurrences\n");
    fprintf(stderr, "With no INPUT, read from standard input.\n");
    fprintf(stderr, "With no OUTPUT, write to standard output.\n");
    exit(1);
}

int main(int argc, char **argv) {
    int opt;
    int count_mode = 0; // Default: no count prefix

    // Parse options
    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt) {
            case 'c':
                count_mode = 1; // Enable count prefix
                break;
            default:
                usage();
        }
    }

    // Determine input and output files
    FILE *input = stdin;
    FILE *output = stdout;

    if (optind < argc) {
        const char *input_file = argv[optind++];
        input = fopen(input_file, "r");
        if (!input) {
            perror("uniq: fopen");
            return 1;
        }
    }

    if (optind < argc) {
        const char *output_file = argv[optind++];
        output = fopen(output_file, "w");
        if (!output) {
            perror("uniq: fopen");
            if (input != stdin) fclose(input);
            return 1;
        }
    }

    char prev_line[4096];
    char current_line[4096];
    int count = 0;
    int first_line = 1;

    while (fgets(current_line, sizeof(current_line), input)) {
        if (first_line) {
            strcpy(prev_line, current_line);
            count = 1;
            first_line = 0;
            continue;
        }

        if (strcmp(prev_line, current_line) == 0) {
            count++;
        } else {
            if (count_mode) {
                fprintf(output, "%d %s", count, prev_line);
            } else {
                fprintf(output, "%s", prev_line);
            }
            strcpy(prev_line, current_line);
            count = 1;
        }
    }

    // Output the last line
    if (!first_line) {
        if (count_mode) {
            fprintf(output, "%d %s", count, prev_line);
        } else {
            fprintf(output, "%s", prev_line);
        }
    }

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}

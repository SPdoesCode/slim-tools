#define _POSIX_C_SOURCE 200809L // Required for getopt(), optarg, optind, and strdup()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: tail [OPTION]... [FILE]\n");
    fprintf(stderr, "Print the last 10 lines of FILE to standard output.\n");
    fprintf(stderr, "With no FILE, read from standard input.\n");
    fprintf(stderr, "  -n NUM   Print the last NUM lines instead of the last 10.\n");
    exit(1);
}

// Function to print the last `num_lines` lines of a file
void print_tail(FILE *file, int num_lines) {
    char buffer[4096];
    char *lines[num_lines]; // Circular buffer to store lines
    int count = 0;
    int full = 0; // Flag to indicate if the buffer is full

    // Initialize the circular buffer
    for (int i = 0; i < num_lines; i++) {
        lines[i] = NULL;
    }

    // Read all lines into the circular buffer
    while (fgets(buffer, sizeof(buffer), file)) {
        if (lines[count % num_lines]) {
            free(lines[count % num_lines]); // Free previously allocated memory
        }
        lines[count % num_lines] = strdup(buffer); // Store a copy of the line
        count++;
    }

    // Determine the starting index for printing
    int start = full ? (count % num_lines) : 0;

    // Print the lines in order
    for (int i = 0; i < (full ? num_lines : count); i++) {
        int idx = (start + i) % num_lines;
        if (lines[idx]) {
            printf("%s", lines[idx]);
            free(lines[idx]); // Free memory after printing
        }
    }
}

int main(int argc, char **argv) {
    int opt;
    int num_lines = 10; // Default number of lines to print
    const char *filename = NULL;

    // Parse options
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                num_lines = atoi(optarg);
                if (num_lines <= 0) {
                    fprintf(stderr, "tail: invalid number of lines '%s'\n", optarg);
                    usage();
                }
                break;
            default:
                usage();
        }
    }

    // Determine input file
    if (optind < argc) {
        filename = argv[optind++];
    }

    FILE *file = stdin; // Default to stdin
    if (filename) {
        file = fopen(filename, "r");
        if (!file) {
            perror("tail: fopen");
            return 1;
        }
    }

    // Print the last `num_lines` lines
    print_tail(file, num_lines);

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for getopt(), optarg, optind

#define MAX_LINE_LENGTH 4096

void usage() {
    fprintf(stderr, "Usage: paste [-d DELIM] FILE...\n");
    exit(1);
}

int main(int argc, char **argv) {
    char delimiter = '\t'; // Default delimiter is tab
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
            case 'd':
                if (strlen(optarg) != 1) {
                    fprintf(stderr, "paste: invalid delimiter\n");
                    usage();
                }
                delimiter = optarg[0];
                break;
            default:
                usage();
        }
    }

    // Ensure at least one file or stdin is provided
    if (optind >= argc) {
        usage();
    }

    // Open all files
    int num_files = argc - optind;
    FILE *files[num_files];
    for (int i = 0; i < num_files; i++) {
        const char *filename = argv[optind + i];
        if (strcmp(filename, "-") == 0) {
            files[i] = stdin; // Use stdin for "-"
        } else {
            files[i] = fopen(filename, "r");
            if (!files[i]) {
                perror("paste: fopen");
                exit(1);
            }
        }
    }

    // Read and merge lines
    char line[MAX_LINE_LENGTH];
    int eof_flags = 0; // Bitmask to track EOF for each file

    while (1) {
        int all_eof = 1; // Assume all files are at EOF initially
        for (int i = 0; i < num_files; i++) {
            if (eof_flags & (1 << i)) {
                // File already reached EOF
                printf("%c", (i == num_files - 1) ? '\n' : delimiter);
                continue;
            }

            if (fgets(line, sizeof(line), files[i])) {
                all_eof = 0; // At least one file has more data
                // Remove trailing newline if present
                size_t len = strlen(line);
                if (len > 0 && line[len - 1] == '\n') {
                    line[len - 1] = '\0';
                }
                printf("%s%c", line, (i == num_files - 1) ? '\n' : delimiter);
            } else {
                // Mark this file as EOF
                eof_flags |= (1 << i);
                printf("%c", (i == num_files - 1) ? '\n' : delimiter);
            }
        }

        // Exit loop if all files are at EOF
        if (all_eof) {
            break;
        }
    }

    // Close files
    for (int i = 0; i < num_files; i++) {
        if (files[i] != stdin) {
            fclose(files[i]);
        }
    }

    return 0;
}

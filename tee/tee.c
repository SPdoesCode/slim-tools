#define _POSIX_C_SOURCE 200809L // Required for getopt(), optarg, and optind
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: tee [OPTION]... [FILE]...\n");
    fprintf(stderr, "Copy standard input to each FILE, and also to standard output.\n");
    fprintf(stderr, "  -a   Append to the given FILEs, do not overwrite\n");
    exit(1);
}

int main(int argc, char **argv) {
    int opt;
    int append_mode = 0; // Default: overwrite mode

    // Parse options
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
            case 'a':
                append_mode = 1; // Enable append mode
                break;
            default:
                usage();
        }
    }

    // Open all specified files
    FILE *files[argc - optind];
    int file_count = 0;

    for (int i = optind; i < argc; i++) {
        const char *filename = argv[i];
        FILE *file = fopen(filename, append_mode ? "a" : "w");
        if (!file) {
            perror("tee: fopen");
            exit(1);
        }
        files[file_count++] = file;
    }

    // Read from stdin and write to stdout and files
    char buffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0) {
        // Write to stdout
        fwrite(buffer, 1, bytes_read, stdout);

        // Write to each file
        for (int i = 0; i < file_count; i++) {
            fwrite(buffer, 1, bytes_read, files[i]);
        }
    }

    // Close all files
    for (int i = 0; i < file_count; i++) {
        fclose(files[i]);
    }

    return 0;
}

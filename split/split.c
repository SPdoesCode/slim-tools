#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for getopt(), optarg, and optind

void usage() {
    fprintf(stderr, "Usage: split [OPTION]... [FILE [PREFIX]]\n");
    fprintf(stderr, "Split FILE into pieces.\n");
    fprintf(stderr, "  -b SIZE  Set the size of each piece (e.g., 100, 1K, 1M)\n");
    exit(1);
}

unsigned long parse_size(const char *arg) {
    char *endptr;
    unsigned long size = strtoul(arg, &endptr, 10);

    if (endptr == arg || size == 0) {
        fprintf(stderr, "split: invalid size '%s'\n", arg);
        usage();
    }

    // Handle suffixes (K, M, G)
    switch (*endptr) {
        case '\0': // No suffix, default to bytes
            break;
        case 'K': // Kilobytes
            size *= 1024;
            break;
        case 'M': // Megabytes
            size *= 1024 * 1024;
            break;
        case 'G': // Gigabytes
            size *= 1024 * 1024 * 1024;
            break;
        default:
            fprintf(stderr, "split: invalid suffix '%c'\n", *endptr);
            usage();
    }

    return size;
}

void split_file(FILE *file, const char *prefix, unsigned long chunk_size) {
    char buffer[4096];
    size_t bytes_read;
    unsigned long total_bytes = 0;
    int part_number = 0;

    FILE *output_file = NULL;
    char output_filename[256];

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (total_bytes % chunk_size == 0) {
            // Close the previous output file
            if (output_file) {
                fclose(output_file);
            }

            // Open a new output file
            snprintf(output_filename, sizeof(output_filename), "%s%02d", prefix, part_number++);
            output_file = fopen(output_filename, "wb");
            if (!output_file) {
                perror("split: fopen");
                exit(1);
            }
        }

        // Write data to the current output file
        size_t bytes_to_write = (total_bytes + bytes_read > (part_number * chunk_size))
                                    ? (part_number * chunk_size - total_bytes)
                                    : bytes_read;
        fwrite(buffer, 1, bytes_to_write, output_file);

        // Update total bytes written
        total_bytes += bytes_read;

        // Write remaining data to the next file
        if (bytes_to_write < bytes_read) {
            fseek(file, -(long)(bytes_read - bytes_to_write), SEEK_CUR);
        }
    }

    if (output_file) {
        fclose(output_file);
    }
}

int main(int argc, char **argv) {
    int opt;
    unsigned long chunk_size = 0; // Default: no chunk size
    const char *prefix = "x";     // Default prefix

    // Parse options
    while ((opt = getopt(argc, argv, "b:")) != -1) {
        switch (opt) {
            case 'b':
                chunk_size = parse_size(optarg);
                break;
            default:
                usage();
        }
    }

    if (!chunk_size) {
        fprintf(stderr, "split: missing chunk size\n");
        usage();
    }

    // Determine input file and prefix
    const char *filename = NULL;
    if (optind < argc) {
        filename = argv[optind++];
    }

    if (optind < argc) {
        prefix = argv[optind++];
    }

    // Open the input file
    FILE *file = stdin;
    if (filename) {
        file = fopen(filename, "rb");
        if (!file) {
            perror("split: fopen");
            return 1;
        }
    }

    // Split the file
    split_file(file, prefix, chunk_size);

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

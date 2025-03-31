#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for getopt()
#include <errno.h>

#define BUFFER_SIZE 16 // Number of bytes per line

void usage() {
    fprintf(stderr, "Usage: od [-t TYPE] [FILE]\n");
    fprintf(stderr, "  TYPE:\n");
    fprintf(stderr, "    o   octal\n");
    fprintf(stderr, "    x   hexadecimal\n");
    fprintf(stderr, "    d   decimal\n");
    fprintf(stderr, "    c   characters (ASCII)\n");
    exit(1);
}

int main(int argc, char **argv) {
    char type = 'o'; // Default to octal
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't':
                if (strlen(optarg) != 1 || strchr("oxdc", optarg[0]) == NULL) {
                    fprintf(stderr, "od: invalid type '%s'\n", optarg);
                    usage();
                }
                type = optarg[0];
                break;
            default:
                usage();
        }
    }

    // Ensure a file argument is provided
    FILE *file = stdin; // Default to stdin if no file is provided

    if (optind < argc) {
        file = fopen(argv[optind], "rb"); // Open in binary mode
        if (!file) {
            perror("od: fopen");
            return 1;
        }
    }

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long offset = 0; // File offset

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        printf("%07lx ", offset); // Print offset in hexadecimal

        for (size_t i = 0; i < bytes_read; i++) {
            switch (type) {
                case 'o': // Octal
                    printf("%03o ", buffer[i]);
                    break;
                case 'x': // Hexadecimal
                    printf("%02x ", buffer[i]);
                    break;
                case 'd': // Decimal
                    printf("%03d ", buffer[i]);
                    break;
                case 'c': // Characters (ASCII)
                    if (buffer[i] >= 32 && buffer[i] <= 126) {
                        printf(" %c ", buffer[i]); // Printable ASCII
                    } else {
                        printf(" \\%03o", buffer[i]); // Non-printable as octal escape
                    }
                    break;
            }
        }

        // Pad the line if fewer than BUFFER_SIZE bytes were read
        for (size_t i = bytes_read; i < BUFFER_SIZE; i++) {
            printf("    "); // Padding for alignment
        }

        // Print ASCII representation
        printf(" |");
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] >= 32 && buffer[i] <= 126) {
                putchar(buffer[i]); // Printable ASCII
            } else {
                putchar('.'); // Non-printable as '.'
            }
        }
        printf("|\n");

        offset += bytes_read;
    }

    if (ferror(file)) {
        perror("od: fread");
        if (file != stdin) fclose(file);
        return 1;
    }

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

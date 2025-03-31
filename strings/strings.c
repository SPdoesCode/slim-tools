#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void usage() {
    fprintf(stderr, "Usage: strings [FILE]\n");
    fprintf(stderr, "Display printable strings in FILE.\n");
    exit(1);
}

void extract_strings(FILE *file) {
    char buffer[4096];
    size_t pos = 0;

    while (fread(&buffer[pos], 1, 1, file) == 1) {
        if (isprint(buffer[pos]) || buffer[pos] == '\t' || buffer[pos] == '\n' || buffer[pos] == '\r') {
            // Continue building the string
            pos++;
            if (pos >= sizeof(buffer)) {
                fwrite(buffer, 1, pos, stdout);
                pos = 0;
            }
        } else {
            // End of a string
            if (pos >= 4) { // Minimum length for a valid string
                fwrite(buffer, 1, pos, stdout);
                putchar('\n');
            }
            pos = 0;
        }
    }

    // Handle any remaining characters in the buffer
    if (pos >= 4) {
        fwrite(buffer, 1, pos, stdout);
        putchar('\n');
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        usage();
    }

    FILE *file = stdin; // Default to stdin
    if (argc == 2) {
        const char *filename = argv[1];
        file = fopen(filename, "rb");
        if (!file) {
            perror("strings: fopen");
            return 1;
        }
    }

    extract_strings(file);

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

void usage() {
    fprintf(stderr, "Usage: nl [FILE]\n");
    exit(1);
}

int main(int argc, char **argv) {
    // Check arguments
    if (argc > 2) {
        usage();
    }

    FILE *file = stdin; // Default to stdin if no file is provided
    const char *filename = "<stdin>";

    if (argc == 2) {
        filename = argv[1];
        file = fopen(argv[1], "r");
        if (!file) {
            perror("nl: fopen");
            return 1;
        }
    }

    int line_number = 1; // Start numbering from 1
    char buffer[4096];   // Buffer for reading lines

    while (fgets(buffer, sizeof(buffer), file)) {
        // Print line number and content
        printf("%6d\t%s", line_number++, buffer);
    }

    if (ferror(file)) {
        perror("nl: fgets");
        fclose(file);
        return 1;
    }

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

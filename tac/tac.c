#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
    fprintf(stderr, "Usage: tac [FILE]\n");
    fprintf(stderr, "Concatenate and print FILE in reverse.\n");
    exit(1);
}

// Function to reverse lines in a file
void reverse_lines(FILE *file) {
    char *lines[1024]; // Array to store pointers to lines
    char buffer[4096];
    int count = 0;

    // Read all lines into memory
    while (fgets(buffer, sizeof(buffer), file)) {
        // Allocate memory for the line and copy it
        lines[count] = malloc(strlen(buffer) + 1);
        if (!lines[count]) {
            perror("tac: malloc");
            exit(1);
        }
        strcpy(lines[count], buffer);
        count++;

        if (count >= 1024) {
            fprintf(stderr, "tac: too many lines\n");
            exit(1);
        }
    }

    // Print lines in reverse order
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", lines[i]);
        free(lines[i]); // Free allocated memory
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        usage();
    }

    FILE *file = stdin; // Default to stdin
    if (argc == 2) {
        const char *filename = argv[1];
        file = fopen(filename, "r");
        if (!file) {
            perror("tac: fopen");
            return 1;
        }
    }

    reverse_lines(file);

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

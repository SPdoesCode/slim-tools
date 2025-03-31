#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
    fprintf(stderr, "Usage: sort [FILE]\n");
    fprintf(stderr, "Sort lines of text from FILE (or stdin).\n");
    exit(1);
}

// Comparison function for qsort
int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
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
            perror("sort: fopen");
            return 1;
        }
    }

    char line[4096];
    char **lines = NULL;
    int count = 0;

    // Read all lines into memory
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Resize the array of lines
        char **new_lines = realloc(lines, (count + 1) * sizeof(char *));
        if (!new_lines) {
            perror("sort: realloc");
            return 1;
        }
        lines = new_lines;

        // Allocate space for the line and copy it
        lines[count] = malloc(strlen(line) + 1);
        if (!lines[count]) {
            perror("sort: malloc");
            return 1;
        }
        strcpy(lines[count], line);
        count++;
    }

    if (ferror(file)) {
        perror("sort: fgets");
        return 1;
    }

    if (file != stdin) {
        fclose(file);
    }

    // Sort the lines
    qsort(lines, count, sizeof(char *), compare_strings);

    // Print the sorted lines
    for (int i = 0; i < count; i++) {
        printf("%s\n", lines[i]);
        free(lines[i]);
    }

    free(lines);

    return 0;
}

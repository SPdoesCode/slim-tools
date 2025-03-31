#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read a line from a file
char *read_line(FILE *file) {
    size_t size = 0;
    size_t len = 0;
    char *line = NULL;

    while (1) {
        if (len + 1 >= size) {
            size = (size == 0) ? 128 : size * 2;
            line = realloc(line, size);
            if (!line) {
                perror("realloc");
                exit(1);
            }
        }
        int c = fgetc(file);
        if (c == EOF || c == '\n') {
            if (len == 0 && c == EOF) {
                free(line);
                return NULL; // End of file
            }
            line[len] = '\0';
            return line;
        }
        line[len++] = (char)c;
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: comm FILE1 FILE2\n");
        return 1;
    }

    // Open the two files
    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");

    if (!file1 || !file2) {
        perror("comm");
        return 1;
    }

    // Read lines from both files
    char *line1 = read_line(file1);
    char *line2 = read_line(file2);

    while (line1 || line2) {
        int cmp = 0;
        if (line1 && line2) {
            cmp = strcmp(line1, line2);
        } else if (line1) {
            cmp = -1; // File2 is exhausted
        } else {
            cmp = 1; // File1 is exhausted
        }

        if (cmp < 0) {
            // Line is only in file1
            printf("%s\t\t%s\n", line1, ""); // Column 1
            free(line1);
            line1 = read_line(file1);
        } else if (cmp > 0) {
            // Line is only in file2
            printf("\t%s\t%s\n", line2, ""); // Column 2
            free(line2);
            line2 = read_line(file2);
        } else {
            // Line is common to both files
            printf("\t\t%s\n", line1); // Column 3
            free(line1);
            free(line2);
            line1 = read_line(file1);
            line2 = read_line(file2);
        }
    }

    // Clean up
    if (line1) free(line1);
    if (line2) free(line2);
    fclose(file1);
    fclose(file2);

    return 0;
}

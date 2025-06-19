#define _POSIX_C_SOURCE 200809L // Enable POSIX features, including getopt()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for getopt(), optarg, optind

#define MAX_LINE 1024

// Function to split a line into fields based on a delimiter
int split_line(char *line, char **fields, int max_fields, char delimiter) {
    int count = 0;
    fields[count++] = line;

    while (*line && count < max_fields) {
        if (*line == delimiter) {
            *line = '\0';
            fields[count++] = line + 1;
        }
        line++;
    }

    return count;
}

// Function to compare two lines based on the join field
int compare_lines(char *line1, char *line2, int join_field1, int join_field2, char delimiter) {
    char *fields1[MAX_LINE];
    char *fields2[MAX_LINE];

    int num_fields1 = split_line(line1, fields1, MAX_LINE, delimiter);
    int num_fields2 = split_line(line2, fields2, MAX_LINE, delimiter);

    if (join_field1 >= num_fields1 || join_field2 >= num_fields2) {
        return -1; // Invalid join field
    }

    return strcmp(fields1[join_field1], fields2[join_field2]);
}

int main(int argc, char **argv) {
    int join_field1 = 1; // Default join field for file1
    int join_field2 = 1; // Default join field for file2
    char delimiter = ' '; // Default delimiter is space

    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, "1:2:t:")) != -1) {
        switch (opt) {
            case '1':
                join_field1 = atoi(optarg);
                if (join_field1 <= 0) {
                    fprintf(stderr, "join: invalid join field '%s'\n", optarg);
                    return 1;
                }
                break;
            case '2':
                join_field2 = atoi(optarg);
                if (join_field2 <= 0) {
                    fprintf(stderr, "join: invalid join field '%s'\n", optarg);
                    return 1;
                }
                break;
            case 't':
                if (strlen(optarg) != 1) {
                    fprintf(stderr, "join: delimiter must be a single character\n");
                    return 1;
                }
                delimiter = optarg[0];
                break;
            default:
                printf("join from SlimTools\n");
                fprintf(stderr, "Usage: join [-1 FIELD] [-2 FIELD] [-t DELIM] FILE1 FILE2\n");
                return 1;
        }
    }

    // Ensure we have exactly two file arguments
    if (optind + 2 != argc) {
        fprintf(stderr, "join: missing file operands\n");
        return 1;
    }

    const char *file1_name = argv[optind];
    const char *file2_name = argv[optind + 1];

    // Open the input files
    FILE *file1 = fopen(file1_name, "r");
    if (!file1) {
        perror("join: fopen");
        return 1;
    }

    FILE *file2 = fopen(file2_name, "r");
    if (!file2) {
        perror("join: fopen");
        fclose(file1);
        return 1;
    }

    // Read and process lines
    char line1[MAX_LINE], line2[MAX_LINE];
    char *fields1[MAX_LINE], *fields2[MAX_LINE];

    if (!fgets(line1, sizeof(line1), file1)) {
        fclose(file1);
        fclose(file2);
        return 0; // File1 is empty
    }

    if (!fgets(line2, sizeof(line2), file2)) {
        fclose(file1);
        fclose(file2);
        return 0; // File2 is empty
    }

    while (1) {
        int cmp = compare_lines(line1, line2, join_field1 - 1, join_field2 - 1, delimiter);

        if (cmp < 0) {
            // Line1 comes before line2
            if (!fgets(line1, sizeof(line1), file1)) {
                break;
            }
        } else if (cmp > 0) {
            // Line2 comes before line1
            if (!fgets(line2, sizeof(line2), file2)) {
                break;
            }
        } else {
            // Lines match on the join field
            int num_fields1 = split_line(line1, fields1, MAX_LINE, delimiter);
            int num_fields2 = split_line(line2, fields2, MAX_LINE, delimiter);

            // Print joined line
            for (int i = 0; i < num_fields1; i++) {
                printf("%s%c", fields1[i], (i == num_fields1 - 1 ? delimiter : delimiter));
            }
            for (int i = 0; i < num_fields2; i++) {
                if (i != join_field2 - 1) {
                    printf("%s%c", fields2[i], (i == num_fields2 - 1 ? '\n' : delimiter));
                }
            }

            // Advance both files
            if (!fgets(line1, sizeof(line1), file1)) {
                break;
            }
            if (!fgets(line2, sizeof(line2), file2)) {
                break;
            }
        }
    }

    // Close files
    fclose(file1);
    fclose(file2);

    return 0;
}

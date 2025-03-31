#define _POSIX_C_SOURCE 200809L // Enable POSIX features, including strdup()
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strdup()
#include <ctype.h>

#define MAX_LINE 4096

// Function to split a line into fields based on a delimiter
void split_fields(char *line, char delimiter, char **fields, int *field_count, int max_fields) {
    int count = 0;
    char *token = strtok(line, &delimiter);
    while (token && count < max_fields) {
        fields[count++] = token;
        token = strtok(NULL, &delimiter);
    }
    *field_count = count;
}

// Function to extract fields based on the specified range
void extract_fields(const char *input, const char *fields_arg, char delimiter) {
    // Parse the fields argument (e.g., "1-3", "2", "1,3")
    int field_indices[MAX_LINE] = {0};
    int field_count = 0;

    char *fields_copy = strdup(fields_arg); // Duplicate the fields argument
    if (!fields_copy) {
        perror("cut: strdup");
        exit(1);
    }

    char *token = strtok(fields_copy, ",");
    while (token) {
        if (strchr(token, '-')) {
            // Handle ranges like "1-3"
            int start, end;
            sscanf(token, "%d-%d", &start, &end);
            for (int i = start; i <= end; i++) {
                field_indices[field_count++] = i;
            }
        } else {
            // Handle single fields like "2"
            int field = atoi(token);
            field_indices[field_count++] = field;
        }
        token = strtok(NULL, ",");
    }
    free(fields_copy);

    // Split the input line into fields
    char *fields[MAX_LINE];
    int count = 0;

    char *line_copy = strdup(input); // Duplicate the input line
    if (!line_copy) {
        perror("cut: strdup");
        exit(1);
    }

    split_fields(line_copy, delimiter, fields, &count, MAX_LINE);

    // Print the specified fields
    for (int i = 0; i < field_count; i++) {
        int index = field_indices[i] - 1; // Convert to zero-based index
        if (index >= 0 && index < count) {
            printf("%s", fields[index]);
            if (i < field_count - 1) {
                putchar(delimiter); // Add delimiter between fields
            }
        }
    }
    putchar('\n');

    free(line_copy);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: cut [OPTIONS]... [FILE]...\n");
        printf("Extract sections from each line of files.\n");
        printf("Options:\n");
        printf("  -d DELIM   Use DELIM instead of TAB for field delimiter\n");
        printf("  -f LIST    Select only these fields; also print any line that contains no delimiter character\n");
        return 1;
    }

    char delimiter = '\t'; // Default delimiter is TAB
    const char *fields_arg = NULL;
    int arg_index = 1;

    // Parse options
    while (arg_index < argc && argv[arg_index][0] == '-') {
        if (strcmp(argv[arg_index], "-d") == 0) {
            if (arg_index + 1 >= argc) {
                printf("cut: option requires an argument -- 'd'\n");
                return 1;
            }
            delimiter = argv[++arg_index][0]; // Use the first character as the delimiter
        } else if (strcmp(argv[arg_index], "-f") == 0) {
            if (arg_index + 1 >= argc) {
                printf("cut: option requires an argument -- 'f'\n");
                return 1;
            }
            fields_arg = argv[++arg_index];
        } else {
            printf("cut: invalid option -- '%s'\n", argv[arg_index]);
            return 1;
        }
        arg_index++;
    }

    if (!fields_arg) {
        printf("cut: you must specify a list of fields with -f\n");
        return 1;
    }

    // Process input
    if (arg_index < argc) {
        // Read from files
        for (int i = arg_index; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (!file) {
                perror("cut: fopen");
                return 1;
            }

            char line[MAX_LINE];
            while (fgets(line, sizeof(line), file)) {
                // Remove trailing newline
                line[strcspn(line, "\n")] = '\0';
                extract_fields(line, fields_arg, delimiter);
            }

            fclose(file);
        }
    } else {
        // Read from stdin
        char line[MAX_LINE];
        while (fgets(line, sizeof(line), stdin)) {
            // Remove trailing newline
            line[strcspn(line, "\n")] = '\0';
            extract_fields(line, fields_arg, delimiter);
        }
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

// Function to parse the dircolors configuration file
void parse_dircolors(const char *filename, int print_bash) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("dircolors: fopen");
        exit(1);
    }

    char line[MAX_LINE];
    char ls_colors[8192] = ""; // Buffer to store LS_COLORS
    size_t ls_colors_len = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove leading/trailing whitespace
        char *start = line;
        while (isspace(*start)) start++;
        char *end = start + strlen(start) - 1;
        while (end > start && isspace(*end)) *end-- = '\0';

        // Skip empty lines and comments
        if (*start == '\0' || *start == '#') continue;

        // Parse key-value pairs
        char *key = strtok(start, " \t");
        char *value = strtok(NULL, " \t");
        if (key && value) {
            // Append to LS_COLORS
            size_t len = strlen(key) + strlen(value) + 2; // +2 for '=' and ':'
            if (ls_colors_len + len >= sizeof(ls_colors)) {
                fprintf(stderr, "dircolors: LS_COLORS buffer overflow\n");
                fclose(file);
                exit(1);
            }
            if (ls_colors_len > 0) {
                strcat(ls_colors, ":");
                ls_colors_len++;
            }
            strcat(ls_colors, key);
            strcat(ls_colors, "=");
            strcat(ls_colors, value);
            ls_colors_len += len;
        }
    }

    fclose(file);

    // Output the shell commands
    if (print_bash) {
        printf("LS_COLORS='%s'; export LS_COLORS\n", ls_colors);
    } else {
        printf("setenv LS_COLORS '%s';\n", ls_colors);
    }
}

int main(int argc, char **argv) {
    int print_bash = 1; // Default to bash/sh syntax
    const char *filename = NULL;

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            print_bash = 0; // Use csh/tcsh syntax
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "dircolors: invalid option '%s'\n", argv[i]);
            return 1;
        } else {
            filename = argv[i];
        }
    }

    // Default to ~/.dircolors if no file is specified
    if (!filename) {
        const char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "dircolors: HOME not set\n");
            return 1;
        }
        static char default_file[1024];
        snprintf(default_file, sizeof(default_file), "%s/.dircolors", home);
        filename = default_file;
    }

    // Parse and output dircolors
    parse_dircolors(filename, print_bash);

    return 0;
}

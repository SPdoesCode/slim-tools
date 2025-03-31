#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to interpret escape sequences in a string
void interpret_escapes(const char *input, char *output) {
    const char *src = input;
    char *dst = output;

    while (*src) {
        if (*src == '\\' && *(src + 1)) {
            switch (*(src + 1)) {
                case 'n':  *dst++ = '\n'; break;
                case 't':  *dst++ = '\t'; break;
                case '\\': *dst++ = '\\'; break;
                default:   *dst++ = *src; break;
            }
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

int main(int argc, char **argv) {
    int no_newline = 0; // -n flag: suppress trailing newline
    int interpret = 0;  // -e flag: interpret escape sequences
    int start_index = 1;

    // Parse options
    while (start_index < argc && argv[start_index][0] == '-') {
        if (strcmp(argv[start_index], "-n") == 0) {
            no_newline = 1;
        } else if (strcmp(argv[start_index], "-e") == 0) {
            interpret = 1;
        } else {
            fprintf(stderr, "echo: invalid option '%s'\n", argv[start_index]);
            return 1;
        }
        start_index++;
    }

    // Check if there are any arguments left
    if (start_index >= argc) {
        // No arguments provided, just print a newline (or nothing if -n)
        if (!no_newline) {
            printf("\n");
        }
        return 0;
    }

    // Process and print arguments
    for (int i = start_index; i < argc; i++) {
        if (i > start_index) {
            putchar(' '); // Add space between arguments
        }

        if (interpret) {
            char buffer[4096]; // Buffer to hold interpreted string
            interpret_escapes(argv[i], buffer);
            printf("%s", buffer);
        } else {
            printf("%s", argv[i]);
        }
    }

    // Print a newline unless -n is specified
    if (!no_newline) {
        printf("\n");
    }

    return 0;
}

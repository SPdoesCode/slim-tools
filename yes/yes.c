#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
    fprintf(stderr, "yes from SlimTools\n");
    fprintf(stderr, "Usage: yes [STRING]...\n");
    fprintf(stderr, "Repeatedly output STRING or 'y' if no arguments are provided.\n");
    exit(1);
}

int main(int argc, char **argv) {
    // Determine the string to output
    const char *output = "y\n"; // Default output
    if (argc > 1) {
        // Concatenate all arguments into a single string
        size_t length = 0;
        for (int i = 1; i < argc; i++) {
            length += strlen(argv[i]) + 1; // Add space for each argument and a space
        }

        char *buffer = malloc(length + 1); // +1 for null terminator
        if (!buffer) {
            perror("yes: malloc");
            return 1;
        }

        buffer[0] = '\0'; // Start with an empty string
        for (int i = 1; i < argc; i++) {
            strcat(buffer, argv[i]);
            if (i < argc - 1) {
                strcat(buffer, " "); // Add a space between arguments
            }
        }
        strcat(buffer, "\n"); // Add a newline at the end
        output = buffer;
    }

    // Continuously output the string
    while (1) {
        if (fputs(output, stdout) == EOF) {
            perror("yes: fputs");
            if (argc > 1) {
                free((void *)output); // Free allocated memory
            }
            return 1;
        }
    }

    // Unreachable code, but free memory if we ever break the loop
    if (argc > 1) {
        free((void *)output);
    }

    return 0;
}

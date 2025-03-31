#define _POSIX_C_SOURCE 200809L // Enable POSIX features
#define _GNU_SOURCE            // Enable GNU extensions (fallback)
#include <stdio.h>
#include <stdlib.h> // For putenv(), unsetenv(), getenv()
#include <string.h>
#include <unistd.h> // For execvp()

// Function to display all environment variables
void display_environment() {
    extern char **environ; // Access the global environment array
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

// Function to parse and modify the environment
int modify_and_run(int argc, char **argv) {
    int i = 1;

    // Process options until we find a non-option argument
    while (i < argc && argv[i][0] == '-') {
        if (strncmp(argv[i], "-u", 2) == 0) {
            // Unset an environment variable
            const char *var_name = argv[i] + 2;
            if (*var_name == '\0') {
                if (++i >= argc) {
                    fprintf(stderr, "env: option requires an argument -- 'u'\n");
                    return 1;
                }
                var_name = argv[i];
            }
            if (unsetenv(var_name) != 0) {
                perror("env: unsetenv");
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            // Start with an empty environment
            extern char **environ;
            environ = NULL; // Clear the environment
        } else {
            fprintf(stderr, "env: invalid option '%s'\n", argv[i]);
            return 1;
        }
        i++;
    }

    // Set new environment variables
    while (i < argc && strchr(argv[i], '=') != NULL) {
        if (putenv(argv[i]) != 0) {
            perror("env: putenv");
            return 1;
        }
        i++;
    }

    // If no command is provided, just display the environment
    if (i >= argc) {
        display_environment();
        return 0;
    }

    // Execute the specified command
    char *command = argv[i];
    char **args = &argv[i];

    execvp(command, args);
    perror("env: execvp"); // If execvp fails
    return 127;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // No arguments provided, display the environment
        display_environment();
        return 0;
    }

    return modify_and_run(argc, argv);
}

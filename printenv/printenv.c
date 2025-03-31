#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage() {
    fprintf(stderr, "Usage: printenv [VARIABLE]\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        usage();
    }

    // If no arguments are provided, print all environment variables
    if (argc == 1) {
        extern char **environ; // Access the environment variables array
        for (char **env = environ; *env != NULL; env++) {
            printf("%s\n", *env);
        }
        return 0;
    }

    // If a specific variable is provided, print its value
    const char *variable = argv[1];
    const char *value = getenv(variable);

    if (value) {
        printf("%s\n", value);
    } else {
        // Return failure if the variable is not found
        return 1;
    }

    return 0;
}

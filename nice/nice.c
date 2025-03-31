#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for nice(), execvp()
#include <errno.h>

void usage() {
    fprintf(stderr, "Usage: nice [-n N] COMMAND [ARG...]\n");
    exit(1);
}

int main(int argc, char **argv) {
    int niceness = 0; // Default niceness value
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                niceness = atoi(optarg); // Parse niceness value
                break;
            default:
                usage();
        }
    }

    // Ensure a command is provided
    if (optind >= argc) {
        usage();
    }

    // Adjust the niceness of the current process
    errno = 0;
    if (nice(niceness) == -1 && errno != 0) {
        perror("nice");
        return 1;
    }

    // Execute the specified command
    execvp(argv[optind], &argv[optind]);

    // If execvp fails
    perror("execvp");
    return 1;
}

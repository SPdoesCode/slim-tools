#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for readlink()
#include <limits.h> // Required for PATH_MAX

void usage() {
    fprintf(stderr, "Usage: readlink FILE\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
    }

    const char *path = argv[1];
    char buffer[PATH_MAX]; // Buffer to hold the resolved path

    // Resolve the symbolic link
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len == -1) {
        perror("readlink");
        return 1;
    }

    // Null-terminate the resolved path
    buffer[len] = '\0';

    // Print the resolved path
    printf("%s\n", buffer);

    return 0;
}

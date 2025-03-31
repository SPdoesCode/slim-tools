#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for getcwd()

void usage() {
    fprintf(stderr, "Usage: pwd\n");
    exit(1);
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused))) {
    if (argc > 1) {
        usage();
    }

    char cwd[4096]; // Buffer to hold the current working directory

    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd: getcwd");
        return 1;
    }

    // Print the current working directory
    printf("%s\n", cwd);

    return 0;
}

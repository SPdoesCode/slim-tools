#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Required for fork(), execvp()
#include <signal.h>  // Required for signal handling
#include <fcntl.h>   // Required for open()
#include <errno.h>

void usage() {
    fprintf(stderr, "Usage: nohup COMMAND [ARG...]\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    // Redirect standard output and standard error to "nohup.out" if they are terminals
    if (isatty(STDOUT_FILENO)) {
        int fd = open("nohup.out", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fd == -1) {
            perror("nohup: open");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Redirect stdout to nohup.out
        close(fd);
    }

    if (isatty(STDERR_FILENO)) {
        int fd = open("nohup.out", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fd == -1) {
            perror("nohup: open");
            exit(1);
        }
        dup2(fd, STDERR_FILENO); // Redirect stderr to nohup.out
        close(fd);
    }

    // Ignore SIGHUP signal
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
        perror("nohup: signal");
        exit(1);
    }

    // Execute the specified command
    execvp(argv[1], &argv[1]);

    // If execvp fails
    perror("nohup: execvp");
    exit(1);
}

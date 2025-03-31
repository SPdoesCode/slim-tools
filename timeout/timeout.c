#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage() {
    fprintf(stderr, "Usage: timeout DURATION COMMAND [ARG]...\n");
    fprintf(stderr, "Start COMMAND and kill it if it runs longer than DURATION seconds.\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage();
    }

    // Parse the timeout duration
    int timeout = atoi(argv[1]);
    if (timeout <= 0) {
        fprintf(stderr, "timeout: invalid duration '%s'\n", argv[1]);
        usage();
    }

    // Fork a child process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        perror("timeout: fork");
        return 1;
    }

    if (pid == 0) {
        // Child process: execute the command
        execvp(argv[2], &argv[2]);
        perror("timeout: execvp");
        exit(1);
    } else {
        // Parent process: wait for the child or kill it after the timeout
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        // Sleep for the specified duration
        sleep(timeout);

        // Check if the child is still running
        result = waitpid(pid, &status, WNOHANG);
        if (result == 0) {
            // Child is still running; kill it
            kill(pid, SIGTERM);
            printf("timeout: killed '%s' after %d seconds\n", argv[2], timeout);
        } else if (result == -1) {
            perror("timeout: waitpid");
            return 1;
        } else {
            // Child exited before the timeout
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                return WTERMSIG(status);
            }
        }
    }

    return 0;
}

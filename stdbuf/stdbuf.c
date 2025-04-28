#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-oL|-o0|-e0] command [args...]\n", argv[0]);
        return 1;
    }

    int stdout_mode = -1, stderr_mode = -1;
    int cmd_index = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-oL") == 0) {
            stdout_mode = _IOLBF;
            cmd_index = i + 1;
        } else if (strcmp(argv[i], "-o0") == 0) {
            stdout_mode = _IONBF;
            cmd_index = i + 1;
        } else if (strcmp(argv[i], "-e0") == 0) {
            stderr_mode = _IONBF;
            cmd_index = i + 1;
        } else {
            break;
        }
    }

    if (cmd_index >= argc) {
        fprintf(stderr, "No command provided\n");
        return 1;
    }

    if (stdout_mode != -1) {
        if (setvbuf(stdout, NULL, stdout_mode, 0) != 0) {
            perror("Cannot set stdout buffer");
            return 1;
        }
    }

    if (stderr_mode != -1) {
        if (setvbuf(stderr, NULL, stderr_mode, 0) != 0) {
            perror("Cannot set stderr buffer");
            return 1;
        }
    }

    execvp(argv[cmd_index], &argv[cmd_index]);
    perror("Cannot execute command");
    return 1;
}

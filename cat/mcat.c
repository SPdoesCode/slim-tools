#include <unistd.h>
#include <fcntl.h>  // For O_RDONLY
#include <stdio.h>  // Optional: For debugging

int main(int argc, char **argv) {
    char buf[4096];
    int fd, i = (argc > 1) ? 1 : 0;
    ssize_t n;

    do {
        fd = (i) ? open(argv[i], O_RDONLY) : STDIN_FILENO;  // Fixed: Use O_RDONLY
        if (fd < 0) continue;
        while ((n = read(fd, buf, sizeof(buf))) > 0)
            write(STDOUT_FILENO, buf, n);
        close(fd);
    } while (++i < argc);  // Fixed: Increment `i`, not `1`

    return 0;
}

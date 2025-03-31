#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Required for PATH_MAX and NAME_MAX
#include <sys/stat.h> // Required for stat()
#include <errno.h>
#include <unistd.h> // Required for getopt(), optarg, optind

void usage() {
    fprintf(stderr, "Usage: pathchk [-p] PATH...\n");
    fprintf(stderr, "  -p   Check for POSIX portability\n");
    exit(1);
}

int check_path(const char *path, int posix_portable) {
    size_t len = strlen(path);

    // Check overall path length
    if (len > PATH_MAX) {
        fprintf(stderr, "pathchk: '%s': Path too long (max %d characters)\n", path, PATH_MAX);
        return 1;
    }

    // Check individual component length
    const char *start = path;
    const char *end;
    while ((end = strchr(start, '/'))) {
        size_t component_len = end - start;
        if (component_len > NAME_MAX) {
            fprintf(stderr, "pathchk: '%s': Component too long (max %d characters)\n", path, NAME_MAX);
            return 1;
        }
        start = end + 1;
    }

    // Check last component if no trailing slash
    if (*start != '\0') {
        size_t component_len = strlen(start);
        if (component_len > NAME_MAX) {
            fprintf(stderr, "pathchk: '%s': Component too long (max %d characters)\n", path, NAME_MAX);
            return 1;
        }
    }

    // Check for invalid characters (POSIX portability)
    if (posix_portable) {
        for (size_t i = 0; i < len; i++) {
            char c = path[i];
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                  (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-' || c == '/')) {
                fprintf(stderr, "pathchk: '%s': Contains non-portable character '%c'\n", path, c);
                return 1;
            }
        }
    }

    // Check if path exists (optional validation)
    struct stat st;
    if (stat(path, &st) == -1 && errno != ENOENT) {
        perror("pathchk: stat");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int posix_portable = 0; // Default: No POSIX portability checks
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                posix_portable = 1;
                break;
            default:
                usage();
        }
    }

    // Ensure at least one path is provided
    if (optind >= argc) {
        usage();
    }

    int ret = 0;

    // Check each path
    for (int i = optind; i < argc; i++) {
        ret |= check_path(argv[i], posix_portable);
    }

    return ret;
}

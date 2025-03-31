#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for getopt(), optarg, optind
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h> // Required for getpwnam() and struct passwd
#include <grp.h> // Required for getgrnam() and struct group

#define MODE_RWX (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

void usage() {
    fprintf(stderr, "Usage: install [-m MODE] [-o OWNER] [-g GROUP] SOURCE DEST\n");
    exit(1);
}

int main(int argc, char **argv) {
    int opt;
    mode_t mode = MODE_RWX; // Default permissions: rwxr-xr-x
    uid_t owner = -1;       // Default: no change
    gid_t group = -1;       // Default: no change

    // Parse options
    while ((opt = getopt(argc, argv, "m:o:g:")) != -1) {
        switch (opt) {
            case 'm': {
                // Parse mode (octal number)
                char *endptr;
                mode = strtol(optarg, &endptr, 8);
                if (*endptr != '\0') {
                    fprintf(stderr, "install: invalid mode '%s'\n", optarg);
                    return 1;
                }
                break;
            }
            case 'o': {
                // Parse owner (username or UID)
                struct passwd *pw = getpwnam(optarg);
                if (pw) {
                    owner = pw->pw_uid;
                } else {
                    char *endptr;
                    owner = strtol(optarg, &endptr, 10);
                    if (*endptr != '\0' || errno == EINVAL) {
                        fprintf(stderr, "install: invalid owner '%s'\n", optarg);
                        return 1;
                    }
                }
                break;
            }
            case 'g': {
                // Parse group (groupname or GID)
                struct group *grp = getgrnam(optarg);
                if (grp) {
                    group = grp->gr_gid;
                } else {
                    char *endptr;
                    group = strtol(optarg, &endptr, 10);
                    if (*endptr != '\0' || errno == EINVAL) {
                        fprintf(stderr, "install: invalid group '%s'\n", optarg);
                        return 1;
                    }
                }
                break;
            }
            default:
                usage();
        }
    }

    // Ensure we have exactly two arguments (SOURCE and DEST)
    if (optind + 2 != argc) {
        usage();
    }

    const char *source = argv[optind];
    const char *dest = argv[optind + 1];

    // Open the source file
    int src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        perror("install: open");
        return 1;
    }

    // Create or truncate the destination file
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (dest_fd == -1) {
        perror("install: open");
        close(src_fd);
        return 1;
    }

    // Copy data from source to destination
    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("install: write");
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        perror("install: read");
        close(src_fd);
        close(dest_fd);
        return 1;
    }

    // Close file descriptors
    close(src_fd);
    close(dest_fd);

    // Set ownership if specified
    if (owner != (uid_t)-1 || group != (gid_t)-1) {
        if (chown(dest, owner, group) == -1) {
            perror("install: chown");
            return 1;
        }
    }

    // Set permissions if not already set during creation
    if (chmod(dest, mode) == -1) {
        perror("install: chmod");
        return 1;
    }

    return 0;
}

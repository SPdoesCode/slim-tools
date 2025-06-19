#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>   // For DIR, opendir(), readdir(), closedir()
#include <errno.h>    // For errno
#include <limits.h>   // For PATH_MAX

// Fallback value for PATH_MAX if not defined
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define BUFFER_SIZE 4096

// Function to copy a file
int copy_file(const char *src, const char *dest) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("cp: open source");
        return 1;
    }

    // Get source file permissions
    struct stat src_stat;
    if (fstat(src_fd, &src_stat) < 0) {
        perror("cp: fstat");
        close(src_fd);
        return 1;
    }

    // Open or create the destination file with the same permissions
    int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dest_fd < 0) {
        perror("cp: open destination");
        close(src_fd);
        return 1;
    }

    // Copy data in chunks
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("cp: write");
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read < 0) {
        perror("cp: read");
        close(src_fd);
        close(dest_fd);
        return 1;
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}

// Function to copy a directory recursively
int copy_directory(const char *src, const char *dest) {
    // Create the destination directory with the same permissions
    struct stat src_stat;
    if (stat(src, &src_stat) < 0) {
        perror("cp: stat");
        return 1;
    }

    if (mkdir(dest, src_stat.st_mode) < 0 && errno != EEXIST) {
        perror("cp: mkdir");
        return 1;
    }

    // Open the source directory
    DIR *dir = opendir(src);
    if (!dir) {
        perror("cp: opendir");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full paths for source and destination
        char src_path[PATH_MAX], dest_path[PATH_MAX];
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

        // Check if the entry is a directory or a file
        struct stat entry_stat;
        if (stat(src_path, &entry_stat) < 0) {
            perror("cp: stat");
            closedir(dir);
            return 1;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            // Recursively copy directories
            if (copy_directory(src_path, dest_path) != 0) {
                closedir(dir);
                return 1;
            }
        } else {
            // Copy files
            if (copy_file(src_path, dest_path) != 0) {
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
                printf("cp from SlimTools\n");
        printf("Usage: cp SOURCE DEST\n");
        printf("       cp -r SOURCE_DIR DEST_DIR\n");
        return 1;
    }

    // Handle recursive copy option
    int recursive = 0;
    if (argc == 4 && strcmp(argv[1], "-r") == 0) {
        recursive = 1;
        argv++; // Shift arguments
    }

    const char *src = argv[1];
    const char *dest = argv[2];

    // Check if the source exists
    struct stat src_stat;
    if (stat(src, &src_stat) < 0) {
        perror("cp: stat");
        return 1;
    }

    if (S_ISDIR(src_stat.st_mode)) {
        if (!recursive) {
            printf("cp: omitting directory '%s'\n", src);
            return 1;
        }
        // Recursive copy
        if (copy_directory(src, dest) != 0) {
            return 1;
        }
    } else {
        // Regular file copy
        if (copy_file(src, dest) != 0) {
            return 1;
        }
    }

    return 0;
}

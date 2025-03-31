#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>

// Function to display disk usage statistics
void display_disk_usage(const char *path, int human_readable) {
    struct statvfs stats;

    // Get filesystem statistics
    if (statvfs(path, &stats) != 0) {
        perror("df: statvfs");
        return;
    }

    // Calculate block size and total/free blocks
    unsigned long long block_size = stats.f_frsize;
    unsigned long long total_blocks = stats.f_blocks;
    unsigned long long free_blocks = stats.f_bfree;
    unsigned long long available_blocks = stats.f_bavail;

    // Convert to bytes
    unsigned long long total_space = total_blocks * block_size;
    unsigned long long free_space = free_blocks * block_size;
    unsigned long long available_space = available_blocks * block_size;

    // Print the results
    if (human_readable) {
        printf("%-20s %8lluK %8lluK %8lluK %5.1f%%\n", path,
               total_space / 1024, free_space / 1024, available_space / 1024,
               (double)(total_space - free_space) / total_space * 100.0);
    } else {
        printf("%-20s %8llu %8llu %8llu %5.1f%%\n", path,
               total_space, free_space, available_space,
               (double)(total_space - free_space) / total_space * 100.0);
    }
}

int main(int argc, char **argv) {
    int human_readable = 0;

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            human_readable = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "df: invalid option '%s'\n", argv[i]);
            return 1;
        }
    }

    // Print header
    if (human_readable) {
        printf("%-20s %8s %8s %8s %5s\n", "Filesystem", "Size(K)", "Used(K)", "Avail(K)", "Use%");
    } else {
        printf("%-20s %8s %8s %8s %5s\n", "Filesystem", "Size", "Used", "Avail", "Use%");
    }

    // Display disk usage for all arguments or default to "/"
    if (argc == 1 || (argc == 2 && human_readable)) {
        display_disk_usage("/", human_readable);
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                display_disk_usage(argv[i], human_readable);
            }
        }
    }

    return 0;
}

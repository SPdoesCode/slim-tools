#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#define BUFFER_SIZE 4096

// Function to display progress
void display_progress(off_t total_bytes, off_t copied_bytes) {
    static time_t last_update = 0;
    time_t current_time = time(NULL);

    // Update progress every second
    if (current_time != last_update) {
        last_update = current_time;
        double progress = (double)copied_bytes / (double)total_bytes * 100.0;
        printf("\rProgress: %.2f%% (%ld/%ld bytes)", progress, (long)copied_bytes, (long)total_bytes);
        fflush(stdout);
    }
}

// Main dd function
int dd(const char *ifile, const char *ofile, size_t bs, off_t count, int status) {
    int input_fd = -1, output_fd = -1;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    off_t total_copied = 0;

    // Open input file
    if (strcmp(ifile, "-") == 0) {
        input_fd = STDIN_FILENO; // Read from stdin
    } else {
        input_fd = open(ifile, O_RDONLY);
        if (input_fd < 0) {
            perror("dd: open input");
            return 1;
        }
    }

    // Open output file
    if (strcmp(ofile, "-") == 0) {
        output_fd = STDOUT_FILENO; // Write to stdout
    } else {
        output_fd = open(ofile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            perror("dd: open output");
            if (input_fd >= 0 && input_fd != STDIN_FILENO) close(input_fd);
            return 1;
        }
    }

    // Copy data
    while ((bytes_read = read(input_fd, buffer, bs)) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("dd: write");
            if (input_fd >= 0 && input_fd != STDIN_FILENO) close(input_fd);
            if (output_fd >= 0 && output_fd != STDOUT_FILENO) close(output_fd);
            return 1;
        }
        total_copied += bytes_written;

        // Progress reporting
        if (status) {
            struct stat st;
            if (fstat(input_fd, &st) == 0 && S_ISREG(st.st_mode)) {
                display_progress(st.st_size, total_copied);
            }
        }

        // Stop if count is reached
        if (count > 0 && total_copied >= count) break;
    }

    if (bytes_read < 0) {
        perror("dd: read");
        if (input_fd >= 0 && input_fd != STDIN_FILENO) close(input_fd);
        if (output_fd >= 0 && output_fd != STDOUT_FILENO) close(output_fd);
        return 1;
    }

    // Close files
    if (input_fd >= 0 && input_fd != STDIN_FILENO) close(input_fd);
    if (output_fd >= 0 && output_fd != STDOUT_FILENO) close(output_fd);

    // Final progress report
    if (status) {
        printf("\rProgress: 100.00%% (%ld/%ld bytes)\n", (long)total_copied, (long)total_copied);
    } else {
        printf("%ld bytes copied\n", (long)total_copied);
    }

    return 0;
}

int main(int argc, char **argv) {
    const char *ifile = NULL, *ofile = NULL;
    size_t bs = 512; // Default block size
    off_t count = 0; // Default: no limit
    int status = 0;  // Default: no progress

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "if=", 3) == 0) {
            ifile = argv[i] + 3;
        } else if (strncmp(argv[i], "of=", 3) == 0) {
            ofile = argv[i] + 3;
        } else if (strncmp(argv[i], "bs=", 3) == 0) {
            bs = strtoul(argv[i] + 3, NULL, 10);
        } else if (strncmp(argv[i], "count=", 6) == 0) {
            count = strtoull(argv[i] + 6, NULL, 10);
        } else if (strcmp(argv[i], "status=progress") == 0) {
            status = 1;
        } else {
            fprintf(stderr, "dd: invalid argument '%s'\n", argv[i]);
            return 1;
        }
    }

    // Validate required arguments
    if (!ifile || !ofile) {
        fprintf(stderr, "Usage: dd if=INPUT of=OUTPUT [bs=BYTES] [count=BLOCKS] [status=progress]\n");
        return 1;
    }

    // Call the dd function
    return dd(ifile, ofile, bs, count, status);
}

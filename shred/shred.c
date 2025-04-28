#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 4096

int shred_file(const char *filename) {
    int file_descriptor = open(filename, O_WRONLY);
    if (file_descriptor < 0) {
        perror("Cannot open file");
        return 1;
    }

    off_t file_size = lseek(file_descriptor, 0, SEEK_END);
    if (file_size < 0) {
        perror("Cannot get file size");
        close(file_descriptor);
        return 1;
    }
    lseek(file_descriptor, 0, SEEK_SET);

    srand(time(NULL));

    char random_data[BUFFER_SIZE];
    off_t bytes_left = file_size;

    while (bytes_left > 0) {
        size_t bytes_to_write = bytes_left < BUFFER_SIZE ? bytes_left : BUFFER_SIZE;
        for (size_t i = 0; i < bytes_to_write; i++) {
            random_data[i] = rand() % 256;
        }
        ssize_t bytes_written = write(file_descriptor, random_data, bytes_to_write);
        if (bytes_written < 0) {
            perror("Write error");
            close(file_descriptor);
            return 1;
        }
        bytes_left -= bytes_written;
    }

    if (fsync(file_descriptor) < 0) {
        perror("Sync error");
        close(file_descriptor);
        return 1;
    }

    close(file_descriptor);

    if (unlink(filename) < 0) {
        perror("Cannot delete file");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    return shred_file(argv[1]);
}

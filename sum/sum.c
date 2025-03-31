#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: sum [FILE]\n");
    fprintf(stderr, "Calculate and print checksum and block count for FILE.\n");
    exit(1);
}

// Function to calculate the checksum
unsigned int calculate_checksum(FILE *file, unsigned long *block_count) {
    unsigned int checksum = 0;
    unsigned char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            checksum = (checksum >> 1) + ((checksum & 1) << 15); // Rotate right by 1
            checksum += buffer[i];
            checksum &= 0xFFFF; // Keep only 16 bits
        }
        *block_count += (bytes_read + 511) / 512; // Block size is 512 bytes
    }

    return checksum;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        usage();
    }

    const char *filename = NULL;
    FILE *file = stdin; // Default to stdin

    if (argc == 2) {
        filename = argv[1];
        file = fopen(filename, "rb");
        if (!file) {
            perror("sum: fopen");
            return 1;
        }
    }

    unsigned long block_count = 0;
    unsigned int checksum = calculate_checksum(file, &block_count);

    if (file != stdin) {
        fclose(file);
    }

    printf("%u %lu", checksum, block_count);
    if (filename) {
        printf(" %s", filename);
    }
    printf("\n");

    return 0;
}

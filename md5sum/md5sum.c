#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <openssl/evp.h> // Use modern EVP API [[1]]

#define BUFFER_SIZE 4096

void usage() {
    fprintf(stderr, "Usage: md5sum FILE...\n");
    exit(1);
}

// Function to compute the MD5 hash of a file using EVP API
void compute_md5(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("md5sum: fopen");
        return;
    }

    // Initialize EVP context
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5(); // Use MD5 algorithm
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    if (!ctx || !md) {
        perror("md5sum: EVP initialization");
        fclose(file);
        return;
    }

    // Initialize digest
    if (EVP_DigestInit_ex(ctx, md, NULL) != 1) {
        perror("md5sum: EVP_DigestInit_ex");
        fclose(file);
        EVP_MD_CTX_free(ctx);
        return;
    }

    // Read file in chunks and update digest
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (EVP_DigestUpdate(ctx, buffer, bytes_read) != 1) {
            perror("md5sum: EVP_DigestUpdate");
            fclose(file);
            EVP_MD_CTX_free(ctx);
            return;
        }
    }

    if (ferror(file)) {
        perror("md5sum: fread");
        fclose(file);
        EVP_MD_CTX_free(ctx);
        return;
    }

    fclose(file);

    // Finalize digest computation
    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        perror("md5sum: EVP_DigestFinal_ex");
        EVP_MD_CTX_free(ctx);
        return;
    }

    EVP_MD_CTX_free(ctx);

    // Print the hash in hexadecimal format
    for (unsigned int i = 0; i < hash_len; i++) {
        printf("%02x", hash[i]);
    }
    printf("  %s\n", filename);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    for (int i = 1; i < argc; i++) {
        compute_md5(argv[i]);
    }

    return 0;
}

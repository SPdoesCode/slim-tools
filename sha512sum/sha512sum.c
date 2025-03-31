#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h> // Required for EVP API

void usage() {
    fprintf(stderr, "Usage: sha512sum FILE...\n");
    exit(1);
}

void compute_sha512(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("sha512sum: fopen");
        return;
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    unsigned char buffer[4096];
    size_t bytes_read;

    // Create and initialize the EVP context
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        perror("sha512sum: EVP_MD_CTX_new");
        fclose(file);
        return;
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha512(), NULL) != 1) {
        perror("sha512sum: EVP_DigestInit_ex");
        EVP_MD_CTX_free(ctx);
        fclose(file);
        return;
    }

    // Update the hash with file contents
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (EVP_DigestUpdate(ctx, buffer, bytes_read) != 1) {
            perror("sha512sum: EVP_DigestUpdate");
            EVP_MD_CTX_free(ctx);
            fclose(file);
            return;
        }
    }

    if (ferror(file)) {
        perror("sha512sum: fread");
        EVP_MD_CTX_free(ctx);
        fclose(file);
        return;
    }

    // Finalize the hash
    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        perror("sha512sum: EVP_DigestFinal_ex");
        EVP_MD_CTX_free(ctx);
        fclose(file);
        return;
    }

    EVP_MD_CTX_free(ctx);
    fclose(file);

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
        compute_sha512(argv[i]);
    }

    return 0;
}

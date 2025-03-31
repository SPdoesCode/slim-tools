#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() {
    fprintf(stderr, "Usage: sync\n");
    fprintf(stderr, "Flush filesystem buffers.\n");
    exit(1);
}


int main(int argc, char **argv __attribute__((unused))) {
    if (argc != 1) {
        usage();
    }


    // Call to sync() syscall to flush filesys buffers
    sync();

    return 0;
}

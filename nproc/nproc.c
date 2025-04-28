#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* for size_t */
#include <unistd.h> /* required for sysconf() */

void
usage()
{
        fprintf(stderr, "Usage: nproc\n");
        exit(1);
}

int
main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
        if (argc > 1) {
                usage();
        }

        size_t nproc = sysconf(_SC_NPROCESSORS_CONF); /* 16 bits with size_t */ 

        printf("%zu\n", nproc);
        
        return 0;
}

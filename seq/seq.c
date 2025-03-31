#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void usage() {
    fprintf(stderr, "Usage: seq [OPTION]... LAST\n");
    fprintf(stderr, "  or:  seq [OPTION]... FIRST LAST\n");
    fprintf(stderr, "  or:  seq [OPTION]... FIRST INCREMENT LAST\n");
    fprintf(stderr, "Print numbers from FIRST to LAST, in steps of INCREMENT.\n");
    exit(1);
}

int main(int argc, char **argv) {
    double first = 1.0, increment = 1.0, last;

    // Parse arguments
    if (argc < 2 || argc > 4) {
        usage();
    }

    switch (argc) {
        case 2:
            last = atof(argv[1]);
            break;
        case 3:
            first = atof(argv[1]);
            last = atof(argv[2]);
            break;
        case 4:
            first = atof(argv[1]);
            increment = atof(argv[2]);
            last = atof(argv[3]);
            break;
        default:
            usage();
    }

    // Validate increment
    if (increment == 0.0) {
        fprintf(stderr, "seq: increment cannot be zero\n");
        return 1;
    }

    // Generate the sequence
    for (double i = first; (increment > 0) ? (i <= last) : (i >= last); i += increment) {
        printf("%.0f\n", i); // Print as integer if no fractional part
    }

    return 0;
}

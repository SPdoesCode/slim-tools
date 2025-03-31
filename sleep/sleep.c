#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for sleep()

void usage() {
    fprintf(stderr, "Usage: sleep NUMBER[SUFFIX]\n");
    fprintf(stderr, "Pause for NUMBER seconds.\n");
    fprintf(stderr, "SUFFIX may be 's' for seconds (default), 'm' for minutes,\n");
    fprintf(stderr, "'h' for hours, or 'd' for days.\n");
    exit(1);
}

unsigned long parse_time(const char *arg) {
    char *endptr;
    double value = strtod(arg, &endptr); // Parse the numeric part

    if (endptr == arg || value < 0) {
        fprintf(stderr, "sleep: invalid time interval '%s'\n", arg);
        usage();
    }

    // Determine the suffix
    switch (*endptr) {
        case '\0': // No suffix, default to seconds
        case 's':  // Seconds
            return (unsigned long)value;
        case 'm':  // Minutes
            return (unsigned long)(value * 60);
        case 'h':  // Hours
            return (unsigned long)(value * 3600);
        case 'd':  // Days
            return (unsigned long)(value * 86400);
        default:
            fprintf(stderr, "sleep: invalid suffix '%c'\n", *endptr);
            usage();
    }

    // Fallback return (should never be reached due to usage())
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
    }

    unsigned long seconds = parse_time(argv[1]);

    // Sleep for the specified duration
    while (seconds > 0) {
        unsigned long delay = (seconds > 1000) ? 1000 : seconds; // Limit to 1000 seconds per call
        sleep(delay);
        seconds -= delay;
    }

    return 0;
}

#define _POSIX_C_SOURCE 200809L // Required for POSIX-compliant functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>
#include <time.h>

void usage() {
    fprintf(stderr, "Usage: who\n");
    fprintf(stderr, "Display information about users currently logged in.\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        usage();
    }

    struct utmp *entry;

    setutent(); // Open the utmp file for reading

    while ((entry = getutent()) != NULL) {
        if (entry->ut_type == USER_PROCESS) { // Only consider user login sessions
            // Format and print the entry
            char time_buffer[26];
            time_t timestamp = (time_t)entry->ut_tv.tv_sec; // Cast to time_t
            struct tm *time_info = localtime(&timestamp);

            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M", time_info);

            printf("%-8s %-12s %s\n", entry->ut_user, entry->ut_line, time_buffer);
        }
    }

    endutent(); // Close the utmp file

    return 0;
}

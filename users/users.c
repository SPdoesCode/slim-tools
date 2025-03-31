#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>

void usage() {
    fprintf(stderr, "Usage: users\n");
    fprintf(stderr, "Display the usernames of users currently logged in.\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        usage();
    }

    struct utmp *entry;
    char prev_user[UT_NAMESIZE + 1] = ""; // To track the previous username

    setutent(); // Open the utmp file for reading

    while ((entry = getutent()) != NULL) {
        if (entry->ut_type == USER_PROCESS) { // Only consider user login sessions
            if (strcmp(entry->ut_user, prev_user) != 0) { // Avoid duplicates
                printf("%s ", entry->ut_user);
                strncpy(prev_user, entry->ut_user, UT_NAMESIZE);
                prev_user[UT_NAMESIZE] = '\0'; // Ensure null termination
            }
        }
    }

    endutent(); // Close the utmp file
    printf("\n"); // Print a newline at the end

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h> // Required for utmp struct and functions
#include <time.h> // Required for ctime()
#include <unistd.h> // Required for getlogin()

void print_user_info(struct utmp *entry) {
    // Username
    printf("%-8s ", entry->ut_user);

    // Terminal
    printf("%-12s ", entry->ut_line);

    // Login time
    char *time_str = ctime((const time_t *)&entry->ut_tv.tv_sec); // Cast to time_t [[1]]
    if (time_str) {
        time_str[strcspn(time_str, "\n")] = '\0'; // Remove trailing newline
        printf("%-24s ", time_str);
    } else {
        printf("%-24s ", "Unknown");
    }

    // Remote host
    printf("%s\n", entry->ut_host[0] ? entry->ut_host : "localhost");
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused))) {
    // Open the utmp file
    setutent(); // Rewind to the beginning of the utmp file

    struct utmp *entry;
    int user_found = 0;

    // Iterate through the utmp entries
    while ((entry = getutent()) != NULL) {
        if (entry->ut_type == USER_PROCESS) { // Only process user login entries
            print_user_info(entry);
            user_found = 1;
        }
    }

    // Close the utmp file
    endutent();

    if (!user_found) {
        printf("No users logged in.\n");
    }

    return 0;
}

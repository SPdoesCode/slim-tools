#define _GNU_SOURCE // Enable GNU extensions to declare getgrent(), setgrent(), endgrent()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>

// Function to print groups for a given username
void print_groups(const char *username) {
    struct passwd *pw = getpwnam(username);
    if (!pw) {
        fprintf(stderr, "groups: '%s': no such user\n", username);
        exit(1);
    }

    gid_t gid = pw->pw_gid; // Primary group ID

    // Print the username
    printf("%s : ", username);

    // Print the primary group name
    struct group *grp = getgrgid(gid);
    if (grp) {
        printf("%s ", grp->gr_name);
    } else {
        printf("%d ", gid); // Fallback to GID if group name not found
    }

    // Manually find supplementary groups
    setgrent(); // Rewind to the beginning of the group database
    while ((grp = getgrent()) != NULL) {
        for (char **member = grp->gr_mem; *member != NULL; member++) {
            if (strcmp(*member, username) == 0) {
                printf("%s ", grp->gr_name);
                break;
            }
        }
    }
    endgrent(); // Close the group database

    printf("\n");
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: groups [USERNAME]\n");
        return 1;
    }

    const char *username = (argc == 2) ? argv[1] : getenv("USER");
    if (!username) {
        fprintf(stderr, "groups: cannot determine current user\n");
        return 1;
    }

    print_groups(username);

    return 0;
}

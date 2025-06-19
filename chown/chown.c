#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>      // For getpwnam()
#include <grp.h>      // For getgrnam()
#include <unistd.h>   // For chown()
#include <string.h>   // For strchr()

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("chgrp from SlimTools\n");
        printf("Usage: chown [OWNER][:[GROUP]] FILE...\n");
        printf("OWNER and GROUP can be names or numeric IDs\n");
        return 1;
    }

    // Parse the OWNER[:GROUP] argument
    char *owner_group = argv[1];
    char *colon = strchr(owner_group, ':');
    char *owner_str = owner_group;
    char *group_str = NULL;

    uid_t uid = -1;
    gid_t gid = -1;

    if (colon) {
        *colon = '\0'; // Split OWNER and GROUP at the colon
        group_str = colon + 1;
    }

    // Resolve OWNER (user)
    if (owner_str[0] != '\0') { // If OWNER is provided
        char *endptr;
        long owner_id = strtol(owner_str, &endptr, 10);
        if (*endptr == '\0') {
            // Numeric UID
            uid = (uid_t)owner_id;
        } else {
            // Username
            struct passwd *pw = getpwnam(owner_str);
            if (!pw) {
                printf("chown: invalid user '%s'\n", owner_str);
                return 1;
            }
            uid = pw->pw_uid;
        }
    }

    // Resolve GROUP (group)
    if (group_str && group_str[0] != '\0') { // If GROUP is provided
        char *endptr;
        long group_id = strtol(group_str, &endptr, 10);
        if (*endptr == '\0') {
            // Numeric GID
            gid = (gid_t)group_id;
        } else {
            // Group name
            struct group *gr = getgrnam(group_str);
            if (!gr) {
                printf("chown: invalid group '%s'\n", group_str);
                return 1;
            }
            gid = gr->gr_gid;
        }
    }

    // Change ownership for each file/directory provided
    for (int i = 2; i < argc; i++) {
        char *file = argv[i];
        if (chown(file, uid, gid) != 0) {
            perror("chown");
            return 1;
        }
    }

    return 0;
}

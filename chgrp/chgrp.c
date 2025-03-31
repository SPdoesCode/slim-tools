#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: chgrp GROUP FILE...\n");
        return 1;
    }

    // Get the group name from the first argument
    char *group_name = argv[1];
    struct group *grp = getgrnam(group_name);
    if (!grp) {
        printf("chgrp: invalid group '%s'\n", group_name);
        return 1;
    }

    gid_t gid = grp->gr_gid;

    // Change the group for each file/directory provided
    for (int i = 2; i < argc; i++) {
        char *file = argv[i];
        if (chown(file, -1, gid) != 0) {
            perror("chgrp");
            return 1;
        }
    }

    return 0;
}

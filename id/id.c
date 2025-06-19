#define _GNU_SOURCE // Enable GNU extensions, including getgrouplist()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

// Function to print user and group information
void print_id(const char *username) {
    struct passwd *pw;
    uid_t uid;
    gid_t gid;

    if (username) {
        // Get user information by username
        pw = getpwnam(username);
        if (!pw) {
            fprintf(stderr, "id: '%s': no such user\n", username);
            exit(1);
        }
        uid = pw->pw_uid;
        gid = pw->pw_gid;
    } else {
        // Get user information for the current user
        uid = getuid();
        gid = getgid();
        pw = getpwuid(uid);
    }

    // Print UID
    printf("uid=%d", uid);
    if (pw && pw->pw_name) {
        printf("(%s)", pw->pw_name);
    }

    // Print GID
    printf(" gid=%d", gid);
    struct group *grp = getgrgid(gid);
    if (grp && grp->gr_name) {
        printf("(%s)", grp->gr_name);
    }

    // Print supplementary groups
    int ngroups = 0;
    getgrouplist(username ? username : pw->pw_name, gid, NULL, &ngroups); // First call to determine size
    gid_t *groups = malloc(ngroups * sizeof(gid_t));
    if (!groups) {
        perror("id: malloc");
        exit(1);
    }
    if (getgrouplist(username ? username : pw->pw_name, gid, groups, &ngroups) == -1) {
        perror("id: getgrouplist");
        free(groups);
        exit(1);
    }

    printf(" groups=");
    for (int i = 0; i < ngroups; i++) {
        printf("%d", groups[i]);
        grp = getgrgid(groups[i]);
        if (grp && grp->gr_name) {
            printf("(%s)", grp->gr_name);
        }
        if (i < ngroups - 1) {
            printf(",");
        }
    }

    printf("\n");

    free(groups);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        printf("id from SlimTools\n");
        fprintf(stderr, "Usage: id [USERNAME]\n");
        return 1;
    }

    const char *username = (argc == 2) ? argv[1] : NULL;
    print_id(username);

    return 0;
}

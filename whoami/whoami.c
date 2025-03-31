#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

void usage() {
    fprintf(stderr, "Usage: whoami\n");
    fprintf(stderr, "Display the username of the current user.\n");
    exit(1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        usage();
    }

    // Get the effective user ID
    uid_t uid = geteuid();

    // Retrieve the password entry for the user ID
    struct passwd *pw = getpwuid(uid);
    if (!pw) {
        perror("whoami: getpwuid");
        return 1;
    }

    // Print the username
    printf("%s\n", pw->pw_name);

    return 0;
}

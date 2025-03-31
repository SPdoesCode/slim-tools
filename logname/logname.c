#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Required for getlogin()
#include <errno.h>

int main() {
    // Get the login name of the user
    char *login_name = getlogin();

    if (login_name == NULL) {
        if (errno == ENOTTY) {
            fprintf(stderr, "logname: no controlling terminal\n");
        } else {
            perror("logname: getlogin");
        }
        return 1;
    }

    // Print the login name
    printf("%s\n", login_name);

    return 0;
}

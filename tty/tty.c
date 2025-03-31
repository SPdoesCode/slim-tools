#include <stdio.h>
#include <unistd.h>

int main(void) {
    char *tty_name = ttyname(0); // Get the name of the terminal connected to stdin (file descriptor 0)
    if (tty_name) {
        printf("%s\n", tty_name);
        return 0; // Success
    } else {
        printf("not a tty\n");
        return 1; // Failure
    }
}

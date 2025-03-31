#define _DEFAULT_SOURCE // Enable default features, including gethostid()
#include <stdio.h>
#include <unistd.h> // Required for gethostid()

int main() {
    // Get the host ID using gethostid()
    long host_id = gethostid();

    // Print the host ID as an 8-character hexadecimal value
    printf("%08lx\n", (unsigned long)host_id);

    return 0;
}

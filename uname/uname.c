#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

void usage() {
    fprintf(stderr, "Usage: uname [OPTION]...\n");
    fprintf(stderr, "Print system information.\n");
    fprintf(stderr, "  -a, --all       Print all information\n");
    fprintf(stderr, "  -s, --kernel-name   Print the kernel name\n");
    fprintf(stderr, "  -n, --nodename      Print the network node hostname\n");
    fprintf(stderr, "  -r, --kernel-release   Print the kernel release\n");
    fprintf(stderr, "  -v, --kernel-version   Print the kernel version\n");
    fprintf(stderr, "  -m, --machine         Print the machine hardware name\n");
    exit(1);
}

void print_uname_info(int all, int kernel_name, int nodename, int kernel_release,
                      int kernel_version, int machine) {
    struct utsname info;
    if (uname(&info) == -1) {
        perror("uname");
        exit(1);
    }

    if (all || kernel_name) {
        printf("%s ", info.sysname);
    }
    if (all || nodename) {
        printf("%s ", info.nodename);
    }
    if (all || kernel_release) {
        printf("%s ", info.release);
    }
    if (all || kernel_version) {
        printf("%s ", info.version);
    }
    if (all || machine) {
        printf("%s", info.machine);
    }
    if (all || kernel_name || nodename || kernel_release || kernel_version || machine) {
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // Default behavior: print kernel name
        print_uname_info(0, 1, 0, 0, 0, 0);
        return 0;
    }

    int all = 0, kernel_name = 0, nodename = 0, kernel_release = 0, kernel_version = 0, machine = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            all = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--kernel-name") == 0) {
            kernel_name = 1;
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--nodename") == 0) {
            nodename = 1;
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--kernel-release") == 0) {
            kernel_release = 1;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--kernel-version") == 0) {
            kernel_version = 1;
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--machine") == 0) {
            machine = 1;
        } else {
            usage();
        }
    }

    // If no options are specified, default to printing the kernel name
    if (!all && !kernel_name && !nodename && !kernel_release && !kernel_version && !machine) {
        kernel_name = 1;
    }

    print_uname_info(all, kernel_name, nodename, kernel_release, kernel_version, machine);

    return 0;
}

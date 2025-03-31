#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void usage() {
    fprintf(stderr, "Usage: test EXPRESSION\n");
    fprintf(stderr, "Evaluate EXPRESSION and return 0 (true) or 1 (false).\n");
    fprintf(stderr, "Supported operators:\n");
    fprintf(stderr, "  -e FILE     True if FILE exists.\n");
    fprintf(stderr, "  -f FILE     True if FILE exists and is a regular file.\n");
    fprintf(stderr, "  -d FILE     True if FILE exists and is a directory.\n");
    fprintf(stderr, "  STRING      True if STRING is not empty.\n");
    fprintf(stderr, "  -z STRING   True if STRING is empty.\n");
    fprintf(stderr, "  -n STRING   True if STRING is not empty.\n");
    fprintf(stderr, "  STRING1 = STRING2   True if strings are equal.\n");
    fprintf(stderr, "  STRING1 != STRING2  True if strings are not equal.\n");
    fprintf(stderr, "  INT1 -eq INT2       True if integers are equal.\n");
    fprintf(stderr, "  INT1 -ne INT2       True if integers are not equal.\n");
    fprintf(stderr, "  INT1 -lt INT2       True if INT1 < INT2.\n");
    fprintf(stderr, "  INT1 -le INT2       True if INT1 <= INT2.\n");
    fprintf(stderr, "  INT1 -gt INT2       True if INT1 > INT2.\n");
    fprintf(stderr, "  INT1 -ge INT2       True if INT1 >= INT2.\n");
    exit(2);
}

int evaluate_expression(int argc, char **argv) {
    if (argc < 2) {
        return 1; // No expression provided, return false
    }

    char *arg1 = argv[0];
    char *arg2 = argv[1];
    char *arg3 = argc > 2 ? argv[2] : NULL;

    // File tests
    if (strcmp(arg1, "-e") == 0) {
        struct stat sb;
        return stat(arg2, &sb) == 0; // True if file exists
    } else if (strcmp(arg1, "-f") == 0) {
        struct stat sb;
        return stat(arg2, &sb) == 0 && S_ISREG(sb.st_mode); // True if file exists and is regular
    } else if (strcmp(arg1, "-d") == 0) {
        struct stat sb;
        return stat(arg2, &sb) == 0 && S_ISDIR(sb.st_mode); // True if file exists and is directory
    }

    // String tests
    if (strcmp(arg1, "-z") == 0) {
        return strlen(arg2) == 0; // True if string is empty
    } else if (strcmp(arg1, "-n") == 0) {
        return strlen(arg2) != 0; // True if string is not empty
    } else if (arg2 && strcmp(arg2, "=") == 0) {
        return strcmp(arg1, arg3) == 0; // True if strings are equal
    } else if (arg2 && strcmp(arg2, "!=") == 0) {
        return strcmp(arg1, arg3) != 0; // True if strings are not equal
    }

    // Integer tests
    if (arg2 && strcmp(arg2, "-eq") == 0) {
        return atoi(arg1) == atoi(arg3); // True if integers are equal
    } else if (arg2 && strcmp(arg2, "-ne") == 0) {
        return atoi(arg1) != atoi(arg3); // True if integers are not equal
    } else if (arg2 && strcmp(arg2, "-lt") == 0) {
        return atoi(arg1) < atoi(arg3); // True if arg1 < arg3
    } else if (arg2 && strcmp(arg2, "-le") == 0) {
        return atoi(arg1) <= atoi(arg3); // True if arg1 <= arg3
    } else if (arg2 && strcmp(arg2, "-gt") == 0) {
        return atoi(arg1) > atoi(arg3); // True if arg1 > arg3
    } else if (arg2 && strcmp(arg2, "-ge") == 0) {
        return atoi(arg1) >= atoi(arg3); // True if arg1 >= arg3
    }

    // Default: check if the argument is non-empty
    return strlen(arg1) != 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    int result = evaluate_expression(argc - 1, argv + 1);
    return result ? 0 : 1; // Return 0 for true, 1 for false
}

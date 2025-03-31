#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Helper function to apply symbolic mode changes
void apply_symbolic_mode(const char *mode, struct stat *file_stat) {
    char op = '\0'; // Operation: '+', '-', '='
    char who[4] = {0}; // Affected users: 'u', 'g', 'o', 'a'
    char perms[4] = {0}; // Permissions: 'r', 'w', 'x'

    // Parse the symbolic mode format (e.g., "u+x", "+x", "go-w")
    if (sscanf(mode, "%[ugoa]%c%[rwx]", who, &op, perms) != 3) {
        // If no user/group specifier is provided, default to "ugo"
        if (sscanf(mode, "%c%[rwx]", &op, perms) == 2) {
            strcpy(who, "ugo");
        } else {
            printf("chmod: invalid mode '%s'\n", mode);
            exit(1);
        }
    }

    // Apply changes based on the operation
    for (int i = 0; who[i]; i++) {
        int mask = 0;
        switch (who[i]) {
            case 'u': mask = S_IRWXU; break; // User bits
            case 'g': mask = S_IRWXG; break; // Group bits
            case 'o': mask = S_IRWXO; break; // Others bits
            case 'a': mask = S_IRWXU | S_IRWXG | S_IRWXO; break; // All bits
            default:
                printf("chmod: invalid user/group specifier '%c'\n", who[i]);
                exit(1);
        }

        for (int j = 0; perms[j]; j++) {
            int perm_mask = 0;
            switch (perms[j]) {
                case 'r': perm_mask = S_IRUSR >> (i * 3); break;
                case 'w': perm_mask = S_IWUSR >> (i * 3); break;
                case 'x': perm_mask = S_IXUSR >> (i * 3); break;
                default:
                    printf("chmod: invalid permission specifier '%c'\n", perms[j]);
                    exit(1);
            }

            switch (op) {
                case '+': file_stat->st_mode |= perm_mask; break;
                case '-': file_stat->st_mode &= ~perm_mask; break;
                case '=': 
                    file_stat->st_mode &= ~mask;
                    file_stat->st_mode |= perm_mask;
                    break;
                default:
                    printf("chmod: invalid operation '%c'\n", op);
                    exit(1);
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: chmod MODE FILE...\n");
        printf("MODE can be an octal number (e.g., 755) or a symbolic mode (e.g., u+x)\n");
        return 1;
    }

    const char *mode_arg = argv[1];
    struct stat file_stat;

    // Change permissions for each file provided
    for (int i = 2; i < argc; i++) {
        char *file = argv[i];

        // Get the current file's permissions
        if (stat(file, &file_stat) != 0) {
            perror("chmod: stat");
            return 1;
        }

        // Handle octal mode
        if (strchr("01234567", mode_arg[0])) {
            char *endptr;
            long mode = strtol(mode_arg, &endptr, 8); // Base 8 for octal
            if (*endptr != '\0' || mode < 0 || mode > 0777) {
                printf("chmod: invalid mode '%s'\n", mode_arg);
                return 1;
            }
            file_stat.st_mode = (file_stat.st_mode & ~0777) | mode; // Apply octal mode
        } else {
            // Handle symbolic mode
            apply_symbolic_mode(mode_arg, &file_stat);
        }

        // Update the file's permissions
        if (chmod(file, file_stat.st_mode) != 0) {
            perror("chmod");
            return 1;
        }
    }

    return 0;
}

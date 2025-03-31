#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> // For off_t
#include <pwd.h>       // For getpwuid()
#include <grp.h>       // For getgrgid()

#define MAX_PATH 4096

// Function to format file size into human-readable form
const char *format_size(off_t size) {
    static char buffer[32]; // Increased buffer size to 32 to avoid truncation
    if (size < 1024) {
        snprintf(buffer, sizeof(buffer), "%ld B", (long)size);
    } else if (size < 1024 * 1024) {
        snprintf(buffer, sizeof(buffer), "%.1f KB", (double)size / 1024);
    } else if (size < 1024 * 1024 * 1024) {
        snprintf(buffer, sizeof(buffer), "%.1f MB", (double)size / (1024 * 1024));
    } else {
        snprintf(buffer, sizeof(buffer), "%.1f GB", (double)size / (1024 * 1024 * 1024));
    }
    return buffer;
}

// Function to display directory contents
void list_directory(const char *path, int long_format) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("dir: opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Get file stats
        struct stat st;
        if (stat(full_path, &st) < 0) {
            perror("dir: stat");
            continue;
        }

        // Print details in long format
        if (long_format) {
            // File type
            const char *type = S_ISDIR(st.st_mode) ? "d" : "-";

            // Permissions
            char perms[11] = "----------";
            if (st.st_mode & S_IRUSR) perms[1] = 'r';
            if (st.st_mode & S_IWUSR) perms[2] = 'w';
            if (st.st_mode & S_IXUSR) perms[3] = 'x';
            if (st.st_mode & S_IRGRP) perms[4] = 'r';
            if (st.st_mode & S_IWGRP) perms[5] = 'w';
            if (st.st_mode & S_IXGRP) perms[6] = 'x';
            if (st.st_mode & S_IROTH) perms[7] = 'r';
            if (st.st_mode & S_IWOTH) perms[8] = 'w';
            if (st.st_mode & S_IXOTH) perms[9] = 'x';

            // Format modification time
            char mod_time[20];
            strftime(mod_time, sizeof(mod_time), "%b %d %H:%M", localtime(&st.st_mtime));

            // Get user and group names
            struct passwd *pwd = getpwuid(st.st_uid);
            struct group *grp = getgrgid(st.st_gid);

            // Print long format details
            printf("%s%s %3ld %-8s %-8s %8s %s %s\n",
                   type, perms, (long)st.st_nlink,
                   pwd ? pwd->pw_name : "?", grp ? grp->gr_name : "?",
                   format_size(st.st_size), mod_time, entry->d_name);
        } else {
            // Print simple format
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {
    int long_format = 0;
    const char *path = ".";

    // Parse options
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "dir: invalid option '%s'\n", argv[i]);
            return 1;
        } else {
            path = argv[i];
        }
    }

    // List directory contents
    list_directory(path, long_format);

    return 0;
}

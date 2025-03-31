#define _DEFAULT_SOURCE // Enable d_type and other POSIX features [[1]]
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// Function to print file permissions
void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

// Function to convert file size to human-readable format
const char *human_readable_size(off_t size) {
    static char buf[20];
    if (size >= (1 << 30)) {
        snprintf(buf, sizeof(buf), "%.1fG", (double)size / (1 << 30));
    } else if (size >= (1 << 20)) {
        snprintf(buf, sizeof(buf), "%.1fM", (double)size / (1 << 20));
    } else if (size >= (1 << 10)) {
        snprintf(buf, sizeof(buf), "%.1fK", (double)size / (1 << 10));
    } else {
        snprintf(buf, sizeof(buf), "%ld", (long)size);
    }
    return buf;
}

// Comparison functions for qsort
static int compare_by_time(const struct dirent **a, const struct dirent **b) {
    struct stat st_a, st_b;
    char path_a[PATH_MAX], path_b[PATH_MAX];
    snprintf(path_a, sizeof(path_a), "%s/%s", ".", (*a)->d_name);
    snprintf(path_b, sizeof(path_b), "%s/%s", ".", (*b)->d_name);
    stat(path_a, &st_a);
    stat(path_b, &st_b);
    return (st_b.st_mtime - st_a.st_mtime);
}

static int compare_by_size(const struct dirent **a, const struct dirent **b) {
    struct stat st_a, st_b;
    char path_a[PATH_MAX], path_b[PATH_MAX];
    snprintf(path_a, sizeof(path_a), "%s/%s", ".", (*a)->d_name);
    snprintf(path_b, sizeof(path_b), "%s/%s", ".", (*b)->d_name);
    stat(path_a, &st_a);
    stat(path_b, &st_b);
    return (st_b.st_size - st_a.st_size);
}

void list_directory(const char *dir_path, int show_hidden, int almost_all,
                    int long_listing, int human_readable, int sort_by_time,
                    int reverse_order, int classify, int list_dirs_only,
                    int sort_by_size) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("ls: opendir");
        exit(1);
    }

    struct dirent **entries = NULL;
    int num_entries = scandir(dir_path, &entries, NULL, alphasort);
    if (num_entries == -1) {
        perror("ls: scandir");
        closedir(dir);
        exit(1);
    }

    // Apply sorting
    if (sort_by_time) {
        qsort(entries, num_entries, sizeof(struct dirent *), (int (*)(const void *, const void *))compare_by_time);
    } else if (sort_by_size) {
        qsort(entries, num_entries, sizeof(struct dirent *), (int (*)(const void *, const void *))compare_by_size);
    }

    // Reverse order if requested
    if (reverse_order) {
        for (int i = 0; i < num_entries / 2; i++) {
            struct dirent *tmp = entries[i];
            entries[i] = entries[num_entries - i - 1];
            entries[num_entries - i - 1] = tmp;
        }
    }

    for (int i = 0; i < num_entries; i++) {
        struct dirent *entry = entries[i];
        struct stat st;
        char path[PATH_MAX];

        // Skip entries based on flags
        if (!show_hidden && !almost_all && entry->d_name[0] == '.') continue;
        if (almost_all && (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))) continue;

        // Handle directory listing mode (-d)
        if (list_dirs_only) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            if (stat(path, &st) == -1 || !S_ISDIR(st.st_mode)) continue;
        }

        // Get file metadata
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        if (stat(path, &st) == -1) {
            perror("ls: stat");
            continue;
        }

        // Long listing format
        if (long_listing) {
            print_permissions(st.st_mode);
            printf(" %ld ", (long)st.st_nlink);

            // Owner and group
            struct passwd *pw = getpwuid(st.st_uid);
            struct group *grp = getgrgid(st.st_gid);
            printf("%s %s ", pw ? pw->pw_name : "?", grp ? grp->gr_name : "?");

            // Size
            if (human_readable) {
                printf("%8s ", human_readable_size(st.st_size));
            } else {
                printf("%8ld ", (long)st.st_size);
            }

            // Modification time
            char timebuf[20];
            strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));
            printf("%s ", timebuf);

            // Name and classification
            printf("%s", entry->d_name);
            if (classify) {
                if (S_ISDIR(st.st_mode)) printf("/");
                else if (S_ISLNK(st.st_mode)) printf("@");
                else if (st.st_mode & S_IXUSR) printf("*");
            }
            printf("\n");
        } else {
            // Short listing with classification
            printf("%s", entry->d_name);
            if (classify) {
                if (S_ISDIR(st.st_mode)) printf("/");
                else if (S_ISLNK(st.st_mode)) printf("@");
                else if (st.st_mode & S_IXUSR) printf("*");
            }
            printf("\n");
        }
    }

    // Clean up
    for (int i = 0; i < num_entries; i++) free(entries[i]);
    free(entries);
    closedir(dir);
}

int main(int argc, char **argv) {
    int show_hidden = 0; // -a option
    int almost_all = 0; // -A option
    int long_listing = 0; // -l option
    int human_readable = 0; // -h option
    int sort_by_time = 0; // -t option
    int reverse_order = 0; // -r option
    int classify = 0; // -F option
    int list_dirs_only = 0; // -d option
    int sort_by_size = 0; // -S option
    const char *dir_path = "."; // Default to current directory

    // Parse arguments
    int opt;
    while ((opt = getopt(argc, argv, "aAlhtrFdS")) != -1) {
        switch (opt) {
            case 'a':
                show_hidden = 1;
                break;
            case 'A':
                almost_all = 1;
                break;
            case 'l':
                long_listing = 1;
                break;
            case 'h':
                human_readable = 1;
                break;
            case 't':
                sort_by_time = 1;
                break;
            case 'r':
                reverse_order = 1;
                break;
            case 'F':
                classify = 1;
                break;
            case 'd':
                list_dirs_only = 1;
                break;
            case 'S':
                sort_by_size = 1;
                break;
            default:
                fprintf(stderr, "Usage: ls [-a] [-A] [-l] [-h] [-t] [-r] [-F] [-d] [-S] [DIRECTORY]\n");
                return 1;
        }
    }

    // Check for directory argument
    if (optind < argc) {
        dir_path = argv[optind];
    }

    // List the directory contents
    list_directory(dir_path, show_hidden, almost_all, long_listing, human_readable,
                   sort_by_time, reverse_order, classify, list_dirs_only, sort_by_size);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void usage() {
    fprintf(stderr, "Usage: stat FILE\n");
    fprintf(stderr, "Display file or filesystem status.\n");
    exit(1);
}

void print_permissions(mode_t mode) {
    printf("Permissions: ");
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
    printf("\n");
}

void print_file_type(mode_t mode) {
    printf("File Type: ");
    if (S_ISREG(mode)) {
        printf("Regular File\n");
    } else if (S_ISDIR(mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(mode)) {
        printf("Symbolic Link\n");
    } else if (S_ISCHR(mode)) {
        printf("Character Device\n");
    } else if (S_ISBLK(mode)) {
        printf("Block Device\n");
    } else if (S_ISFIFO(mode)) {
        printf("FIFO/Named Pipe\n");
    } else if (S_ISSOCK(mode)) {
        printf("Socket\n");
    } else {
        printf("Unknown\n");
    }
}

void print_timestamp(const char *label, time_t timestamp) {
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    printf("%s: %s\n", label, buffer);
}

void stat_file(const char *filename) {
    struct stat sb;

    if (stat(filename, &sb) == -1) {
        perror("stat");
        exit(1);
    }

    printf("File: %s\n", filename);
    print_file_type(sb.st_mode);
    print_permissions(sb.st_mode);
    printf("Size: %ld bytes\n", (long)sb.st_size);
    printf("Owner UID: %d\n", sb.st_uid);
    printf("Group GID: %d\n", sb.st_gid);
    printf("Device ID: %ld\n", (long)sb.st_dev);
    printf("Inode: %ld\n", (long)sb.st_ino);
    printf("Links: %ld\n", (long)sb.st_nlink);

    print_timestamp("Access Time", sb.st_atime);
    print_timestamp("Modify Time", sb.st_mtime);
    print_timestamp("Change Time", sb.st_ctime);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
    }

    stat_file(argv[1]);

    return 0;
}

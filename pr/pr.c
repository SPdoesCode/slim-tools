#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // Required for getopt(), optarg, optind

#define DEFAULT_PAGE_WIDTH 72
#define DEFAULT_LINES_PER_PAGE 66
#define DEFAULT_COLUMNS 1

void usage() {
    fprintf(stderr, "Usage: pr [-w WIDTH] [-l LINES] [-t] [FILE...]\n");
    exit(1);
}

void print_header(const char *filename, int page_number) {
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    if (time_str) {
        time_str[strcspn(time_str, "\n")] = '\0'; // Remove trailing newline
    }

    printf("\n%s %s Page %d\n\n", filename ? filename : "(stdin)", time_str ? time_str : "", page_number);
}

void process_file(FILE *file, const char *filename, int width, int lines_per_page, int omit_header) {
    char line[4096];
    int line_count = 0;
    int page_number = 1;

    if (!omit_header) {
        print_header(filename, page_number);
    }

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Print the line, padded to the specified width
        printf("%-*s\n", width, line);

        line_count++;
        if (line_count >= lines_per_page) {
            line_count = 0;
            page_number++;
            if (!omit_header) {
                print_header(filename, page_number);
            }
        }
    }

    // Add a form feed at the end of the file
    printf("\f");
}

int main(int argc, char **argv) {
    int width = DEFAULT_PAGE_WIDTH;
    int lines_per_page = DEFAULT_LINES_PER_PAGE;
    int omit_header = 0;
    int opt;

    // Parse options
    while ((opt = getopt(argc, argv, "w:l:t")) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                if (width <= 0) {
                    fprintf(stderr, "pr: invalid width '%s'\n", optarg);
                    usage();
                }
                break;
            case 'l':
                lines_per_page = atoi(optarg);
                if (lines_per_page <= 0) {
                    fprintf(stderr, "pr: invalid lines per page '%s'\n", optarg);
                    usage();
                }
                break;
            case 't':
                omit_header = 1;
                break;
            default:
                usage();
        }
    }

    // Process files
    if (optind >= argc) {
        // No files provided, read from stdin
        process_file(stdin, NULL, width, lines_per_page, omit_header);
    } else {
        for (int i = optind; i < argc; i++) {
            const char *filename = argv[i];
            FILE *file = strcmp(filename, "-") == 0 ? stdin : fopen(filename, "r");
            if (!file) {
                perror("pr: fopen");
                continue;
            }
            process_file(file, filename, width, lines_per_page, omit_header);
            if (file != stdin) {
                fclose(file);
            }
        }
    }

    return 0;
}

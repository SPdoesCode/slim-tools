#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void usage() {
    fprintf(stderr, "Usage: printf FORMAT [ARG...]\n");
    exit(1);
}

// Helper function to process escape sequences
char process_escape_sequence(const char **format) {
    const char *p = *format;
    if (*p == '\\') {
        p++;
        switch (*p) {
            case 'a': *format = p + 1; return '\a'; // Alert (bell)
            case 'b': *format = p + 1; return '\b'; // Backspace
            case 'f': *format = p + 1; return '\f'; // Form feed
            case 'n': *format = p + 1; return '\n'; // Newline
            case 'r': *format = p + 1; return '\r'; // Carriage return
            case 't': *format = p + 1; return '\t'; // Tab
            case 'v': *format = p + 1; return '\v'; // Vertical tab
            case '\\': *format = p + 1; return '\\'; // Backslash
            case '0': { // Octal escape sequence
                int value = 0;
                p++;
                while (*p >= '0' && *p <= '7') {
                    value = value * 8 + (*p - '0');
                    p++;
                }
                *format = p;
                return (char)value;
            }
            default: *format = p; return '\\'; // Unrecognized escape
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage();
    }

    const char *format = argv[1];
    int arg_index = 2;

    while (*format) {
        if (*format == '%') {
            format++; // Move past '%'
            switch (*format) {
                case '%': // Literal '%'
                    putchar('%');
                    break;
                case 'b': { // Process escape sequences in argument
                    if (arg_index < argc) {
                        const char *arg = argv[arg_index++];
                        while (*arg) {
                            char c = process_escape_sequence(&arg);
                            if (c) {
                                putchar(c);
                            } else {
                                putchar(*arg);
                                arg++;
                            }
                        }
                    }
                    break;
                }
                case 's': { // String argument
                    if (arg_index < argc) {
                        fputs(argv[arg_index++], stdout);
                    }
                    break;
                }
                case 'd': { // Integer argument
                    if (arg_index < argc) {
                        int value = atoi(argv[arg_index++]);
                        printf("%d", value);
                    }
                    break;
                }
                case 'c': { // Character argument
                    if (arg_index < argc) {
                        putchar(argv[arg_index++][0]);
                    }
                    break;
                }
                default: { // Unsupported format specifier
                    fprintf(stderr, "printf: unsupported format specifier '%%%c'\n", *format);
                    return 1;
                }
            }
            format++; // Move past format specifier
        } else if (*format == '\\') {
            // Handle escape sequences in the format string
            char c = process_escape_sequence(&format);
            if (c) {
                putchar(c);
            }
        } else {
            // Regular character
            putchar(*format++);
        }
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_TABSTOP 8 // Default tab stop width

// Function to evaluate arithmetic expressions
long evaluate_arithmetic(const char *expr) {
    long a, b;
    char op;

    // Parse the expression
    if (sscanf(expr, "%ld %c %ld", &a, &op, &b) != 3) {
        fprintf(stderr, "expr: invalid expression\n");
        exit(1);
    }

    // Perform the operation
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                fprintf(stderr, "expr: division by zero\n");
                exit(1);
            }
            return a / b;
        case '%':
            if (b == 0) {
                fprintf(stderr, "expr: modulo by zero\n");
                exit(1);
            }
            return a % b;
        default:
            fprintf(stderr, "expr: unknown operator '%c'\n", op);
            exit(1);
    }
}

// Function to evaluate string length
long evaluate_strlen(const char *expr) {
    const char *str = expr + strlen("length(");
    size_t len = strlen(str);

    // Remove trailing ')'
    if (len > 0 && str[len - 1] == ')') {
        return len - 1;
    }

    fprintf(stderr, "expr: invalid string expression\n");
    exit(1);
}

// Function to evaluate substring extraction
void evaluate_substr(const char *expr) {
    const char *str = expr + strlen("substr(");
    size_t start, length; // Changed to size_t
    char buffer[1024];

    // Parse the arguments
    if (sscanf(str, "%[^,],%zu,%zu", buffer, &start, &length) != 3) { // Use %zu for size_t
        fprintf(stderr, "expr: invalid substring expression\n");
        exit(1);
    }

    // Validate indices
    size_t str_len = strlen(buffer);
    if (start < 1 || start > str_len || length == 0 || start + length - 1 > str_len) {
        fprintf(stderr, "expr: invalid substring indices\n");
        exit(1);
    }

    // Print the substring
    printf("%.*s\n", (int)length, buffer + start - 1); // Cast length to int for %.*s
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: expr EXPRESSION\n");
        return 1;
    }

    const char *expression = argv[1];

    // Check for arithmetic operations
    if (strchr(expression, '+') || strchr(expression, '-') ||
        strchr(expression, '*') || strchr(expression, '/') ||
        strchr(expression, '%')) {
        printf("%ld\n", evaluate_arithmetic(expression));
        return 0;
    }

    // Check for string length
    if (strncmp(expression, "length(", 7) == 0) {
        printf("%ld\n", evaluate_strlen(expression));
        return 0;
    }

    // Check for substring extraction
    if (strncmp(expression, "substr(", 7) == 0) {
        evaluate_substr(expression);
        return 0;
    }

    // If no valid operation is found
    fprintf(stderr, "expr: invalid expression\n");
    return 1;
}

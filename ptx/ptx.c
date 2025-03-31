#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 256
#define MAX_LINE_LENGTH 4096

void usage() {
    fprintf(stderr, "Usage: ptx [FILE]\n");
    exit(1);
}

// Function to normalize a word (convert to lowercase and remove punctuation)
void normalize_word(char *word) {
    for (int i = 0; word[i]; i++) {
        if (ispunct(word[i])) {
            word[i] = '\0'; // Remove punctuation
            break;
        }
        word[i] = tolower(word[i]); // Convert to lowercase
    }
}

// Function to process a line and generate the permuted index
void process_line(const char *line) {
    char words[100][MAX_WORD_LENGTH]; // Array to store words
    int word_count = 0;

    // Tokenize the line into words
    const char *delimiters = " \t\n";
    char *copy = strdup(line); // Create a mutable copy of the line
    char *token = strtok(copy, delimiters);

    while (token && word_count < 100) {
        strncpy(words[word_count], token, MAX_WORD_LENGTH - 1);
        words[word_count][MAX_WORD_LENGTH - 1] = '\0';
        normalize_word(words[word_count]);
        word_count++;
        token = strtok(NULL, delimiters);
    }

    free(copy);

    // Generate permutations for each word
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue; // Skip empty words

        // Rotate the words array
        char rotated[100][MAX_WORD_LENGTH];
        for (int j = 0; j < word_count; j++) {
            strcpy(rotated[j], words[(i + j) % word_count]);
        }

        // Print the permuted index entry
        printf("%s ", rotated[0]);
        for (int j = 1; j < word_count; j++) {
            printf("%s ", rotated[j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    FILE *file = stdin; // Default to stdin if no file is provided

    if (argc > 2) {
        usage();
    }

    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("ptx: fopen");
            return 1;
        }
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        process_line(line);
    }

    if (ferror(file)) {
        perror("ptx: fgets");
        if (file != stdin) fclose(file);
        return 1;
    }

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to display the current date and time
void display_date(const char *format) {
    time_t raw_time;
    struct tm *time_info;

    // Get the current time
    time(&raw_time);
    time_info = localtime(&raw_time);

    // Print the formatted date and time
    if (format) {
        char buffer[256];
        strftime(buffer, sizeof(buffer), format, time_info);
        printf("%s\n", buffer);
    } else {
        // Default format: "Wed Oct 15 14:30:00 UTC 2023"
        printf("%s", asctime(time_info));
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        printf("Usage: date [FORMAT]\n");
        printf("Display the current date and time.\n");
        printf("If FORMAT is provided, use it to format the output.\n");
        return 1;
    }

    const char *format = NULL;

    // Check if a custom format is provided
    if (argc == 2) {
        format = argv[1];
    }

    // Display the date and time
    display_date(format);

    return 0;
}

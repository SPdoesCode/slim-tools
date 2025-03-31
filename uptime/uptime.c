#define _POSIX_C_SOURCE 200809L // Required for POSIX-compliant functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

void uptime() {
    struct sysinfo info;
    if (sysinfo(&info) == -1) {
        perror("uptime: sysinfo");
        exit(1);
    }

    // Get current time
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    // Calculate uptime in days, hours, and minutes
    int uptime_seconds = info.uptime;
    int days = uptime_seconds / (60 * 60 * 24);
    int hours = (uptime_seconds % (60 * 60 * 24)) / (60 * 60);
    int minutes = (uptime_seconds % (60 * 60)) / 60;

    // Print uptime
    printf(" %02d:%02d:%02d up ", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    if (days > 0) {
        printf("%d day%s, ", days, days > 1 ? "s" : "");
    }
    printf("%d:%02d", hours, minutes);

    // Print number of users
    FILE *utmp = fopen("/var/run/utmp", "r");
    if (!utmp) {
        perror("uptime: fopen");
        exit(1);
    }

    int user_count = 0;
    char buffer[1024];
    while (fread(buffer, sizeof(buffer), 1, utmp) > 0) {
        user_count++;
    }
    fclose(utmp);

    printf(",  %d user%s", user_count, user_count != 1 ? "s" : "");

    // Print load averages
    double loads[3];
    if (getloadavg(loads, 3) == -1) {
        perror("uptime: getloadavg");
        exit(1);
    }

    printf(",  load average: %.2f, %.2f, %.2f\n", loads[0], loads[1], loads[2]);
}

int main(void) {
    uptime();
    return 0;
}

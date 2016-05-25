#include <iostream>
#include "StringTime.h"
#include <ctime>
#include <cstring>

void printTime()
{
    time_t systemTime;
    time(&systemTime);
    tm *timeTm  = localtime(&systemTime);
    int hour    = timeTm->tm_hour;
    int minutes = timeTm->tm_min;
    int seconds = timeTm->tm_sec;
    std::cout << hour << ':' << minutes << ':' << seconds << space;
    std::cout << GenTimeCStyle(hour, minutes, seconds) << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printTime();
        exit(EXIT_SUCCESS);
    }

    if (!strcmp(argv[1], "--loop"))
        while (true)
            printTime();

    if (!strcmp(argv[1], "--debug")) {
        int seconds = 0;
        for (int hour = 0; hour < 24; ++ hour)
        for (int minutes = 1; minutes < 60; minutes += 5) {
            std::cout << hour << ':' << minutes << space;
            std::cout << GenTimeCStyle(hour, minutes, seconds) << std::endl;
        };
    }

    return 0;
}

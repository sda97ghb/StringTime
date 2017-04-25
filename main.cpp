#include <iomanip>
#include <iostream>
#include <ctime>

#include "MyUtf8.h"
#include "StringTime.h"

void printTime(uint8_t hour, uint8_t minutes, uint8_t seconds)
{
    std::wcout << std::setw(2) << std::setfill(L'0') << hour << ':'
               << std::setw(2) << std::setfill(L'0') << minutes << ':'
               << std::setw(2) << std::setfill(L'0') << seconds << ' ';
    std::wcout << timeToString(hour, minutes, seconds) << std::endl;
}

void printTime()
{
    time_t systemTime;
    time(&systemTime);
    tm* timeTm  = localtime(&systemTime);

    uint8_t hour    = timeTm->tm_hour;
    uint8_t minutes = timeTm->tm_min;
    uint8_t seconds = timeTm->tm_sec;

    printTime(hour, minutes, seconds);
}

int main(int argc, char **argv)
{
    my_utf8::setConsoleToUtf8();

    if (argc < 2)
    {
        printTime();
        exit(EXIT_SUCCESS);
    }

    if (std::string(argv[1]) == "--loop")
        while (true)
            printTime();

    if (std::string(argv[1]) == "--debug")
    {
        int seconds = 0;
        for (int hour = 0; hour < 24; ++ hour)
        {
            for (int minutes = 1; minutes < 60; minutes += 5)
                printTime(hour, minutes, seconds);
            int minutes = 59;
            printTime(hour, minutes, seconds);
        }
    }

    return 0;
}

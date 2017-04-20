#include <iostream>
#include <ctime>

#include "MyUtf8.h"
#include "StringTime.h"

void printTime()
{
    time_t systemTime;
    time(&systemTime);
    tm* timeTm  = localtime(&systemTime);
    int hour    = timeTm->tm_hour;
    int minutes = timeTm->tm_min;
    int seconds = timeTm->tm_sec;
    std::wcout << hour << ':' << minutes << ':' << seconds << ' ';
    std::wcout << timeToString(hour, minutes, seconds) << std::endl;
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
        for (int minutes = 1; minutes < 60; minutes += 5)
        {
            std::wcout << hour << ':' << minutes << ' ';
            std::wcout << timeToString(hour, minutes, seconds) << std::endl;
        };
    }

    return 0;
}

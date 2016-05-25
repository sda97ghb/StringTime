#include <iostream>
#include "StringTime.h"
#include <ctime>

int main() {
    time_t systemTime;
    time(&systemTime);
    int seconds = 0;
    for (int hour = 0; hour < 24; ++ hour)
    for (int minutes = 1; minutes < 60; minutes += 5) {
        std::cout << hour << ':' << minutes << space;
        std::cout << GenTimeCStyle(hour, minutes, seconds) << std::endl;
    };
    return 0;
}

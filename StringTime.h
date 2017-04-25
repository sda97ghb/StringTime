#ifndef STRINGTIME_H
#define STRINGTIME_H

#include <string>

/// @brief Отображение указанного времени в разговорной форме.
std::wstring timeToString(uint8_t hour, uint8_t minutes, uint8_t seconds);

#endif // STRINGTIME_H

#include "StringTime.h"

bool OverTime(char minutes, char seconds, char minMinutes, char minSeconds) {
    return !(minutes < minMinutes || (minutes == minMinutes && seconds < minSeconds));
}

std::pair<std::string, MyTimeOrder> GenMinutes(char minutes, char seconds) {
    for(unsigned i = 0; i < minutesStrings.size(); ++ i)
        if (OverTime(minutes, seconds, std::get<0>(minutesStrings.at(i)), std::get<1>(minutesStrings.at(i))))
            return std::pair<std::string, MyTimeOrder>(std::get<2>(minutesStrings.at(i)), std::get<3>(minutesStrings.at(i)));
    return std::pair<std::string, MyTimeOrder>("MINUTESERROR", MyTimeOrder::current);
}

std::string GenHour(char hour, MyTimeOrder timeOrder) {
    if (hour > 12) hour -= 12;
    if (hour < 0 || hour > 12) return "HOURERROR";
    switch (timeOrder) {
        case MyTimeOrder::current: return hoursStrings[hour][0]; break;
        case MyTimeOrder::nextBegin: return hoursStrings[hour][1]; break;
        case MyTimeOrder::nextEnd: return hoursStrings[hour][1]; break;
        case MyTimeOrder::nextGenetive: return hoursStrings[hour][2]; break;
    };
    return "HOURERROR";
}

std::string GenTime(char hour, char minutes, char seconds){
    std::pair<std::string, MyTimeOrder> a = GenMinutes(minutes, seconds);
    std::string m = a.first;
    std::string h = GenHour(hour, a.second);
    if ((h == hoursStrings[0][0] || h == hoursStrings[12][0]) && // ровно в полночь и полдень
         m == std::get<2>(minutesStrings.at(12))) return h;           // не нужно выводить минуты
    if (h == hoursStrings[0][0] && m == std::get<2>(minutesStrings.at(9)))
        return s0015;                                           //в 00:15 выводится особая строка
    switch (a.second) {
        case MyTimeOrder::current: return h + space + m;
        case MyTimeOrder::nextBegin: return h + space + m;
        case MyTimeOrder::nextEnd: return m + space + h;
        case MyTimeOrder::nextGenetive: return m + space + h;
    };
    return "TIMEERROR";
}

const char *GenTimeCStyle(char hour, char minutes, char seconds) {
    return GenTime(hour, minutes, seconds).c_str();
}

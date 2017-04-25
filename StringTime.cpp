#include "StringTime.h"

#include <iostream>
#include <vector>

namespace fuzzy_time
{
    struct Hour
    {
        std::wstring name;
        std::wstring nextName;
        std::wstring nextGenetive;
    };

    /// @brief Названия часов.
    std::vector<Hour> hours =
    {
        //часы      текущий         следующий    следующий в родительном падеже
        /*00    */ {L"полночь",     L"час",         L"первого"      },
        /*01, 13*/ {L"час",         L"два",         L"второго"      },
        /*02, 14*/ {L"два",         L"три",         L"третьего"     },
        /*03, 15*/ {L"три",         L"четыре",      L"четвертого"   },
        /*04, 16*/ {L"четыре",      L"пять",        L"пятого"       },
        /*05, 17*/ {L"пять",        L"шесть",       L"шестого"      },
        /*06, 18*/ {L"шесть",       L"семь",        L"седьмого"     },
        /*07, 19*/ {L"семь",        L"восемь",      L"восьмого"     },
        /*08, 20*/ {L"восемь",      L"девять",      L"девятого"     },
        /*09, 21*/ {L"девять",      L"десять",      L"десятого"     },
        /*10, 22*/ {L"десять",      L"одиннадцать", L"одиннадцатого"},
        /*11, 23*/ {L"одиннадцать", L"двенадцать",  L"двенадцатого" },
        /*12    */ {L"полдень",     L"час",         L"первого"      }
    };

    /// @brief Порядок вывода минут и часов.
    enum class HMOrder
    {
        current,    ///< сначала текущий час, потом минуты
        nextBegin,  ///< сначала следующий час, потом минуты
        nextEnd,    ///< сначала минуты, потом следующий час
        nextGenetive///< сначала минуты, потом следующий час в родительном падеже
    };

    /// @brief Cтруктура для записей соответсятвий минут.
    struct Minutes
    {
        uint8_t beginMinutes; ///< время начала
        uint8_t beginSeconds; ///< время начала
        std::wstring name;    ///< название
        HMOrder order;        ///< способ комбинации с часами (порядок вывода)
    };

    /// @brief Таблица соответствия минут.
    std::vector<Minutes> minutesRecords = {
       //мин сек название                порядок вывода
        {57, 30, L"почти ровно",         HMOrder::nextEnd},
        {52, 30, L"без пяти",            HMOrder::nextEnd},
        {47, 30, L"без десяти",          HMOrder::nextEnd},
        {42, 30, L"без четверти",        HMOrder::nextEnd},
        {37, 30, L"без двадцати",        HMOrder::nextEnd},
        {32, 30, L"без двадцати пяти",   HMOrder::nextEnd},
        {27, 30, L"пол",                 HMOrder::nextGenetive},
        {22, 30, L"двадцать пять минут", HMOrder::nextGenetive},
        {17, 30, L"двадцать минут",      HMOrder::nextGenetive},
        {12, 30, L"с четвертью",         HMOrder::current},
        { 7, 30, L"десять минут",        HMOrder::nextGenetive},
        { 2, 30, L"пять минут",          HMOrder::nextGenetive},
        { 0,  0, L"ровно",               HMOrder::current}
    };
    const std::wstring s0015 {L"пятнадцать минут первого"};

    const std::wstring HOUR_ERROR    {L"HOUR_ERROR"};
    const std::wstring MINUTES_ERROR {L"MINUTESERROR"};
}

/// @brief Определение, больше ли второе время чем первое.
bool isTimeGreater(uint8_t minutes1, uint8_t seconds1,
                   uint8_t minutes2, uint8_t seconds2)
{
    if (minutes1 > minutes2)
        return true;
    if (minutes1 == minutes2 && seconds1 > seconds2)
        return true;
    return false;
}

/// @brief Генерация строки минут.
std::pair<std::wstring, fuzzy_time::HMOrder>
genMinutes(uint8_t minutes, uint8_t seconds)
{
    for (auto i = fuzzy_time::minutesRecords.begin();
         i != fuzzy_time::minutesRecords.end(); ++ i)
    {
        if (isTimeGreater(minutes, seconds, i->beginMinutes, i->beginSeconds))
            return std::make_pair(i->name, i->order);
    }
    return std::make_pair(fuzzy_time::MINUTES_ERROR,
                          fuzzy_time::HMOrder::current);
}

/// @brief Генерация строки часа.
std::wstring genHour(uint8_t hour, fuzzy_time::HMOrder timeOrder)
{
    if (hour > 12)
        hour -= 12;
    if (hour < 0 || hour > 12)
        return fuzzy_time::HOUR_ERROR;

    using fuzzy_time::hours;
    using fuzzy_time::HMOrder;

    switch (timeOrder)
    {
        case HMOrder::current      : return hours[hour].name;         break;
        case HMOrder::nextBegin    : return hours[hour].nextName;     break;
        case HMOrder::nextEnd      : return hours[hour].nextName;     break;
        case HMOrder::nextGenetive : return hours[hour].nextGenetive; break;
        default                    : return fuzzy_time::HOUR_ERROR;   break;
    };
}

/// @brief Генерация времени словами.
std::wstring timeToString(uint8_t hour, uint8_t minutes, uint8_t seconds)
{
    using fuzzy_time::hours;
    using fuzzy_time::minutesRecords;
    using fuzzy_time::HMOrder;

    std::pair<std::wstring, HMOrder> a = genMinutes(minutes, seconds);
    std::wstring& minutesString = a.first;
    fuzzy_time::HMOrder& order = a.second;

    std::wstring hourString = genHour(hour, order);

    bool isMiddayOrMidnight = hour == 0 || hour == 12 || hour == 24;
    bool isOClock = minutesString == minutesRecords.back().name;
    if (isMiddayOrMidnight && isOClock)
        return hourString; // в полночь и полдень не надо выводить минуты

    bool isMidnight = hour == 0 || hour == 24;
    bool isAbout15Minutes = minutesString == minutesRecords.at(9).name;
    if (isMidnight && isAbout15Minutes)
        return fuzzy_time::s0015; //в 00:15 выводится особая строка

    switch (order)
    {
        case HMOrder::current      : return hourString + L' ' + minutesString;
        case HMOrder::nextBegin    : return hourString + L' ' + minutesString;
        case HMOrder::nextEnd      : return minutesString + L' ' + hourString;
        case HMOrder::nextGenetive : return minutesString + L' ' + hourString;
    };
    return L"TIMEERROR";
}

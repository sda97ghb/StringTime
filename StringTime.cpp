#include "StringTime.h"

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
    std::vector<Hour> hoursStrings =
    {
        //          текущий         следующий    следующий в родительном падеже
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
    enum class TimeOrder
    {
        current,    ///< сначала текущий час, потом минуты
        nextBegin,  ///< сначала следующий час, потом минуты
        nextEnd,    ///< сначала минуты, потом следующий час
        nextGenetive///< сначала минуты, потом следующий час в родительном падеже
    };

    /// @brief Cтруктура для записей соответсятвий минут.
    /// @details Время начала (мин, сек), название минут, порядок вывода.
    struct Minutes
    {
        short minutes;
        short seconds;
        std::wstring name;
        TimeOrder order;
    };

    /// @brief Таблица соответствия минут.
    std::vector<Minutes> minutesStrings = {
        // время начала(м, с) строка вывода          порядок вывода
        {57, 30, L"почти ровно",         TimeOrder::nextEnd},
        {52, 30, L"без пяти",            TimeOrder::nextEnd},
        {47, 30, L"без десяти",          TimeOrder::nextEnd},
        {42, 30, L"без четверти",        TimeOrder::nextEnd},
        {37, 30, L"без двадцати",        TimeOrder::nextEnd},
        {32, 30, L"без двадцати пяти",   TimeOrder::nextEnd},
        {27, 30, L"пол",                 TimeOrder::nextGenetive},
        {22, 30, L"двадцать пять минут", TimeOrder::nextGenetive},
        {17, 30, L"двадцать минут",      TimeOrder::nextGenetive},
        {12, 30, L"с четвертью",         TimeOrder::current},
        {7, 30, L"десять минут",        TimeOrder::nextGenetive},
        {2, 30, L"пять минут",          TimeOrder::nextGenetive},
        {0,  0, L"ровно",               TimeOrder::current}
    };
    const std::wstring s0015 = L"пятнадцать минут первого";
}

/// @brief Определение, находится ли время после указанного.
bool TimeIsGreater(char minutes1, char seconds1, char minutes2, char seconds2)
{
    if (minutes1 > minutes2)
        return true;
    if (minutes1 == minutes2 && seconds1 > seconds2)
        return true;
    return false;
}

/// @brief Генерация строки минут.
std::pair<std::wstring, fuzzy_time::TimeOrder>
GenMinutes(char minutes, char seconds)
{
    using fuzzy_time::minutesStrings;
    using fuzzy_time::TimeOrder;
    for (unsigned i = 0; i < minutesStrings.size(); ++ i)
        if (TimeIsGreater(minutes, seconds, minutesStrings.at(i).minutes, minutesStrings.at(i).seconds))
            return std::make_pair(minutesStrings.at(i).name, minutesStrings.at(i).order);
    return std::make_pair(L"MINUTESERROR", TimeOrder::current);
}

/// @brief Генерация строки часа.
std::wstring GenHour(char hour, fuzzy_time::TimeOrder timeOrder) {
    using fuzzy_time::hoursStrings;
    using fuzzy_time::TimeOrder;

    if (hour > 12) hour -= 12;
    if (hour < 0 || hour > 12) return L"HOURERROR";
    switch (timeOrder) {
        case TimeOrder::current:      return hoursStrings[hour].name; break;
        case TimeOrder::nextBegin:    return hoursStrings[hour].nextName; break;
        case TimeOrder::nextEnd:      return hoursStrings[hour].nextName; break;
        case TimeOrder::nextGenetive: return hoursStrings[hour].nextGenetive; break;
    };
    return L"HOURERROR";
}

/// @brief Генерация времени словами.
std::wstring timeToString(int hour, int minutes, int seconds){
    using fuzzy_time::hoursStrings;
    using fuzzy_time::minutesStrings;
    using fuzzy_time::TimeOrder;
    std::pair<std::wstring, TimeOrder> a = GenMinutes(minutes, seconds);
    std::wstring m = a.first;
    std::wstring h = GenHour(hour, a.second);
    if ((h == hoursStrings[0].name || h == hoursStrings[12].name) && // ровно в полночь и полдень
         m == minutesStrings.at(12).name) return h;          // не нужно выводить минуты
    if (h == hoursStrings[0].name && m == minutesStrings.at(9).name)
        return fuzzy_time::s0015;                                           //в 00:15 выводится особая строка
    switch (a.second) {
        case TimeOrder::current:      return h + L" " + m;
        case TimeOrder::nextBegin:    return h + L" " + m;
        case TimeOrder::nextEnd:      return m + L" " + h;
        case TimeOrder::nextGenetive: return m + L" " + h;
    };
    return L"TIMEERROR";
}

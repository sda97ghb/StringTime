#include "StringTime.h"

#include <tuple>
#include <vector>

namespace
{
    /// @brief Названия часов.
    std::vector<std::vector<std::wstring>> hoursStrings =
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
    using minutesRecord = std::tuple<short, short, std::wstring, TimeOrder>;

    /// @brief Таблица соответствия минут.
    std::vector<minutesRecord> minutesStrings = {
        // время начала(м, с) строка вывода          порядок вывода
        minutesRecord(57, 30, L"почти ровно",         TimeOrder::nextEnd),
        minutesRecord(52, 30, L"без пяти",            TimeOrder::nextEnd),
        minutesRecord(47, 30, L"без десяти",          TimeOrder::nextEnd),
        minutesRecord(42, 30, L"без четверти",        TimeOrder::nextEnd),
        minutesRecord(37, 30, L"без двадцати",        TimeOrder::nextEnd),
        minutesRecord(32, 30, L"без двадцати пяти",   TimeOrder::nextEnd),
        minutesRecord(27, 30, L"пол",                 TimeOrder::nextGenetive),
        minutesRecord(22, 30, L"двадцать пять минут", TimeOrder::nextGenetive),
        minutesRecord(17, 30, L"двадцать минут",      TimeOrder::nextGenetive),
        minutesRecord(12, 30, L"с четвертью",         TimeOrder::current),
        minutesRecord( 7, 30, L"десять минут",        TimeOrder::nextGenetive),
        minutesRecord( 2, 30, L"пять минут",          TimeOrder::nextGenetive),
        minutesRecord( 0,  0, L"ровно",               TimeOrder::current)
    };
    const std::wstring s0015 = L"пятнадцать минут первого";
}

/// @brief Определение, находится ли время после указанного.
bool OverTime(char minutes, char seconds, char minMinutes, char minSeconds) {
    return !(minutes < minMinutes || (minutes == minMinutes && seconds < minSeconds));
}

/// @brief Генерация строки минут.
std::pair<std::wstring, TimeOrder> GenMinutes(char minutes, char seconds) {
    for(unsigned i = 0; i < minutesStrings.size(); ++ i)
        if (OverTime(minutes, seconds, std::get<0>(minutesStrings.at(i)), std::get<1>(minutesStrings.at(i))))
            return std::pair<std::wstring, TimeOrder>(std::get<2>(minutesStrings.at(i)), std::get<3>(minutesStrings.at(i)));
    return std::pair<std::wstring, TimeOrder>(L"MINUTESERROR", TimeOrder::current);
}

/// @brief Генерация строки часа.
std::wstring GenHour(char hour, TimeOrder timeOrder) {
    if (hour > 12) hour -= 12;
    if (hour < 0 || hour > 12) return L"HOURERROR";
    switch (timeOrder) {
        case TimeOrder::current:      return hoursStrings[hour][0]; break;
        case TimeOrder::nextBegin:    return hoursStrings[hour][1]; break;
        case TimeOrder::nextEnd:      return hoursStrings[hour][1]; break;
        case TimeOrder::nextGenetive: return hoursStrings[hour][2]; break;
    };
    return L"HOURERROR";
}

/// @brief Генерация времени словами.
std::wstring timeToString(int hour, int minutes, int seconds){
    std::pair<std::wstring, TimeOrder> a = GenMinutes(minutes, seconds);
    std::wstring m = a.first;
    std::wstring h = GenHour(hour, a.second);
    if ((h == hoursStrings[0][0] || h == hoursStrings[12][0]) && // ровно в полночь и полдень
         m == std::get<2>(minutesStrings.at(12))) return h;           // не нужно выводить минуты
    if (h == hoursStrings[0][0] && m == std::get<2>(minutesStrings.at(9)))
        return s0015;                                           //в 00:15 выводится особая строка
    switch (a.second) {
        case TimeOrder::current:      return h + L" " + m;
        case TimeOrder::nextBegin:    return h + L" " + m;
        case TimeOrder::nextEnd:      return m + L" " + h;
        case TimeOrder::nextGenetive: return m + L" " + h;
    };
    return L"TIMEERROR";
}

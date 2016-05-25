#ifndef STRINGTIME_H
#define STRINGTIME_H

#include <string>
#include <tuple>
#include <vector>

namespace {
    const char space = ' ';

    std::vector<std::vector<std::string>> hoursStrings = {
    //   текущий        следующий      следующий в родительном падеже
        {"полночь",     "час",         "первого"      },
        {"час",         "два",         "второго"      },
        {"два",         "три",         "третьего"     },
        {"три",         "четыре",      "четвертого"   },
        {"четыре",      "пять",        "пятого"       },
        {"пять",        "шесть",       "шестого"      },
        {"шесть",       "семь",        "седьмого"     },
        {"семь",        "восемь",      "восьмого"     },
        {"восемь",      "девять",      "девятого"     },
        {"девять",      "десять",      "десятого"     },
        {"десять",      "одиннадцать", "одиннадцатого"},
        {"одиннадцать", "двенадцать",  "двенадцатого" },
        {"полдень",     "час",         "первого"      }
        };

    /* тип образа вывода */
    // current - текущий час перед минутами
    // nextBegin - следующий час перед минутами
    // nextEnd - следующий час после минут
    // nextGenetive - следующий час в родительном падеже и после минут
    enum class MyTimeOrder {current, nextBegin, nextEnd, nextGenetive};

    // специальная структура для записей соответсятвий минут
    typedef std::tuple<short, short, std::string, MyTimeOrder> minutesRecord;

    std::vector<minutesRecord> minutesStrings = {
        // время начала(м, с) строка вывода          порядок вывода
        minutesRecord(57, 30, "почти ровно",         MyTimeOrder::nextEnd),
        minutesRecord(52, 30, "без пяти",            MyTimeOrder::nextEnd),
        minutesRecord(47, 30, "без десяти",          MyTimeOrder::nextEnd),
        minutesRecord(42, 30, "без четверти",        MyTimeOrder::nextEnd),
        minutesRecord(37, 30, "без двадцати",        MyTimeOrder::nextEnd),
        minutesRecord(32, 30, "без двадцати пяти",   MyTimeOrder::nextEnd),
        minutesRecord(27, 30, "пол",                 MyTimeOrder::nextGenetive),
        minutesRecord(22, 30, "двадцать пять минут", MyTimeOrder::nextGenetive),
        minutesRecord(17, 30, "двадцать минут",      MyTimeOrder::nextGenetive),
        minutesRecord(12, 30, "с четвертью",         MyTimeOrder::current),
        minutesRecord( 7, 30, "десять минут",        MyTimeOrder::nextGenetive),
        minutesRecord( 2, 30, "пять минут",          MyTimeOrder::nextGenetive),
        minutesRecord( 0,  0, "ровно",               MyTimeOrder::current)
    };
    const std::string s0015 = "пятнадцать минут первого";
}

/* определение, находится ли время после указанного */
bool OverTime(char minutes, char seconds, char minMinutes, char minSeconds);
/* генерация строки минут */
std::pair<std::string, MyTimeOrder> GenMinutes(char minutes, char seconds);
/* генерация строки часа */
std::string GenHour(char hour, MyTimeOrder timeOrder);
/* отображение указанного времени в разговорной форме */
std::string GenTime(char hour, char minutes, char seconds);
const char *GenTimeCStyle(char hour, char minutes, char seconds);

#endif // STRINGTIME_H

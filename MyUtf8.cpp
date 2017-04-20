#include "MyUtf8.h"

#include <algorithm>
#include <codecvt>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <locale>

void my_utf8::setConsoleToUtf8()
{
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin),  _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
}

bool my_utf8::needChangingEndianness()
{
    uint16_t number1 = 0xABCD;

    uint8_t number2Arr[2];
    number2Arr[0] = 0xABu;
    number2Arr[1] = 0XCDu;
    uint16_t number2 = *reinterpret_cast<uint16_t*>(number2Arr);

    return number1 != number2;
}

void changeEndianness(std::wstring& string)
{
    for (wchar_t& c : string)
    {
        uint8_t* d = reinterpret_cast<uint8_t*>(&c);
        std::swap(d[0], d[1]);
    }
}

std::wstring my_utf8::convertToWString(const char* bytes,
                                       bool needChangingEndianness)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    std::wstring wstr = convert.from_bytes(bytes);

    if (needChangingEndianness)
        changeEndianness(wstr);

    return wstr;
}

std::wstring my_utf8::convertToWString(const std::string& bytes,
                                       bool needChangingEndianness)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    std::wstring wstr = convert.from_bytes(bytes);

    if (needChangingEndianness)
        changeEndianness(wstr);

    return wstr;
}

std::string my_utf8::convertToBytes(std::wstring string,
                                    bool needChangeEndianness)
{
    if (needChangeEndianness)
        changeEndianness(string);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;

    return convert.to_bytes(string);
}

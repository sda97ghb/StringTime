#ifndef MY_UTF_8_H
#define MY_UTF_8_H

#include <string>

namespace my_utf8
{
    void setConsoleToUtf8();

    bool needChangingEndianness();

    std::wstring convertToWString(const char* bytes,
                                  bool needChangingEndianness = true);
    std::wstring convertToWString(const std::string& bytes,
                                  bool needChangingEndianness = true);

    std::string convertToBytes(std::wstring string,
                               bool needChangeEndianness = false);
}

#endif // MY_UTF_8_H

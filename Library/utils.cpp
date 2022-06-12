#include "includes.h"

std::string toUtf8(const std::wstring& utf16)
{
    if (utf16.empty())
    {
        return "";
    }

    std::string utf8;
    utf8.resize(utf16.size() * 2);
    int count = WideCharToMultiByte(CP_UTF8, 0, utf16.data(), static_cast<int>(utf16.size()), utf8.data(), static_cast<int>(utf8.size()), nullptr, nullptr);
    if (count == 0)
    {
        return "<unknown>";
    }
    utf8.resize(count);

    return utf8;
}
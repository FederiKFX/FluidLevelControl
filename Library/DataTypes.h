#pragma once
#include "includes.h"
#include <algorithm>

enum Colour
{
    DEFAULT = 99,
    TANK = 10,
    WATER = 11,
    GASOLINE = 12
};

typedef struct _StrData
{
    _StrData(std::wstring str, int line, bool highlight = 0, Colour colour = DEFAULT) : str(str), line(line), highlight(highlight), colour(colour) {};
    std::wstring str;
    int line, y = 0, x = 0;
    bool highlight;
    Colour colour;

    bool IsClicked(int wClickY, int wClickX)
    {
        return (wClickY == y) && (wClickX >= x) && (wClickX < x + str.size());
    }

} StrData;

typedef struct _StateData
{
    std::wstring name;
    Colour fluidType;
    int fullness;
    std::vector<bool> sensors;

    std::wstring getNameInfo(std::wstring name)
    {
        return L"Назва: " + name;
    }
    std::wstring getFluidNameInfo()
    {
        return L"Тип рідини: " + getFluidName();
    }
    std::wstring getFullnessNameInfo()
    {
        return L"Заповненість: " + std::to_wstring(fullness) + L"%";
    }
    std::wstring getSensorsCaptionInfo()
    {
        return L"Датчики:";
    }
    std::wstring getSensorNameInfo(int i)
    {
        return L"Датчик " + std::to_wstring(i) + L": ";
    }
    int getMaxWidth()
    {
        int ret = 0;
        ret = getNameInfo(name).size() > getFluidNameInfo().size() ? getNameInfo(name).size() : getFluidNameInfo().size();
        ret = ret > getFullnessNameInfo().size() ? ret : getFullnessNameInfo().size();
        return ret;
    }
    int getMaxHeight()
    {
        int ret = 4 + sensors.size();
        return ret;
    }
    std::wstring getFluidName()
    {
        switch (fluidType)
        {
        case WATER:
            return L"Вода";
            break;
        case GASOLINE:
            return L"Бензина";
            break;
        default:
            return L"Невідома";
            break;
        }
    }
} StateData;
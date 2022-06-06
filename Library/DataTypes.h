#pragma once
#include "includes.h"
#include <algorithm>

enum Colour
{
    TANK = 10,
    WATER = 11,
    GASOLINE = 12
};

typedef struct _StrData
{
    std::wstring str;
    int line, y, x;
    bool highlight;
    Colour colour;

    bool IsClicked(int wClickY, int wClickX)
    {
        return (wClickY == y) && (wClickX >= x) && (wClickX < x + str.size());
    }

} StrData;

enum FluidType
{
    TANK = 10,
    WATER = 11,
    GASOLINE = 12
};

typedef struct _StateData
{
    std::wstring name;
    FluidType fluidType;
    int fullness;
    std::vector<bool> sensors;

    std::wstring getNameInfo(std::wstring name)
    {
        return L"�����: " + name;
    }
    std::wstring getFluidNameInfo()
    {
        return L"��� �����: " + getFluidName();
    }
    std::wstring getFullnessNameInfo()
    {
        return L"������������: " + std::to_wstring(fullness) + L"%";
    }
    std::wstring getSensorsCaptionInfo()
    {
        return L"�������:";
    }
    std::wstring getSensorNameInfo(int i)
    {
        return L"������ " + std::to_wstring(i) + L": ";
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
            return L"����";
            break;
        case GASOLINE:
            return L"�������";
            break;
        default:
            return L"�������";
            break;
        }
    }
} StateData;
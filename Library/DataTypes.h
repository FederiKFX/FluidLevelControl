#pragma once
#include "includes.h"
#include <algorithm>

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

    std::wstring getNameInfo()
    {
        return L"�����: " + name;
    }
    std::wstring getFluidNameInfo()
    {
        return L"��� �����: " + getFluidName();
    }
    std::wstring getFullnessNameInfo()
    {
        return L"�����������: " + std::to_wstring(fullness) + L"%";
    }
    int getMaxWidth()
    {
        int ret = 0;
        ret = getNameInfo().size() > getFluidNameInfo().size() ? getNameInfo().size() : getFluidNameInfo().size();
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
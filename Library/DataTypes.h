#pragma once
#include "includes.h"

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
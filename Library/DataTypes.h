#pragma once
#include "includes.h"

#define WATER_PAIR     10
#define TANK_PAIR    11

typedef struct _StateData
{
    std::wstring name;
    int fullness;
    std::vector<bool> sensors;
} StateData;
#pragma once
#include "includes.h"

#define ON_PAIR     10
#define OFF_PAIR    11

typedef struct _StateData
{
    std::wstring name;
    int fullness;
    std::vector<bool> sensors;
} StateData;
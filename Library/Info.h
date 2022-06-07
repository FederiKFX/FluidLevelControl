#pragma once
#include "DataTypes.h"

class Info
{
public:
    virtual void    UpdateStrData() = 0;
    virtual int     ClickAction(int i) = 0;
public:
    std::shared_ptr<std::vector<StrData>> m_data;
};


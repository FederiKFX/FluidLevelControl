#pragma once
#include "Info.h"

class MenuInfo : public Info
{
public:
    MenuInfo(std::shared_ptr<std::vector<std::wstring>> choices);
    void    AddChoice(std::wstring choice);

protected:
    void    UpdateStrData();
    int     ClickAction(int i);

public:
    std::shared_ptr<std::vector<std::wstring>> m_choices;
};


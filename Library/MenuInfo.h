#pragma once
#include "Info.h"

class MenuInfo : public Info
{
public:
    MenuInfo(const std::vector<std::wstring>& choices);
    void    AddChoice(std::wstring choice);
    void    UpdateStrData();
    int     ClickAction(int i);


public:
    std::vector<std::wstring> m_choices;
};


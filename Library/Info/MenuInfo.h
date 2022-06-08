#pragma once
#include "Info.h"

class MenuInfo : public Info
{
public:
    MenuInfo(std::vector<std::shared_ptr<std::wstring>> choices);
    void    AddChoice(std::shared_ptr<std::wstring> choice);

protected:
    void    UpdateStrData();
    int     ClickAction(int i);

protected:
    std::vector<std::shared_ptr<std::wstring>> m_choices;
};


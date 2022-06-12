#pragma once
#include "Info.h"

class MenuInfo : public Info
{
public:
    MenuInfo(std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices);
    void    AddChoice(std::pair<uint64_t, std::wstring> choice);
    void    UpdateStrData();
protected:   
    int     ClickAction(int i);

protected:
    std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> m_choices;
    std::vector<bool> m_visible;
    int count = 0;
};


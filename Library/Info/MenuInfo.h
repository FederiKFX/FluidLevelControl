#pragma once
#include "Info.h"

class MenuInfo : public Info
{
public:
    MenuInfo(std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices);
    void    AddChoice(std::pair<uint64_t, std::wstring> choice);
    int     ClickAction(int mouse_y, int mouse_x);
    void    UpdateStrData();

protected:
    std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> m_choices;
    std::vector<bool> m_visible;
    int count = 0; 
};


#include "MenuInfo.h"

MenuInfo::MenuInfo(std::vector<std::shared_ptr<std::wstring>> choices) : m_choices(choices)
{
    m_data = std::make_shared<std::vector<StrData>>();
    init_pair(Colour::DEFAULT, COLOR_WHITE, COLOR_BLACK);
    UpdateStrData();
}

void MenuInfo::AddChoice(std::shared_ptr<std::wstring> choice)
{
    m_choices.push_back(choice);
    UpdateStrData();
}

void MenuInfo::UpdateStrData()
{
    m_data->clear();
    int line = 0;
    for (size_t i = 0; i < m_choices.size(); i++)
    {
        m_data->push_back(StrData(*m_choices[i], line++));
    }   
}

int MenuInfo::ClickAction(int i)
{
    for (auto& el : *m_data)
    {
        el.highlight = false;
    }
    if (i != -1)
        (*m_data)[i].highlight = true;
    return i;
}

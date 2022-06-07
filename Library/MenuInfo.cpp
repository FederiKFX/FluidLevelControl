#include "MenuInfo.h"

MenuInfo::MenuInfo(const std::vector<std::wstring>& choices) : m_choices(choices)
{
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
}

void MenuInfo::AddChoice(std::wstring choice)
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
        m_data->push_back(StrData(m_choices[i], line++));
    }   
}

int MenuInfo::ClickAction(int i)
{
    return i;
}

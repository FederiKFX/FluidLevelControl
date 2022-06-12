#include "MenuInfo.h"

MenuInfo::MenuInfo(std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices) : m_choices(choices)
{
    m_data = std::make_shared<std::vector<StrData>>();
    init_pair(Colour::DEFAULT, COLOR_WHITE, COLOR_BLACK);
    UpdateStrData();
}

void MenuInfo::AddChoice(std::pair<uint64_t, std::wstring> choice)
{
    m_choices->push_back(choice);
    UpdateStrData();
}

void MenuInfo::UpdateStrData()
{
    m_data->clear();
    int line = 0;
    for (size_t i = 0; i < m_choices->size(); i++)
    {
        if (i >= m_visible.size())
        {
            m_visible.push_back(false);
        }
        m_data->push_back(StrData((*m_choices)[i].second, line++, m_visible[i]));
    }   
}

int MenuInfo::ClickAction(int i)
{   
    if (i != -1)
        (*m_data)[i].highlight = !(*m_data)[i].highlight;

    m_visible.clear();
    for (auto& el : *m_data)
    {
        m_visible.push_back(el.highlight);
    }
    return i;
}

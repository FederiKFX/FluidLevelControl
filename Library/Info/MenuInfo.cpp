#include "MenuInfo.h"

MenuInfo::MenuInfo(int y, int x, std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices) : m_choices(choices)
{
    init_pair(FluidType::DEFAULT, COLOR_WHITE, COLOR_BLACK);
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    m_window = std::make_shared<Window>(m_data, y, x, true);
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
        std::wstring name = (*m_choices)[i].second;
        if ((*m_choices)[i].first)
            name += L": " + std::to_wstring((*m_choices)[i].first);

        m_data->push_back(StrData(name, line++, m_visible[i]));
    }   
}

void MenuInfo::SetHighlight(int i, bool en)
{
    (*m_data)[i].highlight = en;
    m_visible[i] = en;
}

int MenuInfo::ClickAction(int mouse_y, int mouse_x)
{   
    int i = m_window->ClickedAt(mouse_y, mouse_x);
    if (i != -1)
        (*m_data)[i].highlight = !(*m_data)[i].highlight;

    m_visible.clear();
    for (auto& el : *m_data)
    {
        m_visible.push_back(el.highlight);
    }
    return i;
}

#include "MenuInfo.h"

MenuInfo::MenuInfo(int y, int x, std::shared_ptr<std::vector<std::tuple<uint64_t, std::wstring, FluidType>>> choices) : m_choices(choices)
{
    init_pair(FluidType::DEFAULT, COLOR_WHITE, COLOR_BLACK);
    init_pair(FluidType::ERRORCOL, COLOR_RED, COLOR_BLACK);
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    m_window = std::make_shared<Window>(m_data, y, x, true);
}

void MenuInfo::AddChoice(std::tuple<uint64_t, std::wstring, FluidType> choice)
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
        std::wstring name = std::get<1>((*m_choices)[i]);
        if (std::get<0>((*m_choices)[i]))
            name += L": " + std::to_wstring(std::get<0>((*m_choices)[i]));

        m_data->push_back(StrData(name, line++, m_visible[i], std::get<2>((*m_choices)[i])));
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

int MenuInfo::ClickedAt(int mouse_y, int mouse_x)
{
    return m_window->ClickedAt(mouse_y, mouse_x);
}

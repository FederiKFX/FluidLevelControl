#include "WindowsGrid.h"

WindowsGrid::WindowsGrid(int y, int x) : m_y(y), m_x(x)
{
    getmaxyx(stdscr, m_heightSTD, m_widthSTD);
}

void WindowsGrid::Add(Window* win)
{
    m_Windows.emplace_back(win);
    m_width < m_Windows.back()->GetWidth() ? m_width = m_Windows.back()->GetWidth() : NULL;
    m_height < m_Windows.back()->GetHeight() ? m_height = m_Windows.back()->GetHeight() : NULL;
    CalcPos();
}

void WindowsGrid::Update()
{
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->Update();
    }
}

void WindowsGrid::PosUpdate()
{
    CalcPos();
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->Window::PosUpdate();
    }
}

void WindowsGrid::SizeUpdate()
{
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->SizeUpdate();
    }
}

void WindowsGrid::ClickAction(int mouse_y, int mouse_x)
{
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->ClickAction(mouse_y, mouse_x);
    }
}

void WindowsGrid::CalcPos()
{
    getmaxyx(stdscr, m_heightSTD, m_widthSTD);
    int col = (float)(m_widthSTD - m_x) / m_width;
    int row = (float)(m_heightSTD - m_y) / m_height;
    m_yLast = m_y;
    m_xLast = m_x;
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        if ((i % col == 0) && (i >= col))
        {
            m_yLast += m_height;
            m_xLast = m_x;
        }
        m_Windows[i]->SetPos(m_yLast, m_xLast);
        m_xLast += m_width;
    }
}

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
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->PosUpdate();
    }
}

void WindowsGrid::SizeUpdate()
{
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->SizeUpdate();
    }
}

void WindowsGrid::CalcPosSize()
{
    int col = (float)m_widthSTD / m_width;
    int row = (float)m_heightSTD / m_height;
    m_yLast = m_y;
    m_xLast = m_x;
    for (size_t i = 0; i < m_Windows.size(); ++i)
    {
        m_Windows[i]->SetPos(m_yLast, m_xLast);
        m_xLast += m_width;
        if ((i > col) && (i % col == 0))
        {
            m_yLast += m_height;
            m_xLast = m_x;
        }
    }
}

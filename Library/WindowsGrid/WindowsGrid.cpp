#include "WindowsGrid.h"

WindowsGrid::WindowsGrid(int y, int x) : m_y(y), m_x(x)
{
    getmaxyx(stdscr, m_heightSTD, m_widthSTD);
}

void WindowsGrid::Add(std::shared_ptr<Info> win)
{
    m_InfoVec.push_back(win);
    m_width < m_InfoVec.back()->m_window->GetWidth() ? m_width = m_InfoVec.back()->m_window->GetWidth() : NULL;
    m_height < m_InfoVec.back()->m_window->GetHeight() ? m_height = m_InfoVec.back()->m_window->GetHeight() : NULL;
    CalcPos();
}

void WindowsGrid::Del(int i)
{
    m_InfoVec.erase(m_InfoVec.begin() + i);
}

void WindowsGrid::Update()
{
    int w, h;
    for (size_t i = 0; i < m_InfoVec.size(); ++i)
    {
        m_InfoVec[i]->UpdateStrData();
        m_InfoVec[i]->m_window->Update();
        h = m_InfoVec[i]->m_window->GetHeight();
        w = m_InfoVec[i]->m_window->GetWidth();
        m_height < h ? m_height = h : NULL;
        m_width < w ? m_width = w : NULL;
    }
}

void WindowsGrid::PosUpdate()
{
    CalcPos();
    for (size_t i = 0; i < m_InfoVec.size(); ++i)
    {
        m_InfoVec[i]->m_window->Window::PosUpdate();
    }
}

void WindowsGrid::SizeUpdate()
{
    for (size_t i = 0; i < m_InfoVec.size(); ++i)
    {
        m_InfoVec[i]->m_window->SizeUpdate();
    }
}

bool WindowsGrid::ClickAction(int mouse_y, int mouse_x)
{
    bool res = false;
    for (size_t i = 0; i < m_InfoVec.size(); ++i)
    {
        if (m_InfoVec[i]->ClickAction(mouse_y, mouse_x) != -1)
            res = true;
    }
    return res;
}

void WindowsGrid::CalcPos()
{
    getmaxyx(stdscr, m_heightSTD, m_widthSTD);
    int col = (float)(m_widthSTD - m_x) / m_width;
    int row = (float)(m_heightSTD - m_y) / m_height;
    m_yLast = m_y;
    m_xLast = m_x;
    for (size_t i = 0; i < m_InfoVec.size(); ++i)
    {
        if ((i % col == 0) && (i >= col))
        {
            m_yLast += m_height;
            m_xLast = m_x;
        }
        m_InfoVec[i]->m_window->SetPos(m_yLast, m_xLast);
        m_xLast += m_width;
    }
}

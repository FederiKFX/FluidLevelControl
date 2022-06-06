#include "Window.h"

Window::Window(int y, int x, bool boxEn, int height, int width) : m_height(height), m_width(width), m_y(y), m_x(x), m_boxEn(boxEn)
{
    m_window = newwin(m_height, m_width * 2, m_y, m_x);
}

void Window::PosUpdate()
{
    mvwin(m_window, m_y, m_x);
}

void Window::SizeUpdate()
{
    wresize(m_window, m_height, m_width);
}

void Window::Update()
{
    if (m_boxEn)
        box(m_window, 0, 0);
    wrefresh(m_window);
}

int Window::GetHeight()
{
    return m_height;
}

int Window::GetWidth()
{
    return m_width;
}

void Window::SetPos(int y, int x)
{
    m_y = y;
    m_x = x;
}

void Window::SetSize(int height, int width)
{
    m_height = height;
    m_width = width;
}

bool Window::IsClicked(int mouse_y, int mouse_x)
{
    return (mouse_y > m_y) && (mouse_y < m_y + m_height) && (mouse_x > m_x) && (mouse_x < m_x + m_width);
}

std::wstring Window::GetWstr(int y, int x, int capacity)
{
    return GetWSTR(y, x, capacity);
}

void Window::ColorOn(int color)
{
    wattron(m_window, COLOR_PAIR(color));
}

void Window::ColorOff(int color)
{
    wattroff(m_window, COLOR_PAIR(color));
}

std::wstring Window::GetWSTR(int y, int x, int capacity)
{
    if (!y || !x)
    {
        y = 1;
        x = 2;
    }
    if (!capacity)
    {
        capacity = m_width - x - 2;
    }
    wint_t* buf = new wint_t[50]{ 0 };
    echo();
    int size = mvwget_wstr(m_window, y, x, buf);
    noecho();
    std::vector<wint_t> temp(buf, buf + capacity);
    return std::wstring(temp.begin(), temp.end());
}

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

void Window::ColorOn(int color)
{
    wattron(m_window, COLOR_PAIR(color));
}

void Window::ColorOff(int color)
{
    wattroff(m_window, COLOR_PAIR(color));
}

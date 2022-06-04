#include "Window.h"

Window::Window(int y, int x, int height, int width, bool boxEn) : m_height(height), m_width(width), m_y(y), m_x(x), m_boxEn(boxEn)
{
    m_window = newwin(m_height, m_width * 2, m_y, m_x);
}

void Window::PosUpdate()
{
    mvwin(m_window, m_y, m_x);
}

void Window::Update()
{
    wrefresh(m_window);
}

void Window::ColorOn(int color)
{
    wattron(m_window, COLOR_PAIR(color));
}

void Window::ColorOff(int color)
{
    wattroff(m_window, COLOR_PAIR(color));
}

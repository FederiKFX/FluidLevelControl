#include "Window.h"

Window::Window(int height, int width, int y, int x, bool boxEn) : m_height(height), m_width(width), m_y(y), m_x(x)
{
    m_window = newwin(m_height, m_width * 2, m_y, m_x);
    if (boxEn)
        box(m_window, 0, 0);
}

void Window::PosUpdate()
{
    mvwin(m_window, m_y, m_x);
}

void Window::Update()
{
    wrefresh(m_window);
}

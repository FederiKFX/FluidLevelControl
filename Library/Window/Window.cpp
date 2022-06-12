#include "Window.h"

Window::Window(std::shared_ptr<std::vector<StrData>> data, int y, int x, bool boxEn, int height, int width) : m_data(data), m_height(height), m_width(width), m_y(y), m_x(x), m_boxEn(boxEn)
{
    //m_data = m_winInfo->m_data;
    m_window = newwin(m_height, m_width, m_y, m_x);
    TextUpdate();
    SizeUpdate();
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
        wborder(m_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    delwin(m_window);
    //if (m_boxEn)
       // wborder(m_window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    m_window = newwin(m_height, m_width, m_y, m_x);
    TextUpdate();
    SizeUpdate();

    if (m_boxEn)
        box(m_window, 0, 0);

    wrefresh(m_window);
}

/*int Window::ClickAction(int mouse_y, int mouse_x)
{
    return m_winInfo->ClickAction(ClickedAt(mouse_y, mouse_x));
}*/

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

int Window::ClickedAt(int mouse_y, int mouse_x)
{
    int ret = -1;
    if (IsClicked(mouse_y, mouse_x))
    {
        for (size_t i = 0; i < (*m_data).size(); ++i)
        {
            if ((*m_data)[i].IsClicked(mouse_y - m_y, mouse_x - m_x))
            {
                ret = i;
                break;
            }
        }
    }
    return ret;
}

bool Window::IsClicked(int mouse_y, int mouse_x)
{
    return (mouse_y >= m_y) && (mouse_y < m_y + m_height) && (mouse_x >= m_x) && (mouse_x < m_x + m_width);
}

std::wstring Window::GetWstr(int i)
{
    return GetWSTR((*m_data)[i].y, (*m_data)[i].x, 0);
}

void Window::PaintOn(Colour color, bool highlight)
{
    wattron(m_window, COLOR_PAIR(color));
    if(highlight)
        wattron(m_window, A_REVERSE);
}

void Window::PaintOff(Colour color, bool highlight)
{
    wattroff(m_window, COLOR_PAIR(color));
    if (highlight)
        wattroff(m_window, A_REVERSE);
}

void Window::TextUpdate()
{
    if (!m_data->empty())
    {
        int y = 0, x = 0;
        int maxWidth = (*m_data)[0].str.size();
        m_height = 2 * m_textY;
        m_width = 2 * m_textX;
        for (auto& el : *m_data)
        {
            PaintOn(el.colour, el.highlight);
            if (el.line != y)
            {
                y++;
                maxWidth < x ? maxWidth = x : NULL;
                x = 0;
            }
            mvwaddwstr(m_window, y + m_textY, x + m_textX, el.str.data());
            el.y = y + m_textY;
            el.x = x + m_textX;
            x += el.str.size();
            PaintOff(el.colour, el.highlight);
        }
        maxWidth < x ? maxWidth = x : NULL;
        m_height += ++y;
        m_width += maxWidth;
    }
}

std::wstring Window::GetWSTR(int y, int x, int capacity)
{
    if (!capacity)
    {
        capacity = m_width - x - m_textX;
    }
    wint_t* buf = new wint_t[capacity * 2]{ 0 };
    echo();
    mvwget_wstr(m_window, y, x, buf);
    noecho();
    std::vector<wint_t> temp(buf, buf + capacity);
    return std::wstring(temp.begin(), temp.end());
}

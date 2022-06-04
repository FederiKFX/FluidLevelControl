#include "MenuWindow.h"

MenuWindow::MenuWindow(int height, int width, int y, int x, bool boxEn) : m_height(height), m_width(width), m_y(y), m_x(x)
{
    m_window = newwin(m_height, m_width * 2, m_y, m_x);
    if(boxEn)
        box(m_window, 0, 0);
}

void MenuWindow::setChoices(const std::vector<std::wstring>& choices)
{
    m_choices = choices;
}

void MenuWindow::addChoice(std::wstring choice)
{
    m_choices.push_back(choice);
}

int MenuWindow::ReportChoice(int mouse_y, int mouse_x)
{
    int i, j, choice;
    int ret = -1;
    i = m_x + m_textX;
    j = m_y + m_textY + 1;

    for (choice = 0; choice < m_choices.size(); ++choice)
    {
        if (mouse_y == j + choice && mouse_x >= i && mouse_x <= i + m_choices[choice].size())
        {
            ret = choice;
            break;
        }
    }
    m_highlight = ret;
    return ret;
}

void MenuWindow::PosUpdate()
{
    mvwin(m_window, m_y, m_x);
}

void MenuWindow::TextUpdate()
{
    int x = m_textX;
    int y = m_textY;
    for (size_t i = 0; i < m_choices.size(); ++i)
    {
        if (m_highlight == i)
        {
            wattron(m_window, A_REVERSE);
            //mvwprintw(m_window, y, x, "%s", m_choices[i].data());
            mvwaddwstr(m_window, y, x, m_choices[i].data());
            wattroff(m_window, A_REVERSE);
        }
        else
        {
            //mvwprintw(m_window, y, x, "%s", m_choices[i].data());
            mvwaddwstr(m_window, y, x, m_choices[i].data());
        }
        ++y;
    }
}

void MenuWindow::Update()
{
    TextUpdate();
    wrefresh(m_window);
}

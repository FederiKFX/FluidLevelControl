#include "MenuWindow.h"

MenuWindow::MenuWindow(int y, int x, bool boxEn, int height, int width) : Window(y, x, boxEn, height, width)
{
    if (m_boxEn)
        box(m_window, 0, 0);
}

void MenuWindow::SetCaption(std::wstring caption)
{
    m_caption = caption;
}

void MenuWindow::SetChoices(const std::vector<std::wstring>& choices)
{
    m_choices = choices;
}

void MenuWindow::AddChoice(std::wstring choice)
{
    m_choices.push_back(choice);
}

int MenuWindow::ReportChoice(int mouse_y, int mouse_x)
{
    int i, j, choice;
    int ret = -1;
    if (IsClicked(mouse_y, mouse_x))
    {
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
    }
    return ret;
}

void MenuWindow::TextUpdate()
{
    int x = m_textX;
    int y = m_textY;
    if (!m_caption.empty())
    {
        mvwaddwstr(m_window, m_textY - 1, x, m_caption.data());
    }
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
    Window::Update();
}

#pragma once
#include "includes.h"
#include "Window\Window.h"

class MenuWindow : public Window
{
public:
            MenuWindow      (int height, int width, int y, int x, bool boxEn = false);
    void    SetCaption      (std::wstring caption);
    void    SetChoices      (const std::vector<std::wstring>& choices);
    void    AddChoice       (std::wstring choice);
    int     ReportChoice    (int mouse_x, int mouse_y);
    void    Update          ();

private:
    void    TextUpdate      ();
private:
    int                         m_highlight = -1;
    std::vector<std::wstring>   m_choices;
    std::wstring                m_caption;

    int                         m_textX = 2;
    int                         m_textY = 1;
};


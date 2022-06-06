#pragma once
#include "includes.h"
#include "DataTypes.h"
#include "Window/Window.h"

class WindowsGrid
{
public:
            WindowsGrid (int y, int x);
    void    Add         (Window* win);
    void    Update      ();
    void    PosUpdate   ();
    void    SizeUpdate  ();
    void    ClickAction (int mouse_y, int mouse_x);
    void    SetRename   (bool flag);

private:
    void    CalcPos ();
private:
    std::vector<std::unique_ptr<Window>> m_Windows;
    int m_y, m_x;
    int m_yLast, m_xLast;
    int m_height = 0, m_width = 0;
    int m_heightSTD = 0, m_widthSTD = 0;
};


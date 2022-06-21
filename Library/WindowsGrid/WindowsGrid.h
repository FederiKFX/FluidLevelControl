#pragma once
#include "includes.h"
#include "Info/Info.h"

class WindowsGrid
{
public:
            WindowsGrid (int y, int x);
    void    Add         (std::shared_ptr<Info> win);
    void    Del         (int i);
    void    PosUpdate   ();
    void    SizeUpdate  ();
    void    Update      ();
    int     ClickAction (int mouse_y, int mouse_x);

private:
    void    CalcPos     ();
private:
    std::vector<std::shared_ptr<Info>> m_InfoVec;
    int m_y, m_x;
    int m_yLast = 0, m_xLast = 0;
    int m_height = 0, m_width = 0;
    int m_heightSTD = 0, m_widthSTD = 0;
};


#pragma once
#include "includes.h"

class Window
{
public:
                    Window      (int height, int width, int y, int x, bool boxEn = false);
    virtual void    PosUpdate   ();
    virtual void    Update      ();

protected:
    WINDOW*     m_window;
    int         m_x;
    int         m_y;
    int         m_width;
    int         m_height;
};


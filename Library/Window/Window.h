#pragma once
#include "includes.h"

class Window
{
public:
                    Window      (int height, int width, int y, int x, bool boxEn = false);
    virtual void    PosUpdate   ();
    virtual void    Update      ();

protected:
    void            ColorOn     (int color);
    void            ColorOff    (int color);

protected:
    WINDOW*     m_window;
    int         m_x;
    int         m_y;
    int         m_width;
    int         m_height;
};


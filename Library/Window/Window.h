#pragma once
#include "includes.h"

class Window
{
public:
                    Window      (int y, int x, bool boxEn = false, int height = 5, int width = 5);
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
    bool        m_boxEn;
};


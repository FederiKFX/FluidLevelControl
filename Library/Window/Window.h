#pragma once
#include "includes.h"

class Window
{
public:
                    Window      (int y, int x, bool boxEn = false, int height = 0, int width = 0);
    virtual void    PosUpdate   ();
<<<<<<< HEAD
    virtual void    SizeUpdate();
=======
    virtual void    SizeUpdate  ();
>>>>>>> e00d40e40b41758669d17825acdbbef7530139a5
    virtual void    Update      ();
    int             GetHeight   ();
    int             GetWidth    ();
    void            SetPos      (int y, int x);
    void            SetSize     (int height, int width);

    int             GetHeight();
    int             GetWidth();
    void            SetPos(int y, int x);
    void            SetSize(int height, int width);

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


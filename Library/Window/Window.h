#pragma once
#include "includes.h"

class Window
{
public:
                    Window      (int y, int x, bool boxEn = false, int height = 0, int width = 0);
    virtual void    PosUpdate   ();
    virtual void    SizeUpdate  ();
    virtual void    Update      ();
    virtual int     ClickAction(int mouse_y, int mouse_x) = 0;
    int             GetHeight   ();
    int             GetWidth    ();
    void            SetPos      (int y, int x);
    void            SetSize     (int height, int width);
    bool            IsClicked   (int mouse_y, int mouse_x);
    virtual std::wstring    GetWstr     (int y, int x, int capacity);
public:
    bool            m_renameAllowed = false;
protected:
    void            ColorOn     (int color);
    void            ColorOff    (int color);
private:
    std::wstring    GetWSTR(int y = 0, int x = 0, int capacity = 0);

protected:
    WINDOW*     m_window;
    int         m_x;
    int         m_y;
    int         m_width;
    int         m_height;
    bool        m_boxEn;
};


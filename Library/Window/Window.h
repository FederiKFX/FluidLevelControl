#pragma once
#include "includes.hpp"
#include "info/Info.h"

class Window
{
public:
            Window      (std::shared_ptr<Info> info, int y, int x, bool boxEn = false, int height = 0, int width = 0);
    void    SetPos      (int y, int x);
    void    SetSize     (int height, int width);
    void    PosUpdate   ();
    void    SizeUpdate  ();
    void    Update      ();
    int     ClickAction (int mouse_y, int mouse_x);
    int     GetHeight   ();
    int     GetWidth    ();

    std::wstring    GetWstr     (int i);

protected:
    bool            IsClicked   (int mouse_y, int mouse_x);
    int             ClickedAt   (int mouse_y, int mouse_x);
    void            PaintOn     (Colour color, bool highlight);
    void            PaintOff    (Colour color, bool highlight);
    std::wstring    GetWSTR     (int y = 0, int x = 0, int capacity = 0);
    void            TextUpdate  ();

protected:
    std::shared_ptr<Info>                   m_winInfo;
    std::shared_ptr<std::vector<StrData>>   m_data;
    
    WINDOW* m_window;
    int     m_y;
    int     m_x;   
    bool    m_boxEn;
    int     m_height;
    int     m_width;
    int     m_textX = 2;
    int     m_textY = 1;   
};


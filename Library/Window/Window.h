#pragma once
#include "includes.h"
#include "DataTypes.h"

class Window
{
public:
                    Window      (std::shared_ptr<std::vector<StrData>> data, int y, int x, bool boxEn = false, int height = 0, int width = 0);
    virtual void    PosUpdate   ();
    virtual void    SizeUpdate  ();
    virtual void    Update      ();
    //virtual int     ClickAction (int mouse_y, int mouse_x) = 0;
    int             GetHeight   ();
    int             GetWidth    ();
    void            SetPos      (int y, int x);
    void            SetSize     (int height, int width);
    int             ClickedAt   (int mouse_y, int mouse_x);
    
    virtual std::wstring    GetWstr     (int i);
public:
    bool            m_renameAllowed = false;
protected:
    bool            IsClicked   (int mouse_y, int mouse_x);
    void            PaintOn     (Colour color, bool highlight);
    void            PaintOff    (Colour color, bool highlight);
    std::wstring    GetWSTR     (int y = 0, int x = 0, int capacity = 0);
    void            TextUpdate  ();
protected:
    std::shared_ptr<std::vector<StrData>>    m_data;
    int                     m_textX = 2;
    int                     m_textY = 1;

    WINDOW*     m_window;
    int         m_x;
    int         m_y;
    int         m_width;
    int         m_height;
    bool        m_boxEn;
};


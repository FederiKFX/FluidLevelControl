#pragma once
#include "includes.h"
//#include "info/Info.h"

enum FluidType
{
    DEFAULT = 99,
    TANK = 10,
    WATER = 11,
    GASOLINE = 12,
    OIL = 13,
    MILK = 14,
    CHEMICALS = 15,
    OTHER = 98,
    ERRORCOL = 100
};

struct StrData
{
    StrData(std::wstring str, int line, bool highlight = 0, FluidType colour = DEFAULT) : str(str), line(line), highlight(highlight), colour(colour) {};
    std::wstring str;
    int line, y = 0, x = 0;
    bool highlight;
    FluidType colour;

    bool IsClicked(int wClickY, int wClickX)
    {
        return (wClickY == y) && (wClickX >= x) && (wClickX < x + str.size());
    }
};

class Window
{
public:
            Window      (std::shared_ptr<std::vector<StrData>> data, int y, int x, bool boxEn = false, int height = 0, int width = 0);
    void    SetPos      (int y, int x);
    void    SetSize     (int height, int width);
    void    PosUpdate   ();
    void    SizeUpdate  ();
    void    Update      ();
    int     ClickedAt(int mouse_y, int mouse_x);
    int     GetHeight   ();
    int     GetWidth    ();

    std::wstring    GetWstr     (int i);

protected:
    bool            IsClicked   (int mouse_y, int mouse_x);   
    void            PaintOn     (FluidType color, bool highlight);
    void            PaintOff    (FluidType color, bool highlight);
    std::wstring    GetWSTR     (int y = 0, int x = 0, int capacity = 0);
    void            TextUpdate  ();

protected:
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


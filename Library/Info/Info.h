#pragma once
#include "includes.hpp"

enum Colour
{
    DEFAULT = 99,
    TANK = 10,
    WATER = 11,
    GASOLINE = 12
};

typedef struct _StrData
{
    _StrData(std::wstring str, int line, bool highlight = 0, Colour colour = DEFAULT) : str(str), line(line), highlight(highlight), colour(colour) {};
    std::wstring str;
    int line, y = 0, x = 0;
    bool highlight;
    Colour colour;

    bool IsClicked(int wClickY, int wClickX)
    {
        return (wClickY == y) && (wClickX >= x) && (wClickX < x + str.size());
    }
} StrData;

class Window;

class Info
{
    friend Window;
protected:
    virtual void    UpdateStrData() = 0;
    virtual int     ClickAction(int i) = 0;
protected:
    std::shared_ptr<std::vector<StrData>> m_data;
};


#pragma once
#include "includes.h"
#include "Window/Window.h"

class Info
{
public:
    std::shared_ptr<Window> m_window;
    virtual int     ClickAction(int mouse_y, int mouse_x) = 0;
    virtual void    UpdateStrData() = 0;

protected:
    std::shared_ptr<std::vector<StrData>> m_data;
};


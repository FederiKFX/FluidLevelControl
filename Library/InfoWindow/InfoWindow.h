#pragma once
#include "includes.h"
#include "DataTypes.h"
#include "Window\Window.h"

class InfoWindow : public Window
{
public:
            InfoWindow  (int y, int x, bool boxEn = false, int height = 0, int width = 0);
            InfoWindow  (std::shared_ptr<StateData> data, bool boxEn);
    void    SetData     (std::shared_ptr<StateData> data);
    void    Update      ();
    int     ClickAction (int mouse_y, int mouse_x);

private:
    void    ColourInit  ();
    void    TextUpdate  ();
    void    CalcSize    ();

private:  
    int                         m_textX = 2;
    int                         m_textY = 1;
    std::shared_ptr<StateData>  m_Data;
    std::vector<std::pair<int, int>>m_sensorsPos;
};


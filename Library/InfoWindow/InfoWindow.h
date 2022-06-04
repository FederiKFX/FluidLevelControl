#pragma once
#include "includes.h"
#include "DataTypes.h"
#include "Window\Window.h"

class InfoWindow : public Window
{
public:
            InfoWindow  (int height, int width, int y, int x, bool boxEn = false);
    void    SetData     (StateData data);
    void    Update      ();

private:
    void    TextUpdate  ();

private:
    int         m_textX = 2;
    int         m_textY = 1;
    StateData   m_Data;
};


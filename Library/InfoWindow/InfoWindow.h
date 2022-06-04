#pragma once
#include "includes.h"
#include "DataTypes.h"
#include "Window\Window.h"

class InfoWindow : public Window
{
public:
            InfoWindow  (int y, int x, int height = 5, int width = 5, bool boxEn = false);
    void    SetData     (std::shared_ptr<StateData> data);
    void    Update      ();

private:
    void    TextUpdate  ();

private:
    int                         m_textX = 2;
    int                         m_textY = 1;
    std::shared_ptr<StateData>  m_Data;
};


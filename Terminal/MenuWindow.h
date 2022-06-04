#pragma once
#include "global.h"

class MenuWindow
{
public:
    MenuWindow(int height, int width, int y, int x, bool box = false);
    void setChoices(const std::vector<std::wstring>& choices);
    void addChoice(std::wstring choice);
    int ReportChoice(int mouse_x, int mouse_y);
    void PosUpdate();
    void Update();

private:
    void TextUpdate();
private:
    int m_highlight = -1;
    std::vector<std::wstring> m_choices;

    WINDOW* m_window;
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    int m_textX = 2;
    int m_textY = 1;
};


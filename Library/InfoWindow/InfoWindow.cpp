#include "InfoWindow.h"

InfoWindow::InfoWindow(int height, int width, int y, int x, bool boxEn) : Window(height, width, y, x, boxEn)
{
    init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(TANK_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void InfoWindow::SetData(StateData data)
{
    m_Data = data;
}

void InfoWindow::Update()
{
    TextUpdate();
    Window::Update();
}

void InfoWindow::TextUpdate()
{
    int x = m_textX;
    int y = m_textY;
    mvwaddwstr(m_window, y++, x, (L"Назва: " + m_Data.name).data());
    mvwaddwstr(m_window, y++, x, (L"Заповненість: " + std::to_wstring(m_Data.fullness) + L"%").data());
    mvwaddwstr(m_window, y++, x, L"Датчики: ");
    
    for (size_t i = m_Data.sensors.size() - 1; i != -1; i--)
    {
        std::wstring message = L"Датчик " + std::to_wstring(i) + L": ";
        mvwaddwstr(m_window, y, x, message.data());
        m_Data.sensors[i] ? ColorOn(WATER_PAIR) : ColorOn(TANK_PAIR);
        mvwaddwstr(m_window, y++, x + message.size(), L"000000000");
        m_Data.sensors[i] ? ColorOff(WATER_PAIR) : ColorOff(TANK_PAIR);
    }
}

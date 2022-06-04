#include "InfoWindow.h"

InfoWindow::InfoWindow(int height, int width, int y, int x, bool boxEn) : Window(height, width, y, x, boxEn)
{
    init_pair(FluidType::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(FluidType::GASOLINE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FluidType::TANK, COLOR_WHITE, COLOR_WHITE);
}

void InfoWindow::SetData(std::shared_ptr<StateData> data)
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
    mvwaddwstr(m_window, y++, x, (L"Назва: " + m_Data->name).data());
    mvwaddwstr(m_window, y++, x, (L"Тип рідини: " + m_Data->getFluidName()).data());
    mvwaddwstr(m_window, y++, x, (L"Заповненість: " + std::to_wstring(m_Data->fullness) + L"%").data());
    mvwaddwstr(m_window, y++, x, L"Датчики: ");
    
    for (size_t i = m_Data->sensors.size() - 1; i != -1; i--)
    {
        std::wstring message = L"Датчик " + std::to_wstring(i) + L": ";
        mvwaddwstr(m_window, y, x, message.data());
        m_Data->sensors[i] ? ColorOn(m_Data->fluidType) : ColorOn(FluidType::TANK);
        mvwaddwstr(m_window, y++, x + message.size(), L"000000000");
        m_Data->sensors[i] ? ColorOff(m_Data->fluidType) : ColorOff(FluidType::TANK);
    }
}

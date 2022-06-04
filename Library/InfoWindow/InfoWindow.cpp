#include "InfoWindow.h"

InfoWindow::InfoWindow(int y, int x, int height, int width, bool boxEn) : Window(y, x, height, width, boxEn)
{
    init_pair(FluidType::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(FluidType::GASOLINE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FluidType::TANK, COLOR_WHITE, COLOR_WHITE);
}

void InfoWindow::SetData(std::shared_ptr<StateData> data)
{
    m_Data = data;
    CalcSize();
    wresize(m_window, m_height, m_width);
    if (m_boxEn)
        box(m_window, 0, 0);
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
    mvwaddwstr(m_window, y++, x, m_Data->getNameInfo().data());
    mvwaddwstr(m_window, y++, x, m_Data->getFluidNameInfo().data());
    mvwaddwstr(m_window, y++, x, m_Data->getFullnessNameInfo().data());
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

void InfoWindow::CalcSize()
{
    m_width = m_Data->getMaxWidth() + 2 * m_textX;
    m_height = m_Data->getMaxHeight() + 2 * m_textY;
}

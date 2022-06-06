#include "InfoWindow.h"

InfoWindow::InfoWindow(int y, int x, bool boxEn, int height, int width) : Window(y, x, boxEn, height, width)
{
    ColourInit();
}

InfoWindow::InfoWindow(std::shared_ptr<StateData> data, bool boxEn) : Window(0, 0, boxEn, 0, 0)
{
    ColourInit();
    SetData(data);
}

void InfoWindow::SetData(std::shared_ptr<StateData> data)
{
    m_Data = data;
    if (!m_width || !m_height)
    {
        CalcSize();
        SetSize(m_height, m_width);
        SizeUpdate();
    }
}

void InfoWindow::Update()
{
    TextUpdate();
    Window::Update();
}

int InfoWindow::ClickAction(int mouse_y, int mouse_x)
{
    if (IsClicked(mouse_y, mouse_x))
    {
        if (mouse_y == m_y + m_textY + 1 && mouse_x >= m_x + m_textX + m_Data->getNameInfo(L"").size() && mouse_x <= m_x + m_width - m_textX)
        {
            m_Data->name = GetWstr(mouse_y - 1 - m_y, m_textX + m_Data->getNameInfo(L"").size(), m_width - m_textX - m_textX - m_Data->getNameInfo(L"").size());
        }
        for (size_t i = 0; i < m_sensorsPos.size(); ++i)
        {
            if (m_sensorsPos[i].first + m_y == mouse_y)
            {
                m_Data->sensors[m_Data->sensors.size() - 1 - i] = !m_Data->sensors[m_Data->sensors.size() - 1 - i];
                break;
            }
        }
    }
    return 0;
}

void InfoWindow::ColourInit()
{
    init_pair(FluidType::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(FluidType::GASOLINE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FluidType::TANK, COLOR_WHITE, COLOR_WHITE);
}

void InfoWindow::TextUpdate()
{
    int x = m_textX;
    int y = m_textY;
    mvwaddwstr(m_window, y++, x, m_Data->getNameInfo(m_Data->name).data());
    mvwaddwstr(m_window, y++, x, m_Data->getFluidNameInfo().data());
    mvwaddwstr(m_window, y++, x, m_Data->getFullnessNameInfo().data());
    mvwaddwstr(m_window, y++, x, m_Data->getSensorsCaptionInfo().data());
    
    for (size_t i = m_Data->sensors.size() - 1; i != -1; i--)
    {
        std::wstring message = m_Data->getSensorNameInfo(i);
        std::wstring tank(50, L'0');
        mvwaddwstr(m_window, y, x, message.data());
        m_Data->sensors[i] ? ColorOn(m_Data->fluidType) : ColorOn(FluidType::TANK);
        m_sensorsPos.push_back(std::pair <int, int>(y + m_textY, x + message.size() + m_textX));
        mvwaddwstr(m_window, y++, x + message.size(), std::wstring(tank.begin(), tank.begin() + (m_width - message.size() - 2 * m_textX)).data());
        m_Data->sensors[i] ? ColorOff(m_Data->fluidType) : ColorOff(FluidType::TANK);
    }
}

void InfoWindow::CalcSize()
{
    m_width = m_Data->getMaxWidth() + 2 * m_textX;
    m_height = m_Data->getMaxHeight() + 2 * m_textY;
}

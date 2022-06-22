#include "SetupInfo.h"

int SetupInfo::ClickAction(int mouse_y, int mouse_x)
{
    return 0;
}

void SetupInfo::UpdateStrData()
{
    m_data->clear();
    int line = 0;
    m_data->push_back(StrData(L"Пристрій: ", line));
    m_data->push_back(StrData(m_device->name, line++));

    m_data->push_back(StrData(L"Слідкує за(ID): ", line));
    m_data->push_back(StrData(std::to_wstring(m_device->follow_id), line++));

    m_data->push_back(StrData(L"Порівняння: ", line));
    std::wstring comp;
    if (m_device->follow_comparison)
    {
        comp = L">";
    }
    else
    {
        comp = L"<";
    }
    m_data->push_back(StrData(comp, line++));

    m_data->push_back(StrData(L"Наповненість: ", line));
    m_data->push_back(StrData(std::to_wstring(m_device->follow_fullness) + L"%", line++));

    m_data->push_back(StrData(L"Налаштування виходів:", line++));

    m_pinStrID.clear();
    for (size_t i = m_device->pins_state.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Вихід " + std::to_wstring(i) + L": ", line));
        FluidType col = FluidType::OIL;
        if (m_device->pins_state[i])
            col = FluidType::CHEMICALS;
        m_pinStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"000", line++, 0, col));
    }
}

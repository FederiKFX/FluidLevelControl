#include "SetupInfo.h"

SetupInfo::SetupInfo(int y, int x, std::shared_ptr<Device> device) : m_device(device)
{
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    m_window = std::make_shared<Window>(m_data, y, x, true);
}

int SetupInfo::ClickAction(int mouse_y, int mouse_x)
{
    int i = m_window->ClickedAt(mouse_y, mouse_x);
    if (i != -1)
    {
        if (i == 2)
        {
            try
            {
                int num = std::stoi(m_window->GetWstr(3));
                m_device->follow_id = static_cast<FluidType>(num);
            }
            catch (const std::exception&) {}       
        }
        if (i == 4)
        {
            try
            {
                int num = std::stoi(m_window->GetWstr(5));
                m_device->follow_comparison = static_cast<Comparison>(num);
            }
            catch (const std::exception&) {}
        }
        if (i == 6)
        {
            try
            {
                int num = std::stoi(m_window->GetWstr(7));
                m_device->follow_fullness = num;
            }
            catch (const std::exception&) {}
        }
        auto pos = std::find(m_pinStrID.begin(), m_pinStrID.end(), i) - m_pinStrID.begin();
        if (pos < m_pinStrID.size()) {
            m_device->pins_state[m_device->pins_state.size() - 1 - pos] = !m_device->pins_state[m_device->pins_state.size() - 1 - pos];
        } 
        UpdateStrData();
    }
    return i;
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

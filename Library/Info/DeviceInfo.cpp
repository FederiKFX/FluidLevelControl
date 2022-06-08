#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(std::wstring name, Colour fluidType, int fullness, std::vector<bool> sensors) : name(name), fluidType(fluidType), fullness(fullness), sensors(sensors)
{
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    init_pair(Colour::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(Colour::GASOLINE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(Colour::TANK, COLOR_WHITE, COLOR_WHITE);
}

void DeviceInfo::UpdateStrData()
{
    m_data->clear();
    int line = 0;
    m_data->push_back(StrData(L"Назва: ", line));
    m_data->push_back(StrData(name, line++));
    
    m_data->push_back(StrData(L"Тип рідини: ", line));
    m_data->push_back(StrData(getFluidName(), line++));

    m_data->push_back(StrData(L"Заповненість: ", line));
    m_data->push_back(StrData(std::to_wstring(fullness) + L"%", line++));

    m_data->push_back(StrData(L"Датчики:", line++));

    for (size_t i = sensors.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Датчик " + std::to_wstring(i) + L": ", line));
        Colour col = Colour::TANK;
        if (sensors[i])
            col = fluidType;
        m_sensorStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"00000", line++, 0, col));
    } 
}

int DeviceInfo::ClickAction(int i)
{
    auto pos = std::find(m_sensorStrID.begin(), m_sensorStrID.end(), i) - m_sensorStrID.begin();
    if (pos < m_sensorStrID.size()) {
        sensors[sensors.size() - 1 - pos] = !sensors[sensors.size() - 1 - pos];
        UpdateStrData();
    }
    
    return pos;
}

std::wstring DeviceInfo::getFluidName()
{
    switch (fluidType)
    {
    case WATER:
        return L"Вода";
        break;
    case GASOLINE:
        return L"Бензина";
        break;
    default:
        return L"Невідома";
        break;
    }
}

#include "DeviceInfo.h"

void from_json(const nlohmann::json& j, Device& d)
{
    j.at("id").get_to(d.id);
    j.at("name").get_to(d.name);
    j.at("fluidType").get_to(d.fluidType);
    j.at("fullness").get_to(d.fullness);
    j.at("sensors").get_to(d.sensors);
    j.at("pins").get_to(d.pins);
}

void to_json(nlohmann::json& j, const Device& d)
{
    j = nlohmann::json{
        { "id", d.id },
        { "name", d.name },
        { "fluidType", d.fluidType },
        { "fullness", d.fullness },
        { "sensors", d.sensors },
        { "pins", d.pins }
    };
}

DeviceInfo::DeviceInfo(std::shared_ptr<Device> device): m_device(device)
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
    m_data->push_back(StrData(m_device->name, line++));
    
    m_data->push_back(StrData(L"Тип рідини: ", line));
    m_data->push_back(StrData(getFluidName(), line++));

    m_data->push_back(StrData(L"Заповненість: ", line));
    m_data->push_back(StrData(std::to_wstring(m_device->fullness) + L"%", line++));

    m_data->push_back(StrData(L"Датчики:", line++));

    for (size_t i = m_device->sensors.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Датчик " + std::to_wstring(i) + L": ", line));
        Colour col = Colour::TANK;
        if (m_device->sensors[i])
            col = m_device->fluidType;
        m_sensorStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"00000", line++, 0, col));
    } 
}

int DeviceInfo::ClickAction(int i)
{
    auto pos = std::find(m_sensorStrID.begin(), m_sensorStrID.end(), i) - m_sensorStrID.begin();
    if (pos < m_sensorStrID.size()) {
        m_device->sensors[m_device->sensors.size() - 1 - pos] = !m_device->sensors[m_device->sensors.size() - 1 - pos];
        UpdateStrData();
    }
    
    return i;
}

std::wstring DeviceInfo::getFluidName()
{
    switch (m_device->fluidType)
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

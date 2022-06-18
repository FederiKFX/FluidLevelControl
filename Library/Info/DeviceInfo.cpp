#include "DeviceInfo.h"

void from_json(const nlohmann::json& j, Device& d)
{
    if (j.contains("id"))
        j.at("id").get_to(d.id);
    if (j.contains("name"))
    {
        j.at("name").get_to(d.name);
    }
    else
    {
        d.name = L"NoName";
    }
    if (j.contains("fluidType"))
        j.at("fluidType").get_to(d.fluidType);
    if (j.contains("fullness"))
        j.at("fullness").get_to(d.fullness);
    if (j.contains("sensors"))
        j.at("sensors").get_to(d.sensors);
    if (j.contains("pins"))
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

DeviceInfo::DeviceInfo(std::shared_ptr<Device> device) : m_device(device)
{
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    m_window = std::make_shared<Window>(m_data, 0, 0, true);
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

    m_sensorStrID.clear();
    for (size_t i = m_device->sensors.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Датчик " + std::to_wstring(i) + L": ", line));
        Colour col = Colour::TANK;
        if (m_device->sensors[i])
            col = m_device->fluidType;
        m_sensorStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"000000", line++, 0, col));
    } 
}

int DeviceInfo::ClickAction(int mouse_y, int mouse_x)
{
    int i = m_window->ClickedAt(mouse_y, mouse_x);
    if (i != -1)
    {
        if (i == 0)
        {
            m_device->name = m_window->GetWstr(1);
        }
        if (i == 2)
        {
            m_device->fluidType = static_cast<Colour>(std::stoi(m_window->GetWstr(3)));
        }
        if (i == 6)
        {
            m_device->sensors.push_back(false);
        }
        auto pos = std::find(m_sensorStrID.begin(), m_sensorStrID.end(), i) - m_sensorStrID.begin();
        if (pos < m_sensorStrID.size()) {
            m_device->sensors[m_device->sensors.size() - 1 - pos] = !m_device->sensors[m_device->sensors.size() - 1 - pos];
        }
        int numOfOn = 0;
        for (auto sensor : m_device->sensors)
        {
            if (sensor)
            {
                numOfOn++;
            }
            else
            {
                break;
            }
        }
        m_device->fullness = (float)numOfOn / m_device->sensors.size() * 100;
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

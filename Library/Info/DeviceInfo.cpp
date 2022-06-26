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
    if (j.contains("erNum"))
        j.at("erNum").get_to(d.erNum);
    if (j.contains("sensors"))
        j.at("sensors").get_to(d.sensors);
    if (j.contains("pins"))
        j.at("pins").get_to(d.pinsState);
    if (j.contains("follow_id"))
        j.at("follow_id").get_to(d.follow_id);
    if (j.contains("follow_comparison"))
        j.at("follow_comparison").get_to(d.follow_comparison);
    if (j.contains("follow_fullness"))
        j.at("follow_fullness").get_to(d.follow_fullness);
    if (j.contains("pins_state"))
        j.at("pins_state").get_to(d.pinsStateConf);
}

void to_json(nlohmann::json& j, const Device& d)
{
    j = nlohmann::json{
        { "id", d.id },
        { "name", d.name },
        { "fluidType", d.fluidType },
        { "fullness", d.fullness },
        { "erNum", d.erNum },
        { "sensors", d.sensors },
        { "pins", d.pinsState },
        { "follow_id", d.follow_id },
        { "follow_comparison", d.follow_comparison },
        { "follow_fullness", d.follow_fullness },
        { "pins_state", d.pinsStateConf }
    };
}

DeviceInfo::DeviceInfo(std::shared_ptr<Device> device) : m_device(device)
{
    m_data = std::make_shared<std::vector<StrData>>();
    UpdateStrData();
    m_window = std::make_shared<Window>(m_data, 0, 0, true);
    init_pair(FluidType::WATER, COLOR_BLUE, COLOR_BLUE);
    init_pair(FluidType::GASOLINE, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(FluidType::OIL, COLOR_RED, COLOR_RED);
    init_pair(FluidType::MILK, COLOR_CYAN, COLOR_CYAN);
    init_pair(FluidType::CHEMICALS, COLOR_GREEN, COLOR_GREEN);
    init_pair(FluidType::OTHER, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(FluidType::TANK, COLOR_WHITE, COLOR_WHITE);
}

void DeviceInfo::UpdateStrData()
{
    m_data->clear();
    int line = 0;
    m_data->push_back(StrData(L"Назва: ", line));
    m_data->push_back(StrData(m_device->name, line++));
    
    m_data->push_back(StrData(L"Тип рідини: ", line));
    m_data->push_back(StrData(getFluidName(), line++));

    m_data->push_back(StrData(L"Наповненість: ", line));
    m_data->push_back(StrData(std::to_wstring(m_device->fullness) + L"%", line++));

    m_data->push_back(StrData(L"Датчики:", line++));

    m_sensorStrID.clear();
    for (size_t i = m_device->sensors.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Датчик " + std::to_wstring(i) + L": ", line));
        FluidType col = FluidType::TANK;
        if (m_device->sensors[i])
            col = m_device->fluidType;
        m_sensorStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"000000", line++, 0, col));
    } 

    m_data->push_back(StrData(L"Виходи:", line++));

    m_pinStrID.clear();
    for (size_t i = m_device->pinsState.size() - 1; i != -1; i--)
    {
        m_data->push_back(StrData(L"Вихід " + std::to_wstring(i) + L": ", line));
        FluidType col = FluidType::OIL;
        if (m_device->pinsState[i])
            col = FluidType::CHEMICALS;
        m_pinStrID.push_back(m_data->size());
        m_data->push_back(StrData(L"000", line++, 0, col));
    }

    //m_data->push_back(StrData(L"Налаштування:", line++));
}

int DeviceInfo::ClickAction(int mouse_y, int mouse_x)
{
    int i = m_window->ClickedAt(mouse_y, mouse_x);
    if (i != -1)
    {
#ifndef Terminal
        if (i == 0)
        {
            m_device->name = m_window->GetWstr(1);
        }
        if (i == 2)
        {
            try
            {
                int num = std::stoi(m_window->GetWstr(3));
                m_device->fluidType = static_cast<FluidType>(num);
            }
            catch (const std::exception&) {}
        }
#endif // Terminal
#ifndef Emulator
        if (i == 6)
        {
            m_device->sensors.push_back(false);
        }
        if (i == 7 + m_device->sensors.size() * 2)
        {
            m_device->pinsState.push_back(false);
            m_device->pinsStateConf.push_back(false);
        }
        auto pos = std::find(m_sensorStrID.begin(), m_sensorStrID.end(), i) - m_sensorStrID.begin();
        if (pos < m_sensorStrID.size()) {
            m_device->sensors[m_device->sensors.size() - 1 - pos] = !m_device->sensors[m_device->sensors.size() - 1 - pos];
        }
        int numOfOn = 0;
        bool check = true;
        m_device->erNum = 0;
        for (auto sensor : m_device->sensors)
        {
            if (sensor)
            {
                if (check)
                {
                    numOfOn++;
                }
                else
                {
                    m_device->erNum = 1;
                }
            }
            else
            {
                check = false;
            }
        }
        m_device->fullness = (float)numOfOn / m_device->sensors.size() * 100;

        auto posP = std::find(m_pinStrID.begin(), m_pinStrID.end(), i) - m_pinStrID.begin();
        if (posP < m_pinStrID.size()) {
            m_device->pinsState[m_device->pinsState.size() - 1 - posP] = !m_device->pinsState[m_device->pinsState.size() - 1 - posP];
        }
#endif // Emulator      
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
    case OIL:
        return L"Нафта";
        break;
    case MILK:
        return L"Молоко";
        break;
    case CHEMICALS:
        return L"Хімікати";
        break;
    case OTHER:
        return L"Інше";
        break;
    default:
        return L"Невідома";
        break;
    }
}
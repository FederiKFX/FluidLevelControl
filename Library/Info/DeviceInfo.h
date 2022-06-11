#pragma once
#include "Info.h"

struct Device 
{
    std::wstring name;
    Colour fluidType;
    int fullness;
    std::vector<bool> sensors;
    std::vector<bool> pins; 
};

void from_json(const nlohmann::json& j, Device& d)
{
    j.at("name").get_to(d.name);
    j.at("fluidType").get_to(d.fluidType);
    j.at("fullness").get_to(d.fullness);
    j.at("sensors").get_to(d.sensors);
    j.at("pins").get_to(d.pins);
}

void to_json(nlohmann::json& j, const Device& d)
{
    j = nlohmann::json{
        { "name", d.name },
        { "fluidType", d.fluidType },
        { "fullness", d.fullness },
        { "sensors", d.sensors },
        { "pins", d.pins }
    };
}

class DeviceInfo : public Info
{
public:
    DeviceInfo(std::wstring name, Colour fluidType, int fullness, std::vector<bool> sensors);
    void    UpdateStrData();
    int     ClickAction(int i);

private:
    std::wstring getFluidName();

    std::vector<int> m_sensorStrID;
public:
    std::shared_ptr<Device> m_device;
};


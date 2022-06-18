#pragma once
#include "Info.h"

struct Device 
{
    uint64_t id;
    std::wstring name;
    FluidType fluidType;
    uint32_t fullness;
    std::vector<bool> sensors;
    std::vector<bool> pins; 
};

void from_json(const nlohmann::json& j, Device& d);

void to_json(nlohmann::json& j, const Device& d);

class DeviceInfo : public Info
{
public:
    DeviceInfo(std::shared_ptr<Device> device);
    void    UpdateStrData();
    int     ClickAction(int mouse_y, int mouse_x);

public:
    std::shared_ptr<Device> m_device;

private:
    std::wstring getFluidName();

private:
    std::vector<int> m_sensorStrID;
    std::vector<int> m_pinStrID;
};


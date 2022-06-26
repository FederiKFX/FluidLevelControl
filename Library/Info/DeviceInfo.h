#pragma once
#include "Info.h"

enum Comparison
{
    LESS = 0,
    GREATE = 1
};

struct Device 
{
    uint64_t id;
    std::wstring name;
    FluidType fluidType;
    uint32_t fullness;
    int erNum;
    std::vector<bool> sensors;
    std::vector<bool> pins;
    uint64_t follow_id;
    Comparison follow_comparison;
    uint32_t follow_fullness;
    std::vector<bool> pins_state;
};

void from_json(const nlohmann::json& j, Device& d);

void to_json(nlohmann::json& j, const Device& d);

class DevEqu {
    std::shared_ptr<Device> m_dev;

public:
    DevEqu(std::shared_ptr<Device> dev) :m_dev(dev) {}
    bool operator()(std::shared_ptr<Device> dev) const
    {
        return m_dev->id == dev->id;
    }
};

class IDEqu {
    uint64_t m_id;

public:
    IDEqu(uint64_t id) :m_id(id) {}
    bool operator()(std::shared_ptr<Device> dev) const
    {
        return m_id == dev->id;
    }
};

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


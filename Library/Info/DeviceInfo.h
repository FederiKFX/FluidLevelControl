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


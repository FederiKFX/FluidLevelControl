#pragma once
#include "DeviceInfo.h"

class SetupInfo : public Info
{
public:
    int     ClickAction(int mouse_y, int mouse_x);
    void    UpdateStrData();

private:
    std::shared_ptr<Device> m_device;
    std::vector<int>        m_pinStrID;
};


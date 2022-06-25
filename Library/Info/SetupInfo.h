#pragma once
#include "DeviceInfo.h"

class SetupInfo : public Info
{
public:
            SetupInfo       (int y, int x, std::shared_ptr<Device> device);
    int     ClickAction     (int mouse_y, int mouse_x);
    void    UpdateStrData   ();

private:
    std::shared_ptr<Device> m_device;
    std::vector<int>        m_pinStrID;
};


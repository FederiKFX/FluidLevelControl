#define Terminal
#include "Window/Window.h"
#include "Info/MenuInfo.h"
#include "Info/DeviceInfo.h"
#include "Info/SetupInfo.h"
#include "WindowsGrid/WindowsGrid.h"

#include <iostream>

mmask_t old;
void initialize()
{
    initscr();
    noecho();
    nonl();
    raw();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old);
    curs_set(0);
    start_color();
}

void on_connect(struct mosquitto* mosq, void* obj, int rc) {
    if (rc == MOSQ_ERR_SUCCESS)
    {
        mosquitto_subscribe(mosq, NULL, "Status/#", 0);
    }
}

std::mutex devMut;

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {

    char** topics = new char* [10];
    for (int i = 0; i < 10; i++) {
        topics[i] = new char[20];
    }

    int count;
    if (mosquitto_sub_topic_tokenise(msg->topic, &topics, &count) == MOSQ_ERR_SUCCESS)
    {
        if (std::string(topics[0]) == "Status")
        {
            if (devMut.try_lock())
            {
                std::vector<std::shared_ptr<Device>>* devices = static_cast<std::vector<std::shared_ptr<Device>>*>(obj);

                nlohmann::json j = nlohmann::json::parse((char*)msg->payload);
                std::shared_ptr<Device> dev = std::make_shared<Device>(j.get<Device>());

                auto it = std::find_if(devices->begin(), devices->end(), DevEqu(dev));
                if (it == devices->end())
                {
                    devices->push_back(dev);
                }
                else
                {
                    **it = *dev;
                }
                devMut.unlock();
            } 
        }
    }
}

int main()
{
    SetConsoleTitle(L"Terminal");
    bool active = true;
    std::shared_ptr<std::vector<std::shared_ptr<Device>>> devices = std::make_shared<std::vector<std::shared_ptr<Device>>>();

    mosquitto_lib_init();
    struct mosquitto* mosq;

    std::thread mosqThread([&] {        
        mosq = mosquitto_new("Terminal", true, devices.get());

        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        if (mosquitto_connect(mosq, "192.168.1.128", 1883, 10) == MOSQ_ERR_SUCCESS)
        {
            mosquitto_loop_start(mosq);
            Sleep(500);
            //devices->push_back(std::make_shared<Device>(Device(63, L"Test", FluidType::WATER, 60, { 1,1,1,1,0,0 }, { 0,1 })));
            while (active) {}
            mosquitto_loop_stop(mosq, true);
        }

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        });

    mosqThread.detach();

    initialize();
    WindowsGrid infoWins(0, 30);

    std::shared_ptr<std::vector<std::tuple<uint64_t, std::wstring, FluidType>>> choices = std::make_shared<std::vector<std::tuple<uint64_t, std::wstring, FluidType>>>();
    choices->push_back(std::make_tuple<uint64_t, std::wstring, FluidType>(0, L"Змінити інформацію", FluidType::DEFAULT));
    choices->push_back(std::make_tuple<uint64_t, std::wstring, FluidType>(0, L"Змінити налаштування", FluidType::DEFAULT));
    choices->push_back(std::make_tuple<uint64_t, std::wstring, FluidType>(0, L"Вихід", FluidType::DEFAULT));
    std::shared_ptr<MenuInfo> menuInfo = std::make_shared<MenuInfo>(0, 0, choices);

    std::shared_ptr<std::vector<std::tuple<uint64_t, std::wstring, FluidType>>> devisesChoice = std::make_shared<std::vector<std::tuple<uint64_t, std::wstring, FluidType>>>();
    devisesChoice->push_back(std::make_tuple<uint64_t, std::wstring, FluidType>(0, L"     ", FluidType::DEFAULT));
    std::shared_ptr<MenuInfo> deviceList = std::make_shared<MenuInfo>(17, 0, devisesChoice);

    int ch;
    MEVENT event;
    std::mutex win;
    std::vector<uint64_t> activWins;
    refresh();
    while (active)
    {    
        std::thread([&] {
            while (active) {
                Sleep(1000);
                std::scoped_lock lck(win);
                devisesChoice->clear();
                for (auto dev : *devices)
                {
                    int numOfOn = 0;
                    dev->name.erase(std::remove(dev->name.begin(), dev->name.end(), '\0'), dev->name.end());
                    FluidType col = FluidType::DEFAULT;
                    if (dev->erNum)
                        col = FluidType::ERRORCOL;
                    devisesChoice->push_back(std::make_tuple(dev->id, dev->name, col));
                }
                infoWins.PosUpdate();
                infoWins.Update();
                deviceList->UpdateStrData();
                deviceList->m_window->Update();
                menuInfo->UpdateStrData();
                menuInfo->m_window->Update();
                refresh();
            }
        }).detach();

        ch = getch();
        switch (ch)
        {
        case KEY_RESIZE:
        {
            std::scoped_lock lck(win);
            resize_term(0, 0);
            deviceList->m_window->PosUpdate();
            menuInfo->m_window->PosUpdate();
            infoWins.PosUpdate();
            break;
        }
        case KEY_MOUSE:
        {           
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    std::scoped_lock lck(win);
                    int choice = menuInfo->ClickAction(event.y, event.x);
                    menuInfo->UpdateStrData();
                    menuInfo->m_window->Update();
                    if (choice != -1)
                    {
                        if (choice == 0)
                        {
                            std::scoped_lock lck(devMut);
                            uint64_t devId = -1;
                            while (true)
                            {
                                infoWins.Update();
                                ch = getch();
                                if (ch == KEY_MOUSE)
                                    if (getmouse(&event) == OK)
                                        if (event.bstate & BUTTON1_CLICKED)
                                            if (infoWins.ClickAction(event.y, event.x, devId) == -1)
                                                break;
                            }
                            menuInfo->SetHighlight(choice, 0);
                            if (devId != -1)
                            {
                                auto it = std::find_if(devices->begin(), devices->end(), IDEqu(devId));
                                nlohmann::json j = *((*it).get());
                                std::string msg = j.dump();
                                mosquitto_publish(mosq, NULL, ("SetupInfo/Dev" + std::to_string((*it)->id)).data(), msg.size(), msg.c_str(), 0, false);
                            }
                            
                        }
                        else if (choice == 1)
                        {
                            int dev = -1;
                            while (dev == -1)
                            {
                                ch = getch();
                                if (ch == KEY_MOUSE)
                                    if (getmouse(&event) == OK)
                                        if (event.bstate & BUTTON1_CLICKED)
                                            dev = deviceList->ClickedAt(event.y, event.x);
                            }
                            auto it = std::find_if(devices->begin(), devices->end(), IDEqu((*devices)[dev]->id));
                            SetupInfo deviceConf(5, 0, *it); 
                            deviceConf.UpdateStrData();
                            deviceConf.m_window->Update();
                            refresh();
                            refresh();
                            std::scoped_lock lck(devMut);
                            int cho = -1;
                            do
                            {
                                ch = getch();
                                if (ch == KEY_MOUSE)
                                    if (getmouse(&event) == OK)
                                        if (event.bstate & BUTTON1_CLICKED)
                                        {
                                            cho = deviceConf.ClickAction(event.y, event.x);
                                            deviceConf.m_window->Update();
                                            refresh();
                                        }
                            } while (cho != -1);
                            clear();
                            menuInfo->SetHighlight(choice, 0);
                            nlohmann::json j = *((*it).get());
                            std::string msg = j.dump();
                            mosquitto_publish(mosq, NULL, ("Setup/Dev" + std::to_string((*it)->id)).data(), msg.size(), msg.c_str(), 0, false);
                        }
                        else if (choice == choices->size() - 1)
                        {
                            active = false;
                        }
                    }
                    choice = deviceList->ClickAction(event.y, event.x);
                    if (choice != -1)
                    {
                        if (choice < devices->size())
                        {
                            auto it = std::find(activWins.begin(), activWins.end(), std::get<0>(devisesChoice->at(choice)));
                            if (it != activWins.end())
                            {
                                infoWins.Del(it - activWins.begin());
                                activWins.erase(it);
                                clear();
                            }
                            else
                            {
                                activWins.push_back(std::get<0>(devisesChoice->at(choice)));
                                infoWins.Add(std::make_shared<DeviceInfo>(devices->at(choice)));
                            }
                        }
                    }
                }
            }
            break;
        }
        }
        std::scoped_lock lck(win);
        infoWins.PosUpdate();
        infoWins.Update();
        deviceList->UpdateStrData();
        deviceList->m_window->Update();
        menuInfo->UpdateStrData();
        menuInfo->m_window->Update();
        refresh();
    }
    endwin();
    WaitForSingleObject(mosqThread.native_handle(), INFINITE);
    mosquitto_lib_cleanup();
    return 0;
}

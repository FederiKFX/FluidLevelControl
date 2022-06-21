#include "Window/Window.h"
#include "Info/MenuInfo.h"
#include "Info/DeviceInfo.h"
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

class DevEqu {
    std::shared_ptr<Device> m_dev;

public:
    DevEqu(std::shared_ptr<Device> dev) :m_dev(dev) {}
    bool operator()(std::shared_ptr<Device> dev) const
    {
        return m_dev->id == dev->id;
    }
};

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
        }
    }
}

int main()
{
    SetConsoleTitle(L"Terminal");
    bool active = true;
    std::shared_ptr<std::vector<std::shared_ptr<Device>>> devices = std::make_shared<std::vector<std::shared_ptr<Device>>>();

    mosquitto_lib_init();

    std::thread mosqThread([&] {
        struct mosquitto* mosq;
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

    std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices = std::make_shared<std::vector<std::pair<uint64_t, std::wstring>>>();
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Змінити інформацію"));
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Змінити налаштування"));
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Вихід"));
    std::shared_ptr<MenuInfo> menuInfo = std::make_shared<MenuInfo>(0, 0, choices);

    std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> devisesChoice = std::make_shared<std::vector<std::pair<uint64_t, std::wstring>>>();
    devisesChoice->push_back(std::make_pair<uint64_t, std::wstring>(0, L"     "));
    std::shared_ptr<MenuInfo> deviceList = std::make_shared<MenuInfo>(10, 0, devisesChoice);

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
                    devisesChoice->push_back(std::pair<uint64_t, std::wstring>(dev->id, dev->name));
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
                            while (true)
                            {
                                ch = getch();
                                if (ch == KEY_MOUSE)
                                    if (getmouse(&event) == OK)
                                        if (event.bstate & BUTTON1_CLICKED)
                                            if (infoWins.ClickAction(event.y, event.x) == -1)
                                            {
                                                menuInfo->SetHighlight(choice, 0);
                                                break;
                                            }
                            }
                        }
                        else if (choice == 1)
                        {
                            clear();
                            menuInfo->m_window->Update();
                            deviceList->m_window->Update();
                            refresh();
                            Sleep(10000);
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
                            auto it = std::find(activWins.begin(), activWins.end(), devisesChoice->at(choice).first);
                            if (it != activWins.end())
                            {
                                infoWins.Del(it - activWins.begin());
                                activWins.erase(it);
                                clear();
                            }
                            else
                            {
                                activWins.push_back(devisesChoice->at(choice).first);
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

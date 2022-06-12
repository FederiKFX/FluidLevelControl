#include "Window/Window.h"
#include "Info/MenuInfo.h"
#include "Info/DeviceInfo.h"
#include "WindowsGrid/WindowsGrid.h"

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
        //mosquitto_subscribe(mosq, NULL, "Status/#", 0);
        mosquitto_subscribe(mosq, NULL, "Setup/#", 0);
    }
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {

    char** topics = new char* [10];
    for (int i = 0; i < 10; i++) {
        topics[i] = new char[20];
    }

    int count;

    /*if (mosquitto_sub_topic_tokenise(msg->topic, &topics, &count) == MOSQ_ERR_SUCCESS)
    {
        if (std::string(topics[1]) == toUtf8(static_cast<Device*>(obj)->name))
        {
            if (std::string(topics[0]) == "Status")
            {
                std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                printf("New message from %s: %s\n", topics[1], (char*)msg->payload);
            }
            if (std::string(topics[0]) == "Setup")
            {
                std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                printf("New config to %s: %s\n", topics[1], (char*)msg->payload);
            }
        }
    }*/
}

void DeviceSimulation(std::shared_ptr<Device> device)
{
    struct mosquitto* mosq;
    mosq = mosquitto_new(("Dev" + std::to_string(device->id)).data(), true, device.get());

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, "localhost", 1883, 10) == MOSQ_ERR_SUCCESS)
    {
        while (true)
        {
            nlohmann::json j = *(device.get());
            std::string msg = j.dump();
            mosquitto_publish(mosq, NULL, ("Status/Dev" + std::to_string(device->id)).data(), msg.size(), msg.c_str(), 0, false);
            Sleep(10000);
        }
        mosquitto_loop_stop(mosq, true);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
    }
}

int main()
{
    bool active = true;

    std::vector<std::shared_ptr<Device>> devices;
    devices.push_back(std::make_shared<Device>());
    devices.push_back(std::make_shared<Device>());
    devices[0]->sensors = { 1,1,1,1,1,0,0,0 };
    devices[0]->id = 54;
    devices[0]->name = L"Dev0Тесті";
    devices[0]->fluidType = Colour::GASOLINE;

    devices[1]->sensors = { 1,1,1,1,0,0 };
    devices[1]->id = 18;
    devices[1]->name = L"Dev1";
    devices[1]->fluidType = Colour::WATER;

    for (auto dev : devices)
    {
        int numOfOn = 0;
        for (auto sensor : dev->sensors)
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
        dev->fullness = (float)numOfOn / dev->sensors.size() * 100;
    }

    mosquitto_lib_init();

    //std::thread thr1(DeviceSimulation, devices[0]);
    //std::thread thr2(DeviceSimulation, devices[1]);


    initialize();
    WindowsGrid infoWins(0, 30);

    std::shared_ptr<std::vector<std::pair<uint64_t, std::wstring>>> choices = std::make_shared<std::vector<std::pair<uint64_t, std::wstring>>>();
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Додати пристрій"));
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Видалити пристрій"));
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Вихід"));

    std::shared_ptr<MenuInfo> menuInfo = std::make_shared<MenuInfo>(choices);


    //int res = WaitForSingleObject(thr1.native_handle(), INFINITE);
    //thr1.detach();
    //Sleep(1000);
    //devices[0]->name = L"Dev010";
    //thr2.detach();


    int ch;
    MEVENT event;
    std::mutex win;
    std::vector<uint64_t> activWins;
    refresh();
    while (active)
    {
        std::thread([&] {
            while (active) {
                Sleep(800);
                std::scoped_lock lck(win);
                infoWins.PosUpdate();
                infoWins.Update();
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
                    bool is = infoWins.ClickAction(event.y, event.x);
                    if (choice != -1)
                    {
                        if (choice == choices->size() - 1)
                        {
                            active = false;
                        }
                        if (choice == 0)
                        {
                            std::shared_ptr<Device> dev = std::make_shared<Device>();
                            //dev->name = L"    ";
                            dev->id = devices.size() + 5;
                            devices.push_back(dev);
                            infoWins.Add(std::make_shared<DeviceInfo>(dev));
                            std::thread(DeviceSimulation, dev).detach();
                        }
                        /*else if (choice < devices->size())
                        {
                            auto it = std::find(activWins.begin(), activWins.end(), choices->at(choice).first);
                            if (it != activWins.end())
                            {
                                infoWins.Del(it - activWins.begin());
                                activWins.erase(it);
                                clear();
                            }
                            else
                            {
                                activWins.push_back(choices->at(choice).first);
                                infoWins.Add(new Window(std::make_shared<DeviceInfo>(devices->at(choice)), 0, 0, true));
                            }
                        }*/
                        mvprintw(22, 1, "Choice made is : %d", choice);
                    }
                }
            }
            break;
        }
        }
        std::scoped_lock lck(win);
        infoWins.PosUpdate();
        infoWins.Update();
        menuInfo->UpdateStrData();
        menuInfo->m_window->Update();
        refresh();
    }
    endwin();
    //WaitForSingleObject(mosqThread.native_handle(), INFINITE);
    mosquitto_lib_cleanup();

    return 0;
}
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
    //printf("ID: %d\n", *(int*)obj);
    std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
    if (rc) {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "Status/#", 0);
    mosquitto_subscribe(mosq, NULL, "Setup/#", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {

    char** topics = new char* [10];
    for (int i = 0; i < 10; i++) {
        topics[i] = new char[20];
    }

    int count;
    printf("New message from %s: %s\n", msg->topic, (char*)msg->payload);
    if (mosquitto_sub_topic_tokenise(msg->topic, &topics, &count) == MOSQ_ERR_SUCCESS)
    {
        printf("New message from %s: %s\n", msg->topic, (char*)msg->payload);
            if (std::string(topics[0]) == "Status")
            {
                //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                std::cout << (nlohmann::json::parse((char*)msg->payload)).dump();
            }
            if (std::string(topics[0]) == "Setup")
            {
                //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
                printf("New config to %s: %s\n", topics[1], (char*)msg->payload);
            }
    }
}

int main()
{
    bool active = true;
    std::shared_ptr<std::vector<std::shared_ptr<Device>>> devices = std::make_shared<std::vector<std::shared_ptr<Device>>>();
    devices->push_back(std::make_shared<Device>());
    devices->push_back(std::make_shared<Device>());
    devices->at(0)->sensors = { 1,1,1,1,1,0,0,0 };
    devices->at(1)->name = L"Dev0";

    devices->at(0)->sensors = { 1,1,1,1,0,0 };
    devices->at(1)->name = L"Dev1";

    for (auto dev : *devices)
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
    
    std::thread mosqThread([&] {
        struct mosquitto* mosq;
        mosq = mosquitto_new(NULL, true, devices.get());

        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        int rc = mosquitto_connect(mosq, "localhost", 1883, 10);
        if (rc) {
            printf("Could not connect to Broker with return code %d\n", rc);
            return -1;
        }
        std::string msg = "Working1";

        mosquitto_loop_start(mosq);
        //     printf("Press Enter to quit...\n");
        //     getchar();
        Sleep(500);
        /*mosquitto_publish(mosq, NULL, ("Status/" + toUtf8(devices[0]->name)).data(), msg.size(), msg.c_str(), 0, false);
        mosquitto_publish(mosq, NULL, ("Setup/" + toUtf8(devices[1]->name)).data(), msg.size(), msg.c_str(), 0, false);*/
        while (active)
        {

        }
        mosquitto_loop_stop(mosq, true);

        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        });

    mosqThread.detach();


    mosquitto_lib_cleanup();
    //active = false;
    //return 0;



     initialize();
     std::vector<std::shared_ptr<DeviceInfo>> devices;
     WindowsGrid infoWins(0, 30);
 
     devices.push_back(std::make_shared<DeviceInfo>(DeviceInfo{ L"Tffffank 1", Colour::GASOLINE, 0, { 1,1,1,1,0,0,0 } }));
     devices.push_back(std::make_shared<DeviceInfo>(DeviceInfo{ L"Tank 2", Colour::WATER, 0, { 1,1,1,1,1,0,0,0 } }));
     devices.push_back(std::make_shared<DeviceInfo>(DeviceInfo{ L"Tank 3", Colour::GASOLINE, 0, { 1,1,1,1,0,0,0 } }));
 
     std::vector<std::shared_ptr<std::wstring>> choices;
     for (auto dev : devices)
     {
         int numOfOn = 0;
         choices.push_back(std::make_shared<std::wstring>(dev->name));
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
     choices.push_back(std::make_shared<std::wstring>(L"Input: "));
     choices.push_back(std::make_shared<std::wstring>(L"Вихід"));
 
 
     std::shared_ptr<MenuInfo> menuInfo = std::make_shared<MenuInfo>(choices);
     Window menuWin(menuInfo, 10, 5, true);
 
 
     int ch;
     MEVENT event;
     refresh();
     while (active)
     {
         infoWins.PosUpdate();
         infoWins.Update();
         menuWin.Update();
         refresh();
 
         ch = getch();
         switch (ch)
         {
         case KEY_RESIZE:
         {
             resize_term(0, 0);
             menuWin.PosUpdate();
             infoWins.PosUpdate();
             break;
         }
         case KEY_MOUSE:
         {
             if (getmouse(&event) == OK)
             {
                 if (event.bstate & BUTTON1_CLICKED) {
                     int choice = choice = menuWin.ClickAction(event.y, event.x);
                     infoWins.ClickAction(event.y, event.x);
                     if (choice != -1)
                     {
                         if (choice == choices.size() - 1)
                         {
                             active = false;
                         }
                         else if (choice == choices.size() - 2)
                         {
                             //menuInfo->m_choices->at(choice) = menuWin.GetWstr(choice);
                             menuInfo->AddChoice(std::make_shared < std::wstring>(L"Input2: "));
                             //choices->push_back(L"Input2: ");
                             menuWin.Update();
                         }
                         else if (choice < devices.size())
                         {
                             infoWins.Add(new Window(devices[choice], 0, 0, true));
                         }
                         mvprintw(22, 1, "Choice made is : %d", choice);
                     }
                 }
             }            
             break;
         }
         }
     }
     endwin();
     WaitForSingleObject(mosqThread.native_handle(), INFINITE);
     return 0;
}

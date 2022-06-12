﻿#include "Window/Window.h"
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
    //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
    if (rc) {
        //printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "Status/#", 0);
    mosquitto_subscribe(mosq, NULL, "Setup/#", 0);
    mosquitto_subscribe(mosq, NULL, "Init", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {

    char** topics = new char* [10];
    for (int i = 0; i < 10; i++) {
        topics[i] = new char[20];
    }

    int count;
    //printf("New message from %s: %s\n", msg->topic, (char*)msg->payload);
    if (mosquitto_sub_topic_tokenise(msg->topic, &topics, &count) == MOSQ_ERR_SUCCESS)
    {
        //printf("New message from %s: %s\n", msg->topic, (char*)msg->payload);
        if (std::string(topics[0]) == "Status")
        {
            //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
            //std::cout << (nlohmann::json::parse((char*)msg->payload)).dump();
        }
        if (std::string(topics[0]) == "Setup")
        {
            //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
            //printf("New config to %s: %s\n", topics[1], (char*)msg->payload);
        }
        if (std::string(topics[0]) == "Init")
        {
            //std::cout << toUtf8(static_cast<Device*>(obj)->name) << std::endl;
            nlohmann::json j = nlohmann::json::parse((char*)msg->payload);
            static_cast<std::vector<std::shared_ptr<Device>>*>(obj)->push_back(std::make_shared<Device>(j.get<Device>()));
            //printf("New config to %s: %s\n", topics[1], (char*)msg->payload);
        }
    }
}

int main()
{
    bool active = true;
    std::shared_ptr<std::vector<std::shared_ptr<Device>>> devices = std::make_shared<std::vector<std::shared_ptr<Device>>>();

    mosquitto_lib_init();

    std::thread mosqThread([&] {
        struct mosquitto* mosq;
        mosq = mosquitto_new(NULL, true, devices.get());

        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);

        if (mosquitto_connect(mosq, "localhost", 1883, 10) == MOSQ_ERR_SUCCESS)
        {
            mosquitto_loop_start(mosq);
            Sleep(500);
            devices->push_back(std::make_shared<Device>(Device(63, L"Test", Colour::WATER, 60, { 1,1,1,1,0,0 }, { 0,1 })));
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
    choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Вихід"));

    std::shared_ptr<MenuInfo> menuInfo = std::make_shared<MenuInfo>(choices);
    Window menuWin(menuInfo, 10, 5, true);


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
                choices->clear();
                for (auto dev : *devices)
                {
                    int numOfOn = 0;
                    choices->push_back(std::pair<uint64_t, std::wstring>(dev->id, dev->name));
                }
                choices->push_back(std::make_pair<uint64_t, std::wstring>(0, L"Вихід"));
                infoWins.PosUpdate();
                infoWins.Update();
                menuInfo->UpdateStrData();
                menuWin.Update();
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
            menuWin.PosUpdate();
            infoWins.PosUpdate();
            break;
        }
        case KEY_MOUSE:
        {           
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    std::scoped_lock lck(win);
                    int choice = choice = menuWin.ClickAction(event.y, event.x);
                    infoWins.ClickAction(event.y, event.x);
                    if (choice != -1)
                    {
                        if (choice == choices->size() - 1)
                        {
                            active = false;
                        }
                        else if (choice < devices->size())
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
                        }
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
        menuWin.Update();
        refresh();
    }
    endwin();
    WaitForSingleObject(mosqThread.native_handle(), INFINITE);
    mosquitto_lib_cleanup();
    return 0;
}

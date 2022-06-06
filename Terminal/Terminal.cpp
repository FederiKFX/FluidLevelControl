﻿#include "MenuWindow/MenuWindow.h"
#include "InfoWindow/InfoWindow.h"
#include "WindowsGrid/WindowsGrid.h"
#include <nlohmann/json.hpp>


std::string getstring()
{
    std::string input;

    // let the terminal do the line editing
    nocbreak();
    echo();

    // this reads from buffer after <ENTER>, not "raw" 
    // so any backspacing etc. has already been taken care of
    int ch = getch();

    while (ch != 13)
    {
        input.push_back(ch);
        ch = getch();
    }

    noecho();
    // restore your cbreak / echo settings here

    return input;
}


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

int main()
{
    initialize();
    std::vector<std::shared_ptr<StateData>> devices;
    WindowsGrid infoWins(0, 30);

    devices.push_back(std::make_shared<StateData>(StateData{ L"Tank 1", FluidType::GASOLINE, 0, { 1,1,1,1,0,0,0 } }));
    devices.push_back(std::make_shared<StateData>(StateData{ L"Tank 2", FluidType::WATER, 0, { 1,1,1,1,1,0,0,0 } }));
    devices.push_back(std::make_shared<StateData>(StateData{ L"Tank 3", FluidType::GASOLINE, 0, { 1,1,1,1,0,0,0 } }));

    std::vector<std::wstring> choices;
    for (auto dev : devices)
    {
        int numOfOn = 0;
        choices.push_back(dev->name);
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
    choices.push_back(L"Input: ");
    choices.push_back(L"Вихід");



    MenuWindow statusWin(10, 5, true, 10, 20);
    statusWin.SetChoices(choices);
    statusWin.SetCaption(L"Виберіть датчик:");

    refresh();
    statusWin.Update();

    int choice = 0;
    int xStartPos = 30;
    int y = 0, x = xStartPos;
    int heightSTD = 0, widthSTD = 0;
    int height = 0, width = 0;
    int ch;
    bool active = true;
    MEVENT event;
    while (active)
    {
        ch = getch();
        switch (ch)
        {
        case KEY_RESIZE:
        {
            resize_term(0, 0);
            statusWin.PosUpdate();
            statusWin.Update();
            infoWins.PosUpdate();
            infoWins.Update();
            break;
        }
        case KEY_MOUSE:
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    choice = statusWin.ClickAction(event.y, event.x);
                    infoWins.ClickAction(event.y, event.x);
                    if (choice != -1)
                    {
                        if (choice == choices.size() - 1)
                        {
                            active = false;
                        }
                        else if (choice == choices.size() - 2)
                        {
                            std::wstring t = statusWin.GetWstr(0,0,0);
                            int k = 5;
                        }
                        else
                        {
                            infoWins.Add(new InfoWindow(devices[choice], true));
                            infoWins.SetRename(true);
                            //mvprintw(22, 1, "Choice made is : %d. String Chosen is \"%10s\"", choice, choices[choice].data());
                        }
                    }
                    refresh();
                }
            }
            refresh();
            infoWins.PosUpdate();
            infoWins.Update();
            statusWin.Update();
            break;
        }
        }
    }
    endwin();
    return 0;
}

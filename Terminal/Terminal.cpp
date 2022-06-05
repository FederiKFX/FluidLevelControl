#include "MenuWindow/MenuWindow.h"
#include "InfoWindow/InfoWindow.h"
#include "WindowsGrid/WindowsGrid.h"
#include <nlohmann/json.hpp>

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
    choices.push_back(L"Вихід");



    MenuWindow statusWin(10, 5, false, 10, 10);
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
                    choice = statusWin.ClickAction(event.y + 1, event.x + 1);
                    infoWins.ClickAction(event.y + 1, event.x + 1);
                    if (choice != -1)
                    {
                        if (choice == choices.size() - 1)
                        {
                            active = false;
                        }
                        else
                        {
                            infoWins.Add(new InfoWindow(devices[choice], true));
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

#include "MenuWindow/MenuWindow.h"
#include "InfoWindow/InfoWindow.h"
#include <nlohmann/json.hpp>

std::vector<std::shared_ptr<StateData>> devices;
std::vector<std::shared_ptr<InfoWindow>> infoWins;

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

    devices.push_back(std::make_shared<StateData>(StateData{ L"Tank 1", FluidType::GASOLINE, 0, { 1,1,1,1,0,0,0 } }));
    devices.push_back(std::make_shared<StateData>(StateData{ L"Tank 2", FluidType::WATER, 0, { 1,1,1,1,1,0,0,0 } }));

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



    MenuWindow statusWin(10, 5, true, 10, 10);
    statusWin.SetChoices(choices);
    statusWin.SetCaption(L"test");

    //InfoWindow infoWin(20, 20, 5, 30, true);
    
    
    //infoWin.SetData(data);

    refresh();
    statusWin.Update();
    //infoWin.Update();


    int choice = 0;
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
            for (auto win : infoWins)
            {
                win->PosUpdate();
                win->Update();
            }
            //infoWin.PosUpdate();
            //infoWin.Update();
            break;
        }
        case KEY_MOUSE:
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    choice = statusWin.ReportChoice(event.y + 1, event.x + 1);
                    if (choice == 0)
                    {
                        infoWins.push_back(std::make_shared<InfoWindow>(InfoWindow(5, 30, true)));
                        infoWins[0]->SetData(devices[0]);
                    }
                    if (choice == 1)
                    {
                        infoWins.push_back(std::make_shared<InfoWindow>(InfoWindow(5, 60, true)));
                        infoWins[1]->SetData(devices[1]);
                    }
                    if (choice == 2)
                        active = false;
                    if (choice != -1)
                        mvprintw(22, 1, "Choice made is : %d. String Chosen is \"%10s\"", choice, choices[choice].data());
                    refresh();
                }
            }
            refresh();
            for (auto win : infoWins)
            {
                win->Update();
            }
            statusWin.Update();
            break;
        }
        }
    }
    endwin();
    return 0;
}

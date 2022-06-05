#include "MenuWindow/MenuWindow.h"
#include "InfoWindow/InfoWindow.h"
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
    std::vector<std::shared_ptr<InfoWindow>> infoWins;

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

                    if (choice == choices.size() - 1)
                    {
                        active = false;
                    }
                    else
                    {
                        if (!infoWins.empty())
                        {
                            getmaxyx(stdscr, heightSTD, widthSTD);
                            x += infoWins.back()->GetWidth();
                            if (x + width > widthSTD)
                            {
                                y += height;
                                x = xStartPos;
                            }
                        }
                        infoWins.push_back(std::make_shared<InfoWindow>(InfoWindow(y, x, true)));
                        infoWins.back()->SetData(devices[choice]);
                        width < infoWins.back()->GetWidth() ? width = infoWins.back()->GetWidth() : NULL;
                        height < infoWins.back()->GetHeight() ? height = infoWins.back()->GetHeight() : NULL;
                        /*for (auto win : infoWins)
                        {
                            win->Resize(height, width);
                        }*/
                    }
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

#include "MenuWindow/MenuWindow.h"
#include <nlohmann/json.hpp>


typedef struct _StateData
{
    std::string name;
    std::vector<bool> sensors;
    int fullness;

} StateData;

std::vector<std::wstring> choices = {
    L"Пристрій 1🔐✈️🛸",
    L"Choice 2",
    L"Choice 3",
    L"Choice 4",
    L"Вихід"
};

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
}

int main()
{
    initialize();

    MenuWindow statusWin(10, 10, 10, 5);
    statusWin.SetChoices(choices);
    statusWin.SetCaption(L"test");

    refresh();
    statusWin.Update();


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
            break;
        }
        case KEY_MOUSE:
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    choice = statusWin.ReportChoice(event.y + 1, event.x + 1);
                    if (choice == 4)
                        active = false;
                    if (choice != -1)
                        mvprintw(22, 1, "Choice made is : %d. String Chosen is \"%10s\"", choice, choices[choice].data());
                    refresh();
                }
            }
            statusWin.Update();
            break;
        }
        }
    }
    endwin();
    return 0;
}

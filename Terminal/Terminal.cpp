#include "global.h"
#include "MenuWindow.h"

std::vector<std::string> choices = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit"
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

    MenuWindow statusWin(10, 10, 10, 5, true);
    statusWin.setChoices(choices);
    
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
            //mvwin(statusWin, 10, 5);
            statusWin.Update();
            break;
        }
        case KEY_MOUSE:
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    choice = statusWin.ReportChoice(event.x + 1, event.y + 1);
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
end:;
    endwin();
    system("pause");
    return 0;
}

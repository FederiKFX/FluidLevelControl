#define PDC_WIDE
#define NCURSES_MOUSE_VERSION
#include "global.h"

std::vector<std::string> choices = {
    "Choice 1",
    "Choice 2",
    "Choice 3",
    "Choice 4",
    "Exit"
};

void print_menu(WINDOW* menu_win, int highlight)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < choices.size(); ++i)
    {
        if (highlight == i)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].data());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i].data());
        ++y;
    }
    wrefresh(menu_win);
}

int report_choice(int mouse_x, int mouse_y)
{
    int i, j, choice;
    int ret = -1;
    i = 5 + 2;
    j = 10 + 3;

    for (choice = 0; choice < choices.size(); ++choice)
    {
        if (mouse_y == j + choice && mouse_x >= i && mouse_x <= i + choices[choice].size())
        {
            ret = choice;
            break;
        }
    }
    return ret;
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
}

int main()
{
    WINDOW* statusWin;
    int widthMain = 0, heightMain = 0;

    initialize();
    getmaxyx(stdscr, heightMain, widthMain);

    statusWin = newwin(10, 10 * 2, 10, 5);
    box(statusWin, 0, 0);
    refresh();
    
    print_menu(statusWin, 0);



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
            getmaxyx(stdscr, heightMain, widthMain);
            mvwin(statusWin, 10, 5);
            wrefresh(statusWin);
            break;
        }
        case KEY_MOUSE:
        {
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON1_CLICKED) {
                    choice = report_choice(event.x + 1, event.y + 1);
                    if (choice == 4)
                        active = false;
                    if (choice != -1)
                        mvprintw(22, 1, "Choice made is : %d. String Chosen is \"%10s\"", choice, choices[choice].data());
                    refresh();
                }
            }
            print_menu(statusWin, choice);
            break;
        }
        }
    }
end:;
    endwin();
    system("pause");
    return 0;
}

#include <stdio.h>
#include "simpleCMenu.h"
#include <conio.h>
#include <windows.h>

void displayMenuItem(MenuItemHandle menuItemHandle)
{
    printf("  %s\n", menuItemHandle->name);
}
void displaySelectedMenuItem(MenuItemHandle menuItemHandle)
{
    printf("<<%s>>\n", menuItemHandle->name);
}

void hideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // 设置光标不可见
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void showCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE; // 设置光标可见
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main(void)
{
    hideCursor();
    MenuHandle mainMenu = initMenu(displayMenuItem, displaySelectedMenuItem);
    MenuHandle subMenu = initMenu(displayMenuItem, displaySelectedMenuItem);

    MenuItemHandle mainMenuItem1 = initMenuItem("[A] enter", ENTER_MENU_TYPE, NULL, NULL, subMenu);
    registerMenuItem(mainMenu, mainMenuItem1);
    MenuItemHandle mainMenuItem2 = initMenuItem("[B] Item2", ENTER_MENU_TYPE, NULL, NULL, NULL);
    registerMenuItem(mainMenu, mainMenuItem2);
    MenuItemHandle mainMenuItem3 = initMenuItem("[C] Item3", ENTER_MENU_TYPE, NULL, NULL, NULL);
    registerMenuItem(mainMenu, mainMenuItem3);

    MenuItemHandle subMenuItem1 = initMenuItem("[A] return", EXIT_MENU_TYPE, NULL, mainMenu, NULL);
    registerMenuItem(subMenu, subMenuItem1);
    MenuItemHandle subMenuItem2 = initMenuItem("[B] Item2", ENTER_MENU_TYPE, NULL, NULL, NULL);
    registerMenuItem(subMenu, subMenuItem2);

    char c;
    updateCurrentMenu(mainMenu);
    while (1)
    {
        if (kbhit())
        {
            if (GetAsyncKeyState(VK_UP))
            {
                system("cls");
                updateCurrentMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                system("cls");
                updateCurrentMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'C' && c >= 'A')
            {
                system("cls");
                updateCurrentMenuItem(c);
                printf("command is [%c]\n", c);
            }
            else if (c == '\r')
            {
                triggerCurrentMenuAction();
            }
            else if (c == 'C')
                return 0;
            Sleep(100); // 软件延时消抖
        }
    }

    return 0;
}
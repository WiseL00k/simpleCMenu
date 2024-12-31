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
    MenuItemHandle mainMenuItem1 = initMenuItem("[A] Item1", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem1);
    MenuItemHandle mainMenuItem2 = initMenuItem("[B] Item2", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem2);
    MenuItemHandle mainMenuItem3 = initMenuItem("[C] Item3", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem3);

    char c;

    display(mainMenu);
    while (1)
    {
        if (kbhit())
        {
            if (GetAsyncKeyState(VK_UP))
            {
                system("cls");
                updateCurrentMenuItemHandle(mainMenu, UP);
                display(mainMenu);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                system("cls");
                updateCurrentMenuItemHandle(mainMenu, DOWN);
                display(mainMenu);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'C' && c >= 'A')
            {
                // sel = c - 'A' + 1;
                system("cls");
                updateCurrentMenuItemHandle(mainMenu, c);
                display(mainMenu);
                printf("command is [%c]\n", c);
            }
            else if (c == '\r')
            {
                
            }
            else if (c == 'C')
                return 0;
            Sleep(100); // 软件延时消抖
        }
    }

    return 0;
}
#include <stdio.h>
#include "simpleCMenu.h"
#include <conio.h>
#include <windows.h>

struct node
{
    char name[20];
    int data;
    struct node *next;
};

// your functions
struct node *creatlink(struct node *head);
void printstring(struct node *head);
void quit();

// your menu loops
void mainMenuLoop(MenuHandle menuHandle);
void subMenu1Loop(MenuHandle menuHandle);
void subMenu2Loop(MenuHandle menuHandle);

// your display functions
void displayMenuItem(MenuItemHandle menuItemHandle);
void displaySelectedMenuItem(MenuItemHandle menuItemHandle);
void hideCursor();
void showCursor();
void goToxy(int x, int y);



// init your menus
void initAllMenus(MenuHandle mainMenu);

int main(void)
{
    initMenuDisplayFunctions(displayMenuItem, displaySelectedMenuItem, goToxy);

    MenuHandle mainMenu = initMenu(mainMenuLoop);

    initAllMenus(mainMenu);

    runMainMenu(mainMenu);

    return 0;
}

void mainMenuLoop(MenuHandle menuHandle)
{
    struct node *head = NULL;
    char c;

    while (isCurrentMenu(menuHandle))
    {
        if (kbhit())
        {
            if (GetAsyncKeyState(VK_UP))
            {
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'D' && c >= 'A')
            {
                updateSelectedMenuItem(c);
            }
            else if (c == '\r')
            {
                switch (getSelectedMenuItemTag())
                {
                case 'A':
                    changeCurrentMenu();
                    break;
                case 'B':
                {
                    head = creatlink(head);
                    system("pause");
                    break;
                }
                case 'C':
                    printstring(head);
                    system("pause");
                    break;
                case 'D':
                    quit();
                default:
                    break;
                }
                if (isCurrentMenu(menuHandle))
                    updateCurrentMenu(menuHandle);
            }
            else if (c == 'C')
                exit(0);
            Sleep(100); // 软件延时消抖
        }
    }
}

void subMenu1Loop(MenuHandle menuHandle)
{
    char c;
    while (isCurrentMenu(menuHandle))
    {
        if (kbhit())
        {
            if (GetAsyncKeyState(VK_UP))
            {
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'B' && c >= 'A')
            {
                updateSelectedMenuItem(c);
            }
            else if (c == '\r')
            {
                switch (getSelectedMenuItemTag())
                {
                case 'A':
                    changeCurrentMenu();
                    break;
                case 'B':
                    changeCurrentMenu();
                default:
                    break;
                }
                if (isCurrentMenu(menuHandle))
                    updateCurrentMenu(menuHandle);
            }
            Sleep(100); // 软件延时消抖
        }
    }
}

void subMenu2Loop(MenuHandle menuHandle)
{
    char c;
    while (isCurrentMenu(menuHandle))
    {
        if (kbhit())
        {
            if (GetAsyncKeyState(VK_UP))
            {
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'A' && c >= 'A')
            {
                updateSelectedMenuItem(c);
                printf("command is [%c]\n", c);
            }
            else if (c == '\r')
            {
                switch (getSelectedMenuItemTag())
                {
                case 'A':
                    changeCurrentMenu();
                    break;
                default:
                    break;
                }
                if (isCurrentMenu(menuHandle))
                    updateCurrentMenu(menuHandle);
            }
            Sleep(100); // 软件延时消抖
        }
    }
}

void initAllMenus(MenuHandle mainMenu)
{
    hideCursor();
    MenuHandle subMenu1 = initMenu(subMenu1Loop);
    MenuHandle subMenu2 = initMenu(subMenu2Loop);

    MenuItemHandle mainMenuItem1 = initChangeMenuItem("enter subMenu1", ENTER_MENU_TYPE, subMenu1);
    MenuItemHandle mainMenuItem2 = initExecFuncMenuItem("creatlink");
    MenuItemHandle mainMenuItem3 = initExecFuncMenuItem("printstring");
    MenuItemHandle mainMenuItem4 = initExecFuncMenuItem("exit");
    registerMenuItem(mainMenu, mainMenuItem1);
    registerMenuItem(mainMenu, mainMenuItem2);
    registerMenuItem(mainMenu, mainMenuItem3);
    registerMenuItem(mainMenu, mainMenuItem4);

    MenuItemHandle subMenu1Item1 = initChangeMenuItem("return mainMenu", EXIT_MENU_TYPE, mainMenu);
    registerMenuItem(subMenu1, subMenu1Item1);
    MenuItemHandle subMenu1Item2 = initChangeMenuItem("enter subMenu2", ENTER_MENU_TYPE, subMenu2);
    registerMenuItem(subMenu1, subMenu1Item2);

    MenuItemHandle subMenu2Item1 = initChangeMenuItem("return subMenu1", EXIT_MENU_TYPE, subMenu1);
    registerMenuItem(subMenu2, subMenu2Item1);
}

void quit()
{
    puts("are you sure to exit? y/n");
    char c = getch();
    if (c == 'y' || c == 'Y')
        exit(0);
}

void displayMenuItem(MenuItemHandle menuItemHandle)
{
    printf("%s  \n", menuItemHandle->name);
}

void displaySelectedMenuItem(MenuItemHandle menuItemHandle)
{
    printf("  \033[7m%s\033[0m\n", menuItemHandle->name);
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

void goToxy(int x, int y)
{
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出设备句柄
    SetConsoleCursorPosition(hOut, pos);           // 两个参数分别是指定哪个窗体，具体位置
}

struct node *creatlink(struct node *head)
{
    struct node *p, *temp;
    head = NULL;
    char str[20];
    int data;
    printf("Please input the name and data:\n");
    while (1)
    {
        puts("Input name:");
        fflush(stdin);
        gets(str);
        if (strlen(str) == 0)
        {
            fflush(stdin);
            break;
        }
        puts("Input data:");
        scanf(" %d", &data);
        if (!head)
        {
            head = (struct node *)malloc(sizeof(struct node));
            head->next = NULL;
            strcpy(head->name, str);
            head->data = data;
            p = head;
        }
        else
        {
            temp = (struct node *)malloc(sizeof(struct node));
            strcpy(temp->name, str);
            temp->data = data;
            temp->next = NULL;
            p->next = temp;
            p = temp;
        }
    }
    return head;
}

void printstring(struct node *head)
{
    struct node *p = head;
    while (p)
    {
        printf("%s %d\n", p->name, p->data);
        p = p->next;
    }
}


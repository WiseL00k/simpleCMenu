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
int checkstring(struct node *head, char *str);
void savetofile(struct node *head, char *filename);
struct node *getfromfile(struct node *head, char *filename);
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

// init your menus
void initAllMenus(MenuHandle mainMenu);

int main(void)
{
    MenuHandle mainMenu = initMenu(displayMenuItem, displaySelectedMenuItem, mainMenuLoop);

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
                system("cls");
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                system("cls");
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'D' && c >= 'A')
            {
                system("cls");
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
                case 'B':
                {
                    head = creatlink(head);
                    break;
                }
                case 'C':
                    printstring(head);
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
                system("cls");
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                system("cls");
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'B' && c >= 'A')
            {
                system("cls");
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
                system("cls");
                updateSelectedMenuItem(UP);
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                system("cls");
                updateSelectedMenuItem(DOWN);
            }
            c = getch();
            if (c <= 'z' && c >= 'a')
                c -= ('a' - 'A');
            if (c <= 'A' && c >= 'A')
            {
                system("cls");
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
    MenuHandle subMenu1 = initMenu(displayMenuItem, displaySelectedMenuItem, subMenu1Loop);
    MenuHandle subMenu2 = initMenu(displayMenuItem, displaySelectedMenuItem, subMenu2Loop);

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

int checkstring(struct node *head, char *str)
{
    struct node *p = head;
    while (p)
    {
        if (strcmp(p->name, str) == 0)
            return 1;
        p = p->next;
    }
    return -1;
}

void savetofile(struct node *head, char *filename)
{
    FILE *fp;
    struct node *p = head;
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        exit(0);
    }
    while (p)
    {
        fprintf(fp, "%s %d\n", p->name, p->data);
        p = p->next;
    }
    fclose(fp);
}

struct node *getfromfile(struct node *head, char *filename)
{
    FILE *fp;
    struct node *p, *temp;
    head = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        exit(0);
    }
    while (1)
    {
        if (!head)
        {
            head = (struct node *)malloc(sizeof(struct node));
            head->next = NULL;
            if (fscanf(fp, "%s %d", head->name, &head->data) != 2)
            {
                free(head);
                break;
            }
            p = head;
        }
        else
        {
            temp = (struct node *)malloc(sizeof(struct node));
            temp->next = NULL;
            if (fscanf(fp, "%s %d", temp->name, &temp->data) != 2)
            {
                free(temp);
                break;
            }
            p->next = temp;
            p = temp;
        }
    }
    fclose(fp);
    return head;
}

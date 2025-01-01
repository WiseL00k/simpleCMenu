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

struct node *creatlink(struct node *head);
void *printstring(struct node *head);
int checkstring(struct node *head, char *str);
void savetofile(struct node *head, char *filename);
struct node *getfromfile(struct node *head, char *filename);

void displayMenuItem(MenuItemHandle menuItemHandle);
void displaySelectedMenuItem(MenuItemHandle menuItemHandle);
void hideCursor();
void showCursor();

int main(void)
{
    hideCursor();
    MenuHandle mainMenu = initMenu(displayMenuItem, displaySelectedMenuItem);
    MenuHandle subMenu = initMenu(displayMenuItem, displaySelectedMenuItem);

    MenuItemHandle mainMenuItem1 = initMenuItem("[A] enter", ENTER_MENU_TYPE, NULL, NULL, subMenu);
    registerMenuItem(mainMenu, mainMenuItem1);
    MenuItemHandle mainMenuItem2 = initMenuItem("[B] creatlink", EXECUTIVE_FUNCTION_TYPE, creatlink, NULL, NULL);
    registerMenuItem(mainMenu, mainMenuItem2);
    MenuItemHandle mainMenuItem3 = initMenuItem("[C] printstring", EXECUTIVE_FUNCTION_TYPE, printstring, NULL, NULL);
    registerMenuItem(mainMenu, mainMenuItem3);

    MenuItemHandle subMenuItem1 = initMenuItem("[A] return", EXIT_MENU_TYPE, NULL, mainMenu, NULL);
    registerMenuItem(subMenu, subMenuItem1);
    MenuItemHandle subMenuItem2 = initMenuItem("[B] Item2", ENTER_MENU_TYPE, NULL, NULL, NULL);
    registerMenuItem(subMenu, subMenuItem2);

    struct node *head = NULL;
    char c, currentMenuItemTag = 'A';
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
                switch (getCurrentMenuItemTag())
                {
                case 'A':
                    triggerCurrentMenuAction(NULL);
                    break;
                case 'B':
                {
                    head = triggerCurrentMenuAction(head);
                    break;
                }
                case 'C':
                    triggerCurrentMenuAction(head);
                    break;
                default:
                    break;
                }
            }
            else if (c == 'C')
                return 0;
            Sleep(100); // 软件延时消抖
        }
    }

    return 0;
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

void *printstring(struct node *head)
{
    struct node *p = head;
    while (p)
    {
        printf("%s %d\n", p->name, p->data);
        p = p->next;
    }

    return NULL;
}
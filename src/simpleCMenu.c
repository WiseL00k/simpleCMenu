#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "simpleCMenu.h"

static MenuHandle currentMenuHandle = NULL;
static MenuDisplayFunctions menuDisplayFunctions = {NULL, NULL, NULL};
static time_t currentTime;

void clearMenu()
{
    printf("\033[H\033[J"); // 清屏
}

char getSelectedMenuItemTag()
{
    return currentMenuHandle->selectedMenuItemHandle->tag;
}

void changeCurrentMenu()
{
    if (currentMenuHandle == NULL)
        return;
    switch (currentMenuHandle->selectedMenuItemHandle->type)
    {
    case ENTER_MENU_TYPE:
        currentMenuHandle->selectedMenuItemHandle->un.enter.enterMenuAction(currentMenuHandle->selectedMenuItemHandle);
        break;
    case EXIT_MENU_TYPE:
        currentMenuHandle->selectedMenuItemHandle->un.exit.exitMenuAction(currentMenuHandle->selectedMenuItemHandle);
        break;
    default:
        break;
    }

    updateCurrentMenu(currentMenuHandle);
}

void updateCurrnetMenuTime()
{
    currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime); // 转换为当地时间
    sprintf(currentMenuHandle->bottomMenuInfo, "Current Time:%d-%d-%d %02d:%02d:%02d \n",
            localTime->tm_year + 1900, // 年（从1900年起）
            localTime->tm_mon + 1,     // 月（0-11，需加1）
            localTime->tm_mday,        // 日
            localTime->tm_hour,        // 时
            localTime->tm_min,         // 分
            localTime->tm_sec);        // 秒
}

void displayMenu(MenuHandle menuHandle)
{
    MenuItem *p = menuHandle->menuItemListHandle->head;
    clearMenu();
    for(int i = 0; i < menuHandle->topMenuInfoRows; i++)
        printf("%s\n", menuHandle->topMenuInfo[i]);
    while (p != NULL)
    {
        if (p == menuHandle->selectedMenuItemHandle)
            menuDisplayFunctions.displaySelectedMenuItem(p);
        else
            menuDisplayFunctions.displayMenuItem(p);
        p = p->nextItem;
    }
    puts("=======================================");
    printf("%s\n", menuHandle->bottomMenuInfo);
}

void updateMenu(MenuHandle menuHandle)
{
    MenuItem *p = menuHandle->menuItemListHandle->head;
    while (p != NULL)
    {
        menuDisplayFunctions.moveCursor(p->pos.x, p->pos.y);
        if (p == menuHandle->selectedMenuItemHandle)
            menuDisplayFunctions.displaySelectedMenuItem(p);
        else
            menuDisplayFunctions.displayMenuItem(p);
        p = p->nextItem;
    }
    menuDisplayFunctions.moveCursor(menuHandle->bottomMenuInfoPos.x, menuHandle->bottomMenuInfoPos.y + menuHandle->bottomMenuInfoRows);
    printf("command is [%c]\n", menuHandle->selectedMenuItemTag);
}

MenuItemHandle findMenuItem(MenuHandle menuHandle, const char tag)
{
    MenuItem *p = menuHandle->menuItemListHandle->head;
    while (p != NULL)
    {
        if (p->tag == tag)
            return p;
        p = p->nextItem;
    }
    return NULL;
}

void updateSelectedMenuItem(ChangeMenuItemAction itemAction)
{
    if (currentMenuHandle == NULL)
        return;
    char currentMenuItemTag = currentMenuHandle->selectedMenuItemTag;
    if (itemAction == UP)
    {
        currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->selectedMenuItemHandle->prevItem;
        currentMenuHandle->selectedMenuItemTag = (currentMenuItemTag - 1) - 'A' < 0 ? 'A' + currentMenuHandle->menuItemListHandle->count - 1 : currentMenuItemTag - 1;
        if (currentMenuHandle->selectedMenuItemHandle == NULL)
            currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->menuItemListHandle->tail;
    }
    else if (itemAction == DOWN)
    {
        currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->selectedMenuItemHandle->nextItem;
        currentMenuHandle->selectedMenuItemTag = 'A' + (((currentMenuItemTag + 1) - 'A') % currentMenuHandle->menuItemListHandle->count);
        if (currentMenuHandle->selectedMenuItemHandle == NULL)
            currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->menuItemListHandle->head;
    }
    else
    {
        currentMenuHandle->selectedMenuItemHandle = findMenuItem(currentMenuHandle, itemAction);
        currentMenuHandle->selectedMenuItemTag = currentMenuHandle->selectedMenuItemHandle->tag;
    }
    updateMenu(currentMenuHandle);
    return;
}

int isCurrentMenu(MenuHandle menuHandle)
{
    menuDisplayFunctions.moveCursor(currentMenuHandle->bottomMenuInfoPos.x, currentMenuHandle->bottomMenuInfoPos.y + currentMenuHandle->bottomMenuInfoRows - 1);
    updateCurrnetMenuTime();
    printf("%s\n", menuHandle->bottomMenuInfo);
    return currentMenuHandle == menuHandle;
}

void runMainMenu(MenuHandle menuHandle)
{
    if (menuHandle == NULL)
        return;
    updateCurrentMenu(menuHandle);
    while (1)
    {
        currentMenuHandle->loop(currentMenuHandle);
    }
}

void updateCurrentMenu(MenuHandle menuHandle)
{
    if (menuHandle == NULL)
        return;
    currentMenuHandle = menuHandle;
    displayMenu(currentMenuHandle);
}

void enterMenuAction(struct MenuItem *self)
{
    self->un.enter.nextMenu->selectedMenuItemHandle = self->un.exit.prevMenu->menuItemListHandle->head;
    self->un.enter.nextMenu->selectedMenuItemTag = self->un.exit.prevMenu->menuItemListHandle->head->tag;
    updateCurrentMenu(self->un.enter.nextMenu);
}

void exitMenuAction(struct MenuItem *self)
{
    updateCurrentMenu(self->un.exit.prevMenu);
}

MenuItemHandle initExecFuncMenuItem(const char *name)
{
    if (name == NULL)
        return NULL;
    MenuItemHandle menuItemHandle = (MenuItemHandle)malloc(sizeof(MenuItem));
    if (menuItemHandle == NULL)
        return NULL;
    menuItemHandle->name = name;
    menuItemHandle->tag = '\0';
    menuItemHandle->pos.x = 0;
    menuItemHandle->pos.y = 0;
    menuItemHandle->type = EXECUTIVE_FUNCTION_TYPE;
    menuItemHandle->nextItem = NULL;
    menuItemHandle->prevItem = NULL;
    return menuItemHandle;
}

MenuItemHandle initChangeMenuItem(const char *name, MenuItemType type, MenuHandle menuHandle)
{
    if (name == NULL || menuHandle == NULL)
        return NULL;
    MenuItemHandle menuItemHandle = (MenuItemHandle)malloc(sizeof(MenuItem));
    if (menuItemHandle == NULL)
        return NULL;
    menuItemHandle->name = name;
    menuItemHandle->tag = '\0';
    menuItemHandle->pos.x = 0;
    menuItemHandle->pos.y = 0;
    menuItemHandle->type = type;
    if (type == ENTER_MENU_TYPE)
    {
        menuItemHandle->un.enter.enterMenuAction = enterMenuAction;
        menuItemHandle->un.enter.nextMenu = menuHandle;
    }
    else if (type == EXIT_MENU_TYPE)
    {
        menuItemHandle->un.exit.exitMenuAction = exitMenuAction;
        menuItemHandle->un.exit.prevMenu = menuHandle;
    }
    else
        return NULL;
    menuItemHandle->nextItem = NULL;
    menuItemHandle->prevItem = NULL;
    return menuItemHandle;
}

void initMenuDisplayFunctions(void (*displayMenuItem)(MenuItemHandle), void (*displaySelectedMenuItem)(MenuItemHandle), void (*moveCursor)(int, int))
{
    menuDisplayFunctions.displayMenuItem = displayMenuItem;
    menuDisplayFunctions.displaySelectedMenuItem = displaySelectedMenuItem;
    menuDisplayFunctions.moveCursor = moveCursor;
}

MenuHandle initMenu(void (*loop)(MenuHandle), char **topMenuInfo, char *bottomMenuInfo)
{
    MenuHandle menuHandle = (MenuHandle)malloc(sizeof(Menu));
    if (menuHandle == NULL)
        return NULL;
    menuHandle->topMenuInfoPos.x = 0;
    menuHandle->topMenuInfoPos.y = 0;
    menuHandle->topMenuInfoRows = 8;
    menuHandle->bottomMenuInfoPos.x = 0;
    menuHandle->bottomMenuInfoPos.y = 8;
    menuHandle->bottomMenuInfoRows = 2;
    menuHandle->menuItemListHandle = (MenuItemListHandle)malloc(sizeof(MenuItemList));
    menuHandle->menuItemListHandle->head = NULL;
    menuHandle->menuItemListHandle->tail = NULL;
    menuHandle->menuItemListHandle->count = 0;
    menuHandle->selectedMenuItemHandle = NULL;
    menuHandle->selectedMenuItemTag = '\0';
    menuHandle->loop = loop;
    menuHandle->topMenuInfo = topMenuInfo;
    menuHandle->bottomMenuInfo = (char *)malloc(MAXSIZE * sizeof(char));
    strcpy(menuHandle->bottomMenuInfo, bottomMenuInfo);
    return menuHandle;
}

void registerMenuItem(MenuHandle menuHandle, MenuItemHandle menuItemHandle)
{
    if (menuHandle == NULL || menuItemHandle == NULL)
        return;
    if (menuHandle->menuItemListHandle->head == NULL)
    {
        menuItemHandle->nextItem = NULL;
        menuItemHandle->prevItem = NULL;
        menuHandle->menuItemListHandle->head = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
        menuHandle->selectedMenuItemHandle = menuItemHandle;
        menuHandle->selectedMenuItemTag = 'A';
    }
    else
    {
        menuItemHandle->nextItem = NULL;
        menuItemHandle->prevItem = menuHandle->menuItemListHandle->tail;
        menuHandle->menuItemListHandle->tail->nextItem = menuItemHandle;
        menuHandle->menuItemListHandle->head->prevItem = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
    }
    menuItemHandle->tag = 'A' + menuHandle->menuItemListHandle->count;
    menuItemHandle->pos.x = 0;
    menuItemHandle->pos.y = menuItemHandle->tag - 'A' + menuHandle->topMenuInfoPos.y + menuHandle->topMenuInfoRows;
    menuHandle->bottomMenuInfoPos.x = 0;
    // menuHandle->bottomMenuInfoPos.y = menuItemHandle->pos.y + 1;
    menuHandle->bottomMenuInfoPos.y += 1;
    const char *name = menuItemHandle->name;
    menuItemHandle->name = (char *)malloc(strlen(menuItemHandle->name) + 5);
    sprintf(menuItemHandle->name, "[%c] %s", menuItemHandle->tag, name);
    menuHandle->menuItemListHandle->count++;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCMenu.h"

static MenuHandle currentMenuHandle = NULL;

char getSelectedMenuItemTag()
{
    return currentMenuHandle->selectedMenuItemHandle->tag;
}

void changeCurrentMenu()
{
    if (currentMenuHandle == NULL)
        return NULL;
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

void display(MenuHandle menuHandle)
{
    MenuItem *p = menuHandle->menuItemListHandle->head;
    system("cls");
    puts("Menu:");
    puts("===================================");
    while (p != NULL)
    {
        if (p == menuHandle->selectedMenuItemHandle)
            menuHandle->displaySelectedMenuItem(p);
        else
            menuHandle->displayMenuItem(p);
        p = p->nextItem;
    }
    puts("===================================");
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
        currentMenuHandle->selectedMenuItemTag = 'A' + ((currentMenuItemTag + 1) - 'A' % currentMenuHandle->menuItemListHandle->count);
        if (currentMenuHandle->selectedMenuItemHandle == NULL)
            currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->menuItemListHandle->head;
    }
    else
    {
        currentMenuHandle->selectedMenuItemHandle = findMenuItem(currentMenuHandle, itemAction);
        currentMenuHandle->selectedMenuItemTag = currentMenuHandle->selectedMenuItemHandle->tag;
    }
    display(currentMenuHandle);
    return;
}

int isCurrentMenu(MenuHandle menuHandle)
{
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
    display(currentMenuHandle);
}

void enterMenuAction(struct MenuItem *self)
{
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

MenuHandle initMenu(void (*displayMenuItem)(MenuItemHandle), void (*displaySelectedMenuItem)(MenuItemHandle), void (*loop)(MenuHandle))
{
    MenuHandle menuHandle = (MenuHandle)malloc(sizeof(Menu));
    if (menuHandle == NULL)
        return NULL;
    menuHandle->menuItemListHandle = (MenuItemListHandle)malloc(sizeof(MenuItemList));
    menuHandle->menuItemListHandle->head = NULL;
    menuHandle->menuItemListHandle->tail = NULL;
    menuHandle->menuItemListHandle->count = 0;
    menuHandle->selectedMenuItemHandle = NULL;
    menuHandle->selectedMenuItemTag = '\0';
    menuHandle->displayMenuItem = displayMenuItem;
    menuHandle->displaySelectedMenuItem = displaySelectedMenuItem;
    menuHandle->loop = loop;
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
    const char *name = menuItemHandle->name;
    menuItemHandle->name = (char *)malloc(strlen(menuItemHandle->name) + 5);
    sprintf(menuItemHandle->name, "[%c] %s", menuItemHandle->tag, name);
    menuHandle->menuItemListHandle->count++;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCMenu.h"

static MenuHandle currentMenuHandle = NULL;

void triggerCurrentMenuAction()
{
    if (currentMenuHandle == NULL)
        return;
    switch (currentMenuHandle->selectedMenuItemHandle->type)
    {
    case EXECUTIVE_FUNCTION_TYPE:
        currentMenuHandle->selectedMenuItemHandle->un.action();
        break;
    case ENTER_MENU_TYPE:
        currentMenuHandle->selectedMenuItemHandle->un.enter.enterMenuAction(currentMenuHandle->selectedMenuItemHandle);
        break;
    case EXIT_MENU_TYPE:
        currentMenuHandle->selectedMenuItemHandle->un.exit.exitMenuAction(currentMenuHandle->selectedMenuItemHandle);
        break;
    default:
        break;
    }
    return;
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

void updateCurrentMenuItem(ChangeMenuItemAction itemAction)
{
    if (currentMenuHandle == NULL)
        return;
    if (itemAction == UP)
    {
        currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->selectedMenuItemHandle->prevItem;
        if (currentMenuHandle->selectedMenuItemHandle == NULL)
            currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->menuItemListHandle->tail;
    }
    else if (itemAction == DOWN)
    {
        currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->selectedMenuItemHandle->nextItem;
        if (currentMenuHandle->selectedMenuItemHandle == NULL)
            currentMenuHandle->selectedMenuItemHandle = currentMenuHandle->menuItemListHandle->head;
    }
    else
    {
        currentMenuHandle->selectedMenuItemHandle = findMenuItem(currentMenuHandle, itemAction);
    }
    display(currentMenuHandle);
    return;
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

MenuItemHandle initMenuItem(const char *name, MenuItemType type, void (*action)(), MenuHandle prevMenu, MenuHandle nextMenu)
{
    if (name == NULL || (type == EXECUTIVE_FUNCTION_TYPE && action == NULL))
        return NULL;
    MenuItemHandle menuItemHandle = (MenuItemHandle)malloc(sizeof(MenuItem));
    if (menuItemHandle == NULL)
        return NULL;
    menuItemHandle->name = name;
    menuItemHandle->tag = '\0';
    menuItemHandle->type = type;
    if (type == EXECUTIVE_FUNCTION_TYPE)
        menuItemHandle->un.action = action;
    else if (type == ENTER_MENU_TYPE)
    {
        menuItemHandle->un.enter.enterMenuAction = enterMenuAction;
        menuItemHandle->un.enter.nextMenu = nextMenu;
    }
    else if (type == EXIT_MENU_TYPE)
    {
        menuItemHandle->un.exit.exitMenuAction = exitMenuAction;
        menuItemHandle->un.exit.prevMenu = prevMenu;
    }
    else
        return NULL;
    menuItemHandle->nextItem = NULL;
    menuItemHandle->prevItem = NULL;
    return menuItemHandle;
}

MenuHandle initMenu(void (*displayMenuItem)(MenuItemHandle), void (*displaySelectedMenuItem)(MenuItemHandle))
{
    MenuHandle menuHandle = (MenuHandle)malloc(sizeof(Menu));
    if (menuHandle == NULL)
        return NULL;
    menuHandle->menuItemListHandle = (MenuItemListHandle)malloc(sizeof(MenuItemList));
    menuHandle->menuItemListHandle->head = NULL;
    menuHandle->menuItemListHandle->tail = NULL;
    menuHandle->menuItemListHandle->count = 0;
    menuHandle->selectedMenuItemHandle = NULL;
    menuHandle->displayMenuItem = displayMenuItem;
    menuHandle->displaySelectedMenuItem = displaySelectedMenuItem;
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
    menuHandle->menuItemListHandle->count++;
}

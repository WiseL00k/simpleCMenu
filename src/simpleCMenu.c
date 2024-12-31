#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCMenu.h"

static MenuHandle currentMenuHandle = NULL;

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
        p = p->next;
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
        p = p->next;
    }
    return NULL;
}

void updateCurrentMenuItemHandle(MenuHandle menuHandle, ChangeMenuItemAction action)
{
    if (menuHandle == NULL)
        return;
    if (action == UP)
    {
        menuHandle->selectedMenuItemHandle = menuHandle->selectedMenuItemHandle->prev;
        if (menuHandle->selectedMenuItemHandle == NULL)
            menuHandle->selectedMenuItemHandle = menuHandle->menuItemListHandle->tail;
    }
    else if (action == DOWN)
    {
        menuHandle->selectedMenuItemHandle = menuHandle->selectedMenuItemHandle->next;
        if (menuHandle->selectedMenuItemHandle == NULL)
            menuHandle->selectedMenuItemHandle = menuHandle->menuItemListHandle->head;
    }
    else
    {
        menuHandle->selectedMenuItemHandle = findMenuItem(menuHandle, action);
    }
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
    updateCurrentMenu(self->un.nextMenu);
}

MenuItemHandle initMenuItem(const char *name, MenuItemType type, void (*action)())
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
    else if (type == ENTER_NEXT_MENU_TYPE)
        menuItemHandle->un.enterMenuAction = enterMenuAction;
    else
        return NULL;
    menuItemHandle->next = NULL;
    menuItemHandle->prev = NULL;
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
        menuItemHandle->next = NULL;
        menuItemHandle->prev = NULL;
        menuHandle->menuItemListHandle->head = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
        menuHandle->selectedMenuItemHandle = menuItemHandle;
    }
    else
    {
        menuItemHandle->next = NULL;
        menuItemHandle->prev = menuHandle->menuItemListHandle->tail;
        menuHandle->menuItemListHandle->tail->next = menuItemHandle;
        menuHandle->menuItemListHandle->head->prev = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
    }
    menuItemHandle->tag = 'A' + menuHandle->menuItemListHandle->count;
    menuHandle->menuItemListHandle->count++;
}

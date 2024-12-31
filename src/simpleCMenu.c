#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleCMenu.h"

static MenuHandle currentMenuHandle = NULL;
static MenuItemHandle currentMenuItemHandle = NULL;

void display(MenuItemListHandle menuItemListHandle)
{
    MenuItem *p = menuItemListHandle->head;
    system("cls");
    puts("Menu:");
    while (p != NULL)
    {
        printf("\t%s\n", p->name);
        p = p->next;
    }
}

void updateCurrentMenuItemHandle(MenuItemHandle menuItemHandle)
{
    if (currentMenuItemHandle == NULL)
        return;
    currentMenuItemHandle = menuItemHandle;
}

void updateCurrentMenu(MenuHandle menuHandle)
{
    if (menuHandle == NULL)
        return;
    currentMenuHandle = menuHandle;
    display(currentMenuHandle->menuItemListHandle);
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
    menuItemHandle->type = type;
    if (type == EXECUTIVE_FUNCTION_TYPE)
        menuItemHandle->un.action = action;
    else if (type == ENTER_NEXT_MENU_TYPE)
        menuItemHandle->un.enterMenuAction = enterMenuAction;
    else
        return NULL;
    menuItemHandle->next = NULL;
    return menuItemHandle;
}

MenuHandle initMenu()
{
    MenuHandle menuHandle = (MenuHandle)malloc(sizeof(Menu));
    if (menuHandle == NULL)
        return NULL;
    menuHandle->menuItemListHandle = (MenuItemListHandle)malloc(sizeof(MenuItemList));
    menuHandle->menuItemListHandle->head = NULL;
    menuHandle->menuItemListHandle->tail = NULL;
    menuHandle->menuItemListHandle->count = 0;
    menuHandle->display = display;
    return menuHandle;
}

void registerMenuItem(MenuHandle menuHandle, MenuItemHandle menuItemHandle)
{
    if (menuHandle == NULL || menuItemHandle == NULL)
        return;
    if (menuHandle->menuItemListHandle->head == NULL)
    {
        menuHandle->menuItemListHandle->head = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
    }
    else
    {
        menuHandle->menuItemListHandle->tail->next = menuItemHandle;
        menuHandle->menuItemListHandle->tail = menuItemHandle;
    }
    menuHandle->menuItemListHandle->count++;
}

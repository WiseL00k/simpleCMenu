#include <stdio.h>
#include "simpleCMenu.h"

int main(void)
{
    MenuHandle mainMenu = initMenu();
    MenuItemHandle mainMenuItem1 = initMenuItem("Item1", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem1);
    MenuItemHandle mainMenuItem2 = initMenuItem("Item2", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem2);
    MenuItemHandle mainMenuItem3 = initMenuItem("Item3", ENTER_NEXT_MENU_TYPE, NULL);
    registerMenuItem(mainMenu, mainMenuItem3);

    mainMenu->display(mainMenu->menuItemListHandle);

    return 0;
}
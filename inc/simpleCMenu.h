#ifndef SIMPLE_CMENU_H
#define SIMPLE_CMENU_H

typedef enum
{
    EXECUTIVE_FUNCTION_TYPE,
    ENTER_NEXT_MENU_TYPE
} MenuItemType;

typedef struct Menu *MenuHandle;

// 菜单项
typedef struct MenuItem
{
    MenuItemType type; // 菜单项类型
    const char *name;        // 菜单项名称
    union
    {
        // 类型为EXECUTIVE_FUNCTION_TYPE本项有意义
        void (*action)(); // 菜单项动作

        // 类型为ENTER_NEXT_MENU_TYPE才本项有意义
        void (*enterMenuAction)(struct MenuItem *self); // 进入下一级菜单项动作
        MenuHandle nextMenu;                       // 下一级菜单
    } un;
    struct MenuItem *next; // 下一个菜单项
} MenuItem, *MenuItemHandle;

typedef struct MenuItemList
{
    MenuItem *head; // 菜单项头
    MenuItem *tail; // 菜单项尾
    int count;      // 菜单项数量
} MenuItemList, *MenuItemListHandle;

// 菜单
typedef struct Menu
{
    MenuItemList *menuItemListHandle;                       // 菜单项列表
    void (*display)(MenuItemListHandle menuItemListHandle); // 显示菜单
} Menu, *MenuHandle;

/* 初始化菜单项
 * @param name 菜单项名称
 * @param type 菜单项类型
 * @param action 菜单项动作, 当菜单项类型为EXECUTIVE_FUNCTION_TYPE时有效, 否则为NULL
 */
MenuItemHandle initMenuItem(const char *name, MenuItemType type, void (*action)());
MenuHandle initMenu();
void registerMenu(MenuHandle menuHandle);
void registerMenuItem(MenuHandle menuHandle, MenuItemHandle menuItemHandle);
void updateCurrentMenu(MenuHandle menuHandle);
void updateCurrentMenuItemHandle(MenuItemHandle menuItemHandle);

#endif // SIMPLE_CMENU_H

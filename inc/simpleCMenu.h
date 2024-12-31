#ifndef SIMPLE_CMENU_H
#define SIMPLE_CMENU_H

#define UP -1
#define DOWN 1

typedef char ChangeMenuItemAction;

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
    const char *name;  // 菜单项名称
    char tag;   // 菜单项标签
    union
    {
        // 类型为EXECUTIVE_FUNCTION_TYPE本项有意义
        void (*action)(); // 菜单项动作

        // 类型为ENTER_NEXT_MENU_TYPE才本项有意义
        void (*enterMenuAction)(struct MenuItem *self); // 进入下一级菜单项动作
        MenuHandle nextMenu;                            // 下一级菜单
    } un;
    struct MenuItem *prev; // 上一个菜单项
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
    MenuItemList *menuItemListHandle;                // 菜单项列表
    MenuItemHandle selectedMenuItemHandle;           // 当前选中的菜单项
    void (*displayMenuItem)(MenuItemHandle);         // 显示菜单项
    void (*displaySelectedMenuItem)(MenuItemHandle); // 显示当前选中的菜单项

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

void updateCurrentMenuItemHandle(MenuHandle menuHandle, ChangeMenuItemAction action);
void display(MenuHandle menuHandle);

#endif // SIMPLE_CMENU_H

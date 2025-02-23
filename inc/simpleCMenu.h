#ifndef SIMPLE_CMENU_H
#define SIMPLE_CMENU_H

#define UP -1
#define DOWN 1

#define MAXSIZE 25

typedef char ChangeMenuItemAction;

typedef enum
{
    EXECUTIVE_FUNCTION_TYPE,
    ENTER_MENU_TYPE,
    EXIT_MENU_TYPE,
} MenuItemType;

typedef struct Menu *MenuHandle;

// 菜单项动作参数 用户可自行添加更改不同的参数类型
typedef struct ActionArgs
{
    int num;         // 第一个参数：整数
    const char *str; // 第二个参数：字符串
} ActionArgs;

// 菜单项
typedef struct MenuItem
{
    MenuItemType type; // 菜单项类型
    char *name;        // 菜单项名称
    char tag;          // 菜单项标签
    struct
    {
        int x; // 列
        int y; // 行
    } pos;     // 菜单项坐标
    union
    {
        // 类型为CHANGE_MENU_TYPE才本项有意义
        struct
        {
            void (*enterMenuAction)(struct MenuItem *);

            MenuHandle nextMenu; // 下一级菜单
        } enter;

        struct
        {
            void (*exitMenuAction)(struct MenuItem *);
            MenuHandle prevMenu; // 上一级菜单
        } exit;

    } un;

    struct MenuItem *prevItem; // 上一个菜单项
    struct MenuItem *nextItem; // 下一个菜单项
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
    char **topMenuInfo; // 顶部菜单信息
    struct
    {
        int x;           // 列
        int y;           // 行
    } topMenuInfoPos;    // 顶部菜单信息行坐标
    int topMenuInfoRows; // 顶部菜单信息行数

    char *bottomMenuInfo; // 底部菜单信息
    struct
    {
        int x;              // 列
        int y;              // 行
    } bottomMenuInfoPos;    // 底部菜单信息行
    int bottomMenuInfoRows; // 底部菜单信息行数

    MenuItemList *menuItemListHandle;      // 菜单项列表
    MenuItemHandle selectedMenuItemHandle; // 当前选中的菜单项
    char selectedMenuItemTag;              // 当前选中的菜单项标签
    void (*loop)(MenuHandle);              // 菜单循环函数
} Menu, *MenuHandle;

typedef struct
{
    void (*displayMenuItem)(MenuItemHandle);         // 显示菜单项
    void (*displaySelectedMenuItem)(MenuItemHandle); // 显示当前选中的菜单项
    void (*moveCursor)(int x, int y);                // 移动光标
} MenuDisplayFunctions;

/* 初始化菜单项
 * @param name 菜单项名称
 * @param type 菜单项类型
 * @param action 菜单项动作, 当菜单项类型为EXECUTIVE_FUNCTION_TYPE时有效, 否则为NULL
 */
MenuItemHandle initExecFuncMenuItem(const char *name);
MenuItemHandle initChangeMenuItem(const char *name, MenuItemType type, MenuHandle menuHandle);
MenuHandle initMenu(void (*loop)(MenuHandle), char **topMenuInfo, char *bottomMenuInfo);
void initMenuDisplayFunctions(void (*displayMenuItem)(MenuItemHandle), void (*displaySelectedMenuItem)(MenuItemHandle), void (*moveCursor)(int, int));
void registerMenu(MenuHandle menuHandle);
void registerMenuItem(MenuHandle menuHandle, MenuItemHandle menuItemHandle);
void updateCurrentMenu(MenuHandle menuHandle);
void updateSelectedMenuItem(ChangeMenuItemAction itemAction);
char getSelectedMenuItemTag();
void changeCurrentMenu();
int isCurrentMenu(MenuHandle menuHandle);
void runMainMenu(MenuHandle menuHandle);

#endif // SIMPLE_CMENU_H

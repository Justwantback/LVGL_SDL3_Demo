#ifndef __MENULIST_H__
#define __MENULIST_H__

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 创建菜单列表组件
 * @param parent 父对象
 * @param w 宽度 (可以传 lv_pct(100) 等值，但在C中通常是int，如果是坐标值则int)
 *          为了灵活性，这里设计为 int, 内部使用 lv_obj_set_size. 
 *          如果需要百分比，建议创建后手动设置，或者此处约定 -1 为 100%
 * @param h 高度
 * @return 列表容器对象
 */
lv_obj_t *ui_create_menulist(lv_obj_t *parent, int w, int h);

/**
 * 添加菜单项 (按钮)
 * @param list 菜单列表对象
 * @param text 按钮文本
 * @param click_cb 点击事件回调
 * @return 新创建的按钮对象
 */
lv_obj_t *ui_menulist_add_item(lv_obj_t *list, const char *text, lv_event_cb_t click_cb);

/**
 * 设置选中项 (高亮)
 * @param list 菜单列表对象
 * @param index 索引 (0 based)
 */
void ui_menulist_set_selected(lv_obj_t *list, int index);

/**
 * 获取当前选中项索引
 * @param list 菜单列表对象
 * @return 索引，无选中返回 -1
 */
int ui_menulist_get_selected(lv_obj_t *list);

#ifdef __cplusplus
}
#endif

#endif /* __MENULIST_H__ */

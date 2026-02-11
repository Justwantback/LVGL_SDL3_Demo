#include "menulist.h"
#include <stdio.h>

/* 定义选中项的样式，可以在 create 时初始化，这里简化处理 */

lv_obj_t *ui_create_menulist(lv_obj_t *parent, int w, int h)
{
    /* 创建列表容器 */
    lv_obj_t *list = lv_obj_create(parent);
    
    /* 设置大小 */
    lv_obj_set_size(list, w, h);
    
    /* 设置布局为纵向排列 */
    lv_obj_set_layout(list, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    
    /* 允许垂直滚动 */
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    
    /* 样式调整：去除默认的内边距，紧凑显示 */
    lv_obj_set_style_pad_column(list, 5, 0); /* 成员间距 */
    lv_obj_set_style_pad_all(list, 10, 0);   /* 容器内边距 */
    
    /* 去除边框（可选，视设计而定） */
    // lv_obj_set_style_border_width(list, 0, 0);
    
    return list;
}

lv_obj_t *ui_menulist_add_item(lv_obj_t *list, const char *text, lv_event_cb_t click_cb)
{
    if (!list) return NULL;

    /* 创建按钮，兼容 LVGL v8/v9 */
    /* 注意：LVGL v9 中 lv_btn_create 变为 lv_button_create，或者使用 lv_obj_create + styles */
    /* 如果编译报错，请将 lv_button_create 改为 lv_btn_create */
#if LVGL_VERSION_MAJOR >= 9
    lv_obj_t *btn = lv_button_create(list);
#else
    lv_obj_t *btn = lv_btn_create(list);
#endif

    /* 设置按钮宽度占满列表 */
    lv_obj_set_width(btn, lv_pct(100));
    lv_obj_set_height(btn, LV_SIZE_CONTENT); /* 高度自适应内容 */
    
    /* 设置最小高度，方便点击 */
    lv_obj_set_style_min_height(btn, 40, 0);

    /* 添加 Label */
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text ? text : "");
    lv_obj_center(label);

    /* 添加点击事件 */
    if (click_cb) {
        lv_obj_add_event_cb(btn, click_cb, LV_EVENT_CLICKED, NULL);
    }
    
    /* 设置为可选中 (Checkable) 以支持选中状态 */
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);

    return btn;
}

void ui_menulist_set_selected(lv_obj_t *list, int index)
{
    if (!list) return;

    uint32_t count = lv_obj_get_child_cnt(list);
    for (uint32_t i = 0; i < count; i++) {
        lv_obj_t *child = lv_obj_get_child(list, i);
        
        /* 清除所有项的选中状态 */
        lv_obj_remove_state(child, LV_STATE_CHECKED);
        
        /* 设置目标项为选中 */
        if (i == (uint32_t)index) {
            lv_obj_add_state(child, LV_STATE_CHECKED);
            /* 自动滚动到选中项 */
            lv_obj_scroll_to_view(child, LV_ANIM_ON);
        }
    }
}

int ui_menulist_get_selected(lv_obj_t *list)
{
    if (!list) return -1;
    
    uint32_t count = lv_obj_get_child_cnt(list);
    for (uint32_t i = 0; i < count; i++) {
        lv_obj_t *child = lv_obj_get_child(list, i);
        if (lv_obj_has_state(child, LV_STATE_CHECKED)) {
            return (int)i;
        }
    }
    return -1;
}

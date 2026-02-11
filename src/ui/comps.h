#ifndef __COMPS_H__
#define __COMPS_H__

#include "lvgl.h"
#include "widget.h"

/**********************
 *      TYPEDEFS
 **********************/

/* 组件容器，作为 Widget 的管理器 */
typedef struct {
    lv_obj_t *container;        /* 组件自身的 LVGL 容器 */
    ui_widget_t *widget_list;   /* 注册的 Widget 链表头 */
} ui_component_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * 创建一个基础 Component
 * @param parent 父对象
 * @return 组件句柄
 */
ui_component_t *ui_component_create(lv_obj_t *parent);

/**
 * 销毁组件及其所有注册的 Widget
 */
void ui_component_delete(ui_component_t *comp);

/**
 * 注册 Widget 到 Component
 */
void ui_component_register_widget(ui_component_t *comp, ui_widget_t *widget);

/**
 * 分发事件给所有注册的 Widget
 * @param comp 组件句柄
 * @param id 事件 ID
 * @param param 事件参数指针
 */
void ui_component_dispatch_event(ui_component_t *comp, ui_event_id_t id, void *param);

/**
 * 轮询检查所有控件的触发器
 * 适合在定时器中调用，传入当前的全局状态数据
 * @param comp 组件句柄
 * @param input_data 外部输入数据
 */
void ui_component_check_triggers(ui_component_t *comp, void *input_data);

#endif /* __COMPS_H__ */

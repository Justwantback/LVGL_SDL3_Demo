#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

/**********************
 *      TYPEDEFS
 **********************/

/* --- 事件定义 --- */

typedef enum {
    UI_EVENT_NONE = 0,
    UI_EVENT_TIME_UPDATE,    /* 参数: ui_event_param_time_t* */
    UI_EVENT_WIFI_STATUS,    /* 参数: int* (level 0-4) */
    UI_EVENT_LTE_STATUS,     /* 参数: int* (level 0-4) */
    UI_EVENT_BATTERY_STATUS, /* 参数: ui_event_param_bat_t* */
    
    /* 触发器产生的自定义事件 */
    UI_EVENT_TRIGGER_CUSTOM_1,
    UI_EVENT_TRIGGER_CUSTOM_2,
} ui_event_id_t;

typedef struct {
    int hour;
    int minute;
} ui_event_param_time_t;

typedef struct {
    int level;      /* 0-100% 或 0-4 */
    bool is_charging;
} ui_event_param_bat_t;

typedef struct {
    ui_event_id_t id;
    void *param;
} ui_event_t;

/* --- Widget 基类定义 --- */

typedef struct ui_widget_s ui_widget_t;

/* 事件处理回调函数原型 */
typedef void (*ui_event_cb_t)(ui_widget_t *widget, const ui_event_t *event);

/* 
 * 触发器函数原型 
 * 执行后返回一个事件 ID，如果无需触发事件则返回 UI_EVENT_NONE
 * @param widget 控件本身
 * @param input_data 外部输入的触发条件数据 (可选)
 */
typedef ui_event_id_t (*ui_trigger_cb_t)(ui_widget_t *widget, void *input_data);

/* 事件注册节点 */
typedef struct ui_event_node_s {
    ui_event_id_t id;
    ui_event_cb_t cb;
    struct ui_event_node_s *next;
} ui_event_node_t;

struct ui_widget_s {
    lv_obj_t *obj;                  /* LVGL 基础对象 */
    ui_event_node_t *event_handlers;/* 注册的事件处理链表 */
    ui_trigger_cb_t trigger_cb;     /* 触发器回调函数 */
    ui_widget_t *next;              /* 链表节点，用于 Component 管理 */
    void *user_data;                /* 用户自定义数据 */
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * 创建一个基础 Widget 包装器
 */
ui_widget_t *ui_widget_create(lv_obj_t *obj);

/**
 * 销毁 Widget
 */
void ui_widget_delete(ui_widget_t *widget);

/**
 * 注册事件处理回调
 */
void ui_widget_register_event(ui_widget_t *widget, ui_event_id_t id, ui_event_cb_t cb);

/**
 * 注册触发器函数
 */
void ui_widget_register_trigger(ui_widget_t *widget, ui_trigger_cb_t cb);

/**
 * 执行触发器
 * 调用内部注册的 trigger_cb，如果返回值 != UI_EVENT_NONE，
 * 则立即在本 Widget 内部 process 该事件。
 * 
 * @param widget 目标控件
 * @param input_data 传给 trigger 的外部数据
 * @return 触发产生的事件 ID (供外部知晓)
 */
ui_event_id_t ui_widget_run_trigger(ui_widget_t *widget, void *input_data);

/**
 * 处理事件 (遍历注册表)
 */
void ui_widget_process_event(ui_widget_t *widget, const ui_event_t *event);

/**
 * 设置用户数据
 */
static inline void ui_widget_set_user_data(ui_widget_t *widget, void *data) {
    if(widget) widget->user_data = data;
}

/**
 * 获取用户数据
 */
static inline void *ui_widget_get_user_data(ui_widget_t *widget) {
    return widget ? widget->user_data : NULL;
}

#endif /* __WIDGET_H__ */

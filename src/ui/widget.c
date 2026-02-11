#include "widget.h"
#include <stdio.h>
#include <stdlib.h>

/* --- Widget Base Implementation --- */

ui_widget_t *ui_widget_create(lv_obj_t *obj)
{
    if (!obj) return NULL;

    ui_widget_t *widget = (ui_widget_t *)malloc(sizeof(ui_widget_t));
    if (!widget) return NULL;

    widget->obj = obj;
    widget->event_handlers = NULL;
    widget->trigger_cb = NULL;
    widget->next = NULL;
    widget->user_data = NULL;

    return widget;
}

void ui_widget_delete(ui_widget_t *widget)
{
    if (!widget) return;
    
    /* 释放事件链表 */
    ui_event_node_t *curr = widget->event_handlers;
    while (curr) {
        ui_event_node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    widget->event_handlers = NULL;
    
    free(widget);
}

void ui_widget_register_event(ui_widget_t *widget, ui_event_id_t id, ui_event_cb_t cb)
{
    if (!widget || !cb) return;

    ui_event_node_t *check = widget->event_handlers;
    while (check) {
        if (check->id == id && check->cb == cb) return;
        check = check->next;
    }

    ui_event_node_t *node = (ui_event_node_t *)malloc(sizeof(ui_event_node_t));
    if (!node) return;

    node->id = id;
    node->cb = cb;
    
    node->next = widget->event_handlers;
    widget->event_handlers = node;
}

void ui_widget_register_trigger(ui_widget_t *widget, ui_trigger_cb_t cb)
{
    if (!widget) return;
    widget->trigger_cb = cb;
}

ui_event_id_t ui_widget_run_trigger(ui_widget_t *widget, void *input_data)
{
    if (!widget || !widget->trigger_cb) return UI_EVENT_NONE;

    /* 1. 执行判断逻辑 */
    ui_event_id_t triggered_event_id = widget->trigger_cb(widget, input_data);

    /* 2. 如果产生了有效事件，立即在本 Widget 内部处理 */
    if (triggered_event_id != UI_EVENT_NONE) {
        ui_event_t evt;
        evt.id = triggered_event_id;
        evt.param = input_data; /* 将输入数据直接透传作为事件参数，或者可以在 trigger 中返回更复杂的结构 */
        
        ui_widget_process_event(widget, &evt);
    }

    return triggered_event_id;
}

void ui_widget_process_event(ui_widget_t *widget, const ui_event_t *event)
{
    if (!widget || !event) return;

    ui_event_node_t *curr = widget->event_handlers;
    while (curr) {
        if (curr->id == event->id) {
            if (curr->cb) {
                curr->cb(widget, event);
            }
        }
        curr = curr->next;
    }
}

#include "comps.h"
#include <stdio.h>
#include <stdlib.h>

/* --- Component Base Implementation --- */

ui_component_t *ui_component_create(lv_obj_t *parent)
{
    ui_component_t *comp = (ui_component_t *)malloc(sizeof(ui_component_t));
    if (!comp) return NULL;

    comp->container = lv_obj_create(parent);
    comp->widget_list = NULL;

    return comp;
}

void ui_component_delete(ui_component_t *comp)
{
    if (!comp) return;

    ui_widget_t *curr = comp->widget_list;
    while (curr) {
        ui_widget_t *next = curr->next;
        if (ui_widget_get_user_data(curr)) {
            free(ui_widget_get_user_data(curr));
        }
        ui_widget_delete(curr);
        curr = next;
    }

    if (lv_obj_is_valid(comp->container)) {
        lv_obj_delete(comp->container);
    }
    
    free(comp);
}

void ui_component_register_widget(ui_component_t *comp, ui_widget_t *widget)
{
    if (!comp || !widget) return;
    widget->next = comp->widget_list;
    comp->widget_list = widget;
}

void ui_component_dispatch_event(ui_component_t *comp, ui_event_id_t id, void *param)
{
    if (!comp) return;

    ui_event_t event;
    event.id = id;
    event.param = param;

    ui_widget_t *curr = comp->widget_list;
    while (curr) {
        ui_widget_process_event(curr, &event);
        curr = curr->next;
    }
}

void ui_component_check_triggers(ui_component_t *comp, void *input_data)
{
    if (!comp) return;
    ui_widget_t *curr = comp->widget_list;
    while (curr) {
        ui_widget_run_trigger(curr, input_data);
        curr = curr->next;
    }
}

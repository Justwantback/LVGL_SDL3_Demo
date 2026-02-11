#include "statusbar.h"
#include <stdio.h>
#include <stdlib.h>

/* --- 资源定义 --- */
static const void *WIFI_ICONS[] = {
    LV_SYMBOL_WIFI, LV_SYMBOL_WIFI, LV_SYMBOL_WIFI, LV_SYMBOL_WIFI, LV_SYMBOL_WIFI
};
static const void *LTE_ICONS[] = {
    LV_SYMBOL_SHUFFLE, LV_SYMBOL_SHUFFLE, LV_SYMBOL_SHUFFLE, LV_SYMBOL_SHUFFLE, LV_SYMBOL_SHUFFLE
};
static const void *BAT_ICONS[] = {
    LV_SYMBOL_BATTERY_EMPTY, LV_SYMBOL_BATTERY_1, LV_SYMBOL_BATTERY_2, LV_SYMBOL_BATTERY_3, LV_SYMBOL_BATTERY_FULL
};
static const void *BAT_CHARGING_ICON = LV_SYMBOL_CHARGE;

/* --- 私有状态数据结构 --- */
typedef struct {
    int current_level;
} ui_icon_state_t;

typedef struct {
    int current_level;
    bool is_charging;
} ui_bat_state_t;

/* --- 具体事件处理逻辑 (Callback) --- */

static void on_time_update(ui_widget_t *widget, const ui_event_t *event)
{
    ui_event_param_time_t *p = (ui_event_param_time_t *)event->param;
    if (p && widget->obj) {
        lv_label_set_text_fmt(widget->obj, "%02d:%02d", p->hour, p->minute);
    }
}

static void on_wifi_status(ui_widget_t *widget, const ui_event_t *event)
{
    int level = *((int*)event->param);
    if (level < 0) level = 0; if (level > 4) level = 4;
    
    ui_icon_state_t *state = (ui_icon_state_t *)ui_widget_get_user_data(widget);
    if (state && state->current_level != level) {
        state->current_level = level;
        lv_image_set_src(widget->obj, WIFI_ICONS[level]);
    }
}

static void on_lte_status(ui_widget_t *widget, const ui_event_t *event)
{
    int level = *((int*)event->param);
    if (level < 0) level = 0; if (level > 4) level = 4;
    
    ui_icon_state_t *state = (ui_icon_state_t *)ui_widget_get_user_data(widget);
    if (state && state->current_level != level) {
        state->current_level = level;
        lv_image_set_src(widget->obj, LTE_ICONS[level]);
    }
}

static void on_battery_status(ui_widget_t *widget, const ui_event_t *event)
{
    ui_event_param_bat_t *p = (ui_event_param_bat_t *)event->param;
    int idx = 0;
    if (p->level > 80) idx = 4;
    else if (p->level > 60) idx = 3;
    else if (p->level > 40) idx = 2;
    else if (p->level > 20) idx = 1;
    else idx = 0;

    ui_bat_state_t *state = (ui_bat_state_t *)ui_widget_get_user_data(widget);
    if (state && (state->current_level != idx || state->is_charging != p->is_charging)) {
        state->current_level = idx;
        state->is_charging = p->is_charging;
        
        if (p->is_charging) {
            lv_image_set_src(widget->obj, BAT_CHARGING_ICON);
        } else {
            lv_image_set_src(widget->obj, BAT_ICONS[idx]);
        }
    }
}

/* --- 触发器示例 --- */
/* Wifi 信号触发器：检查原始 RSSI 值，如果不合格则触发事件 */
static ui_event_id_t trigger_wifi_check(ui_widget_t *widget, void *input_data)
{
    int rssi = *((int*)input_data); /* 假设输入是 RSSI 强度 */
    
    /* 模拟：如果 RSSI < -80，则强制设置为 0 格信号 */
    if (rssi < -80) {
        /* 注意：这里我们修改一下输入数据，以便后续 handler 使用修改后的值 */
        /* 在实际场景中，input_data 可能是只读的，你需要构造新的 param */
        *((int*)input_data) = 0; 
        return UI_EVENT_WIFI_STATUS;
    }
    
    /* 正常情况，映射 RSSI 到 0-4 */
    int level = 4;
    if (rssi < -70) level = 3;
    if (rssi < -60) level = 2;
    // ...
    *((int*)input_data) = level;
    return UI_EVENT_WIFI_STATUS;
}

/* --- Status Bar Factory --- */

#define STATUSBAR_HEIGHT    30
#define STATUSBAR_BG_COLOR  lv_color_hex(0x000000)
#define STATUSBAR_TEXT_COLOR lv_color_hex(0xFFFFFF)
#define ICON_GAP            10
#define PADDING_X           15

ui_component_t *ui_statusbar_create(lv_obj_t *parent)
{
    ui_component_t *comp = ui_component_create(parent);
    if (!comp) return NULL;

    /* 样式布局 */
    lv_obj_t *cont = comp->container;
    lv_obj_set_size(cont, lv_pct(100), STATUSBAR_HEIGHT);
    lv_obj_set_style_bg_color(cont, STATUSBAR_BG_COLOR, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_left(cont, PADDING_X, 0);
    lv_obj_set_style_pad_right(cont, PADDING_X, 0);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *left_box = lv_obj_create(cont);
    lv_obj_set_size(left_box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_remove_flag(left_box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(left_box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(left_box, 0, 0);

    lv_obj_t *right_box = lv_obj_create(cont);
    lv_obj_set_size(right_box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_layout(right_box, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(right_box, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(right_box, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(right_box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(right_box, 0, 0);
    lv_obj_set_style_pad_gap(right_box, ICON_GAP, 0);
    lv_obj_remove_flag(right_box, LV_OBJ_FLAG_SCROLLABLE);

    /* 1. Time Widget */
    lv_obj_t *lbl = lv_label_create(left_box);
    lv_label_set_text(lbl, "00:00");
    lv_obj_set_style_text_color(lbl, STATUSBAR_TEXT_COLOR, 0);
    
    ui_widget_t *time_w = ui_widget_create(lbl);
    ui_widget_register_event(time_w, UI_EVENT_TIME_UPDATE, on_time_update);
    ui_component_register_widget(comp, time_w);

    /* 2. Wifi Widget */
    lv_obj_t *img_wifi = lv_image_create(right_box);
    lv_image_set_src(img_wifi, WIFI_ICONS[4]);
    lv_obj_set_style_image_recolor(img_wifi, STATUSBAR_TEXT_COLOR, 0);
    
    ui_widget_t *wifi_w = ui_widget_create(img_wifi);
    ui_icon_state_t *wifi_st = (ui_icon_state_t*)malloc(sizeof(ui_icon_state_t));
    if (wifi_st) { wifi_st->current_level = 4; ui_widget_set_user_data(wifi_w, wifi_st); }
    ui_widget_register_event(wifi_w, UI_EVENT_WIFI_STATUS, on_wifi_status);
    
    /* 注册 Wifi 触发器：用于检查 RSSI 是否过低 */
    ui_widget_register_trigger(wifi_w, trigger_wifi_check);
    
    ui_component_register_widget(comp, wifi_w);

    /* 3. LTE Widget */
    lv_obj_t *img_lte = lv_image_create(right_box);
    lv_image_set_src(img_lte, LTE_ICONS[4]);
    lv_obj_set_style_image_recolor(img_lte, STATUSBAR_TEXT_COLOR, 0);
    
    ui_widget_t *lte_w = ui_widget_create(img_lte);
    ui_icon_state_t *lte_st = (ui_icon_state_t*)malloc(sizeof(ui_icon_state_t));
    if (lte_st) { lte_st->current_level = 4; ui_widget_set_user_data(lte_w, lte_st); }
    ui_widget_register_event(lte_w, UI_EVENT_LTE_STATUS, on_lte_status);
    ui_component_register_widget(comp, lte_w);

    /* 4. Battery Widget */
    lv_obj_t *img_bat = lv_image_create(right_box);
    lv_image_set_src(img_bat, BAT_ICONS[4]);
    lv_obj_set_style_image_recolor(img_bat, STATUSBAR_TEXT_COLOR, 0);
    
    ui_widget_t *bat_w = ui_widget_create(img_bat);
    ui_bat_state_t *bat_st = (ui_bat_state_t*)malloc(sizeof(ui_bat_state_t));
    if (bat_st) { bat_st->current_level = 4; bat_st->is_charging = false; ui_widget_set_user_data(bat_w, bat_st); }
    ui_widget_register_event(bat_w, UI_EVENT_BATTERY_STATUS, on_battery_status);
    ui_component_register_widget(comp, bat_w);

    return comp;
}

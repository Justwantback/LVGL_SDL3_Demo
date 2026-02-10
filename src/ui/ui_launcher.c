#include "ui_launcher.h"


#include <time.h>

/* ================= 状态 ================= */
static lv_obj_t *label_time;
static lv_obj_t *label_count;
static int counter = 0;

/* ================= 时间刷新 ================= */
static void time_update_cb(lv_timer_t *timer)
{
    (void)timer;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M:%S", tm_info);
    lv_label_set_text(label_time, buf);
}

/* ================= 按钮事件 ================= */
static void btn_click_cb(lv_event_t *e)
{
    (void)e;
    counter++;

    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%d", counter);
    lv_label_set_text(label_count, buf);
}


void create_demo_ui(void)
{
    lv_obj_t *scr = lv_scr_act();

    /* ---------- 时间（左上角） ---------- */
    label_time = lv_label_create(scr);
    lv_label_set_text(label_time, "--:--:--");
    lv_obj_align(label_time, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_timer_create(time_update_cb, 1000, NULL);

    /* ---------- 中间计数（大字） ---------- */
    label_count = lv_label_create(scr);
    lv_label_set_text(label_count, "0");

    static lv_style_t style_big;
    lv_style_init(&style_big);
    lv_style_set_text_font(&style_big, &lv_font_montserrat_48);

    lv_obj_add_style(label_count, &style_big, 0);
    lv_obj_align(label_count, LV_ALIGN_CENTER, 0, -20);

    /* ---------- 按钮（下面） ---------- */
    lv_obj_t *btn = lv_button_create(scr);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 60);

    lv_obj_add_event_cb(btn, btn_click_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "+1");
    lv_obj_center(btn_label);
}



int ui_launcher_start()
{
    create_demo_ui();
}